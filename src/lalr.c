#include "lalr.h"
#include "ast.h"
#include "ast_util.h"
#include "sv.h"
#include "tokenizer.h"
#include <stdio.h>
#include "number_parser.h"

lalr_ctx lalr_create() {
	lalr_ctx ctx = {0};
	ctx.stack_top = -1;
	return ctx;
}

void lalr_show_stack(lalr_ctx* ctx) {
	printf("Stack(%d): [", ctx->stack_top);
	for (int i = 0; i <= ctx->stack_top; i++) {
		printf("\n"); printf("%d\n", i); lalr_print_node(ctx->stack[i]);
	}
	printf("]\n");
}

int lalr_reduce_tok_to_term(token tok, AST_Node* out_n) {
	out_n->type = NT_TOKEN;
	out_n->token = tok;
	return 1;
}

int lalr_reduce(lalr_ctx* ctx, AST_Node* out_n) {
	AST_Node peeked[3] = {lalr_peek_n(ctx, 0), lalr_peek_n(ctx, 1), lalr_peek_n(ctx, 2)};

	/**  factor parsing
	 *   	 factor := "(" <expression> ")"
	 *     factor := <id>
	 *     factor := <number>
	 *     factor := <strlit>
	 */
	{
		// factor := <id>
		if (peeked[0].type == NT_TOKEN && peeked[0].token.type == T_ID) {
			out_n->type = NT_FACTOR;
			out_n->factor.type = FACTOR_TYPE_ID;
			out_n->factor.id = peeked[0].token.text;
			return 1;
		}

		// factor := <number>
		if (peeked[0].type == NT_TOKEN && peeked[0].token.type == T_DECIMAL_LIT) {
			out_n->type = NT_FACTOR;
			out_n->factor.type = FACTOR_TYPE_NUMBER;
			out_n->factor.number.type = NUMBER_TYPE_INT;
			out_n->factor.number.i = number_parse_integer_base(peeked[0].token.text.data, 10).ok;
			return 1;
		}

		// factor := <strlit>
		// ?
		if (peeked[0].type == NT_TOKEN && peeked[0].token.type == T_STRING_LIT) {
			out_n->type = NT_FACTOR;
			out_n->factor.type = FACTOR_TYPE_STR;
			out_n->factor.str = peeked[0].token.text;
			return 1;
		}

		// factor := "(" <relate> ")"
		// TODO: Expand this to <expression> rather than <math_expression>
		if (peeked[2].type == NT_TOKEN && peeked[2].token.type == T_LP &&
				peeked[1].type == NT_RELATE &&
				peeked[0].type == NT_TOKEN && peeked[0].token.type == T_RP) {
			out_n->type = NT_FACTOR;
			// out_n->factor.type = FACTOR_TYPE_MATH_EXPR;
			// out_n->factor.mathExpr_test = peeked[1].mathexpr;
			out_n->factor.type = FACTOR_TYPE_RELATE;
			out_n->factor.relational_test = peeked[1].relate;
			return 3;
		}
	}

	/**  term parsing
	 *	   term := <term> "*" <factor>
	 *	   term := <term> "/" <factor>
	 *	   term := <term> "%" <factor>
	 *	   term := <factor>
	 */
	{
		// term := <term> {"*" | "/" | "%"} <factor>
		if ( peeked[2].type == NT_TERM &&
				 peeked[1].type == NT_TOKEN && 
				(peeked[1].token.type == T_MUL || peeked[1].token.type == T_DIV || peeked[1].token.type == T_MOD) &&
				 peeked[0].type == NT_FACTOR) {
			out_n->type = NT_TERM;
			out_n->term = malloc(sizeof(Term));
			out_n->term->type = TERM_TYPE_TERM_OP_FACTOR;
			out_n->term->op = peeked[1].token.text.data[0];
			out_n->term->left = peeked[2].term;
			out_n->term->right = peeked[0].factor;
			return 3;
		}

		// term := <factor>
		if (peeked[0].type == NT_FACTOR) {
			out_n->type = NT_TERM;
			out_n->term = malloc(sizeof(Term));
			out_n->term->type = TERM_TYPE_FACTOR;
			out_n->term->right = peeked[0].factor;
			out_n->term->left = NULL;
			// printf("reducing factor -> term\n");
			// ast_print_term(out_n->term, 3);
			return 1;
		}
	}

	/**  math_expression parsing
	 *	   math_expression := <math_expression> "+" <term>
	 *	   math_expression := <math_expression> "-" <term>
	 *	   math_expression := <term>
	 */
	{
	 	// math_expression := <math_expression> "+/-" <term>
		if (peeked[2].type == NT_MATH_EXPR &&
				peeked[1].type == NT_TOKEN &&
				(peeked[1].token.type == T_PLUS || peeked[1].token.type == T_MINUS) &&
				peeked[0].type == NT_TERM &&
				(ctx->lookahead.type != T_MUL &&
				 ctx->lookahead.type != T_DIV &&
				 ctx->lookahead.type != T_MOD)) {
			out_n->type = NT_MATH_EXPR;
			out_n->mathexpr = malloc(sizeof(MathExpression));
			out_n->mathexpr->type = peeked[1].token.type == T_PLUS ? MATH_EXPR_TYPE_ADD : MATH_EXPR_TYPE_SUB;
			out_n->mathexpr->op = peeked[1].token.text.data[0];
			out_n->mathexpr->left = peeked[2].mathexpr;
			out_n->mathexpr->right = peeked[0].term;

			// printf("Reducing math_expr +- term\n");
			// ast_print_term(peeked[0].term, 3);
			// ast_print_term(out_n->mathexpr->right, 3);
			return 3;
		}

	  // math_expression := <term>
		if (peeked[0].type == NT_TERM &&
				ctx->lookahead.type != T_MUL &&
				ctx->lookahead.type != T_DIV &&
				ctx->lookahead.type != T_MOD) {
			out_n->type = NT_MATH_EXPR;
			out_n->mathexpr = malloc(sizeof(MathExpression));
			out_n->mathexpr->type = MATH_EXPR_TYPE_TERM;
			out_n->mathexpr->right = peeked[0].term;
			return 1;
		}
	}

	/** relational parsing
	 *     relational := relational ">=" math_expression
	 *     relational := relational "<=" math_expression
	 *     relational := relational ">"  math_expression
	 *     relational := relational "<"  math_expression
	 *     relational := relational "==" math_expression
	 *     relational := math_expression
	 */
	{
		// relational := relational ">=" math_expression
		// relational := relational "<=" math_expression
		// relational := relational ">"  math_expression
		// relational := relational "<"  math_expression
		// relational := relational "==" math_expression
		if (peeked[2].type == NT_RELATE &&
				peeked[1].type == NT_TOKEN &&
				peeked[1].token.type >= T_LOGIC_BEG &&
				peeked[1].token.type <= T_LOGIC_END &&
				peeked[0].type == NT_MATH_EXPR) {
			out_n->type = NT_RELATE;
			out_n->relate = calloc(1, sizeof(Relational));

			// relational type
			if (peeked[1].token.type == T_DEQ) out_n->relate->type = RELATIONAL_TYPE_DEQ;
			if (peeked[1].token.type == T_LT) out_n->relate->type = RELATIONAL_TYPE_LT;
			if (peeked[1].token.type == T_LTEQ) out_n->relate->type = RELATIONAL_TYPE_LTEQ;
			if (peeked[1].token.type == T_GT) out_n->relate->type = RELATIONAL_TYPE_GT;
			if (peeked[1].token.type == T_GTEQ) out_n->relate->type = RELATIONAL_TYPE_GTEQ;

			out_n->relate->relate = peeked[2].relate;
			out_n->relate->mathexpr = peeked[0].mathexpr;
			out_n->relate->op = peeked[1].token.text;
			printf("relational (op=" SV_Fmt ") math_expr\n", SV_Arg(out_n->relate->op));
			return 3;
		}

	  // relational := math_expression
		if (peeked[0].type == NT_MATH_EXPR &&
				ctx->lookahead.type != T_PLUS &&
				ctx->lookahead.type != T_MINUS
			 ) {
				//ctx->lookahead.type != T_RP) {
			printf("Reduced math_expr -> relational\n");
			out_n->type = NT_RELATE;
			out_n->relate = calloc(1, sizeof(Relational));
			out_n->relate->type = RELATIONAL_TYPE_MATH_EXPR;
			out_n->relate->mathexpr = peeked[0].mathexpr;
			return 1;
		}
	}

	return 0;
}

void lalr_push(lalr_ctx *ctx, AST_Node n) {
	ctx->stack[++ctx->stack_top] = n;
}

void lalr_pop(lalr_ctx *ctx) {
	ctx->stack_top--;
	if (ctx->stack_top < -1) {
		ctx->stack_top = -1;
	}
}

void lalr_pop_n(lalr_ctx* ctx, int n) {
	for (int i = 0; i < n; i++) {
		lalr_pop(ctx);
	}
}

AST_Node lalr_peek(lalr_ctx *ctx, AST_Node n) {
	return lalr_peek_n(ctx, 0);
}

AST_Node lalr_peek_n(lalr_ctx *ctx, int n) {
	if (ctx->stack_top - n < 0) {
		return (AST_Node){.type=NT_UNDEF}; // no peeking an empty stack
	}
	return ctx->stack[ctx->stack_top - n];
}

AST_Node* lalr_peek_pn(lalr_ctx* ctx, int n) {
	if (ctx->stack_top - n < 0) {
		return NULL; // no peeking an empty stack
	}
	return &ctx->stack[ctx->stack_top - n];
}

void lalr_print_node(AST_Node node) {
	ast_print_node(node, 0);
}