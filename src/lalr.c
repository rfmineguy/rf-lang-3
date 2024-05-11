#include "lalr.h"
#include "ast.h"
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
	printf("Stack: \n");
	for (int i = 0; i <= ctx->stack_top; i++) {
		printf("%d: ", i);
		lalr_print_node(ctx->stack[i]);
	}
	printf("lookahead: %d, " SV_Fmt "\n", ctx->lookahead.type, SV_Arg(ctx->lookahead.text));
}

int lalr_reduce_tok_to_term(token tok, AST_Node* out_n) {
	out_n->type = NT_TOKEN;
	out_n->token = tok;
	return 1;
}

int lalr_reduce(lalr_ctx* ctx, AST_Node* out_n) {
	AST_Node peeked[3] = {lalr_peek_n(ctx, 0), lalr_peek_n(ctx, 1), lalr_peek_n(ctx, 2)};

	/**
	 *   factor parsing
	 *   	 factor := "(" <expression> ")"
	 *     factor := <id>
	 *     factor := <number>
	 *     factor := <strlit>
	 */
	{
	  // factor := "(" <expression> ")"
		if (peeked[2].type == NT_TOKEN && peeked[2].token.type == T_LP &&
				peeked[1].type == NT_EXPR &&
				peeked[0].type == NT_TOKEN && peeked[0].token.type == T_RP) {
			out_n->type = NT_FACTOR;
			out_n->factor.type = FACTOR_TYPE_EXPR;
			out_n->factor.expr = peeked[1].expr;
			return 3;
		}

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
	}

	/**
	 *   term parsing
	 *	   term := <term> "*" <factor>
	 *	   term := <term> "/" <factor>
	 *	   term := <term> "%" <factor>
	 *	   term := <factor>
	 */
	{
		// term := <factor>
		if (peeked[0].type == NT_FACTOR) {
			out_n->type = NT_TERM;
			out_n->term.type = TERM_TYPE_FACTOR;
			out_n->term.right = peeked[0].factor;
			out_n->term.left = NULL;
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
}
