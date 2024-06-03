#include "ast_util.h"
#include "ast.h"
#include "tokenizer.h"
#include <stdio.h>

#define TREE_FMT "%*c"
#define TREE_ARG(offset) (d + (offset)) * 2, ' '

void ast_print_node(AST_Node n, int d) {
	switch (n.type) {
		case NT_UNDEF:           printf("UNDEFINED\n"); break;
		case NT_TOKEN:           printf(TREE_FMT "[Token] %s\n", TREE_ARG(0), token_str(n.token.type)); break;
		case NT_FACTOR:          ast_print_factor(n.factor, d); break;
		case NT_NUMBER:          ast_print_number(n.number, d); break;
		case NT_EXPRESSION:      ast_print_expr(n.expr, d); break;
		case NT_LOGIC_DISJ:      ast_print_logical_disj(n.logicdisj, d); break;
		case NT_LOGIC_CONJ:      ast_print_logical_conj(n.logicconj, d); break;
		case NT_RELATE:          ast_print_relational(n.relate, d); break;
		case NT_MATH_EXPR:       ast_print_math_expr(n.mathexpr, d); break;
		case NT_TERM: 			     ast_print_term(n.term, d); break;
		case NT_STATEMENT:       ast_print_stmt(n.stmt, d); break;
		case NT_EXPRESSION_LIST: ast_print_expr_list(n.exprList, d); break;
		case NT_FUNC_CALL:       ast_print_func_call(n.funcCall, d); break;
		case NT_HEADER: 				 ast_print_header(n.header, d); break;
		case NT_TYPED_ID:				 ast_print_typed_id(n.typed_id, d); break;
		case NT_DEREF:           ast_print_deref(n.deref, d); break;
		case NT_VAR_TYPE:        ast_print_vartype(n.var_type, d); break;
		case NT_TYPED_ID_LIST:   ast_print_typed_idlist(n.typed_idlist, d); break;
		case NT_FUNC_HEADER:     ast_print_function_header(n.funcHeader, d); break;
		case NT_BLOCK:           ast_print_block(n.block, d); break;
		case NT_IF: 						 ast_print_if(n.iff, d); break;
		case NT_STATEMENT_LIST:  ast_print_stmt_list(n.stmtList, d); break;
		case NT_FUNCTION:        ast_print_function(n.function, d); break;
		case NT_ASSIGNMENT:      ast_print_assignment(n.assign, d); break;
	}
}

void ast_print_header(Header h, int d) {
	printf(TREE_FMT "[Header]\n", TREE_ARG(0));
	switch (h.type) {
		case HEADER_TYPE_MODULE:
			printf(TREE_FMT "[Module " SV_Fmt "]", TREE_ARG(1), SV_Arg(h.module.name));
			break;
		case HEADER_TYPE_USE:
			assert(0 && "Header Use not implemented");
			break;
	}
}

void ast_print_factor(Factor f, int d) {
	printf(TREE_FMT "[Factor]\n", TREE_ARG(0));
	// printf("%*c[Factor]\n", d * 2, ' ');
	switch (f.type) {
		case FACTOR_TYPE_UND:       printf(TREE_FMT "[UNDEFINED]\n", TREE_ARG(1));                      break;
		case FACTOR_TYPE_ID:        printf(TREE_FMT "[Id] " SV_Fmt "\n", TREE_ARG(1), SV_Arg(f.id));    break;
		case FACTOR_TYPE_STR:       printf(TREE_FMT "[Str]  " SV_Fmt "\n", TREE_ARG(1), SV_Arg(f.str)); break;
		case FACTOR_TYPE_NUMBER:    ast_print_number(f.number, d + 1);  																break;
		case FACTOR_TYPE_EXPR:      ast_print_expr(f.expr, d + 1); 																			break; 
		case FACTOR_TYPE_LOGIC_DISJ:assert(0 && "No factor for logic disjunction");											break;
		case FACTOR_TYPE_LOGIC_CONJ:assert(0 && "No factor for logic conjunction"); break;
		case FACTOR_TYPE_FUNC_CALL: ast_print_func_call(f.funcCall, d + 1); break;
		case FACTOR_TYPE_DEREF: ast_print_deref(f.deref, d + 1); break;
	}
}

void ast_print_vartype(VarType type, int d) {
	printf(TREE_FMT "[VarType]\n", TREE_ARG(0));
	switch (type.type) {
		case VAR_TYPE_ID:
			printf(TREE_FMT "[Id " SV_Fmt "]\n", TREE_ARG(1), SV_Arg(type.Id.id));
			break;
		case VAR_TYPE_ARRAY:
			printf(TREE_FMT "[Id " SV_Fmt "]\n", TREE_ARG(1), SV_Arg(type.Array.id));
			ast_print_expr_list(type.Array.exprList, d + 1);
			break;
		case VAR_TYPE_NONE: 
			printf(TREE_FMT "None\n", TREE_ARG(0));
			break;
	}
}

void ast_print_typed_id(TypedId id, int d) {
	printf(TREE_FMT "[TypedId]\n", TREE_ARG(0));
	printf(TREE_FMT "[Id " SV_Fmt "]\n", TREE_ARG(1), SV_Arg(id.id));
	ast_print_vartype(id.type, d + 1);
	// printf(TREE_FMT "[Type " SV_Fmt "]\n", TREE_ARG(1), SV_Arg(id.type));
}

void ast_print_typed_idlist_rec(TypedIdList* list, int i, int d) {
	if (list == NULL) {
		printf(TREE_FMT "[TypedIdList(NULL)]\n", TREE_ARG(0));
		return;
	}

	ast_print_typed_id(list->typedId, d + 1);
	ast_print_typed_idlist_rec(list->next, i + 1, d);
}

void ast_print_typed_idlist(TypedIdList* list, int d) {
	printf(TREE_FMT "[TypedIdList]\n", TREE_ARG(0));
	ast_print_typed_idlist_rec(list, 0, d);
}

void ast_print_deref(Deref deref, int d) {
	printf(TREE_FMT "[Deref %d]\n", TREE_ARG(0), deref.type);
	switch (deref.type) {
		case DEREF_TYPE_BRKT:
			printf(TREE_FMT "[Id " SV_Fmt "]\n", TREE_ARG(1), SV_Arg(deref.Brkt.id));
			ast_print_expr_list(deref.Brkt.exprList, d + 1);
			break;
		case DEREF_TYPE_ASTERISK: assert(0 && "Not printable yet"); break;
	}
}

void ast_print_number(Number n, int d) {
	printf(TREE_FMT "[Number]", TREE_ARG(0));
	switch (n.type) {
		case NUMBER_TYPE_INT:    printf("%d\n", n.i); break;
		case NUMBER_TYPE_DOUBLE: printf("%0.4f\n", n.d); break;
		case NUMBER_TYPE_FLOAT:  printf("%0.4f\n", n.f); break;
	}
}
void ast_print_logical_disj(LogicalDisj* l, int d) {
	if (l == NULL)
		return;
	ast_print_logical_disj(l->disj, d + 1);
	printf(TREE_FMT "[LogicalDisj]\n", TREE_ARG(0));
	ast_print_logical_conj(l->conj, d + 1);
}
void ast_print_logical_conj(LogicalConj* l, int d) {
	if (l == NULL)
		return;
	switch (l->type) {
		case LOGICAL_CONJ_TYPE_CONJ_RELATE:
			printf(TREE_FMT "[LogicalConj &&]\n", TREE_ARG(0));
			ast_print_logical_conj(l->conj, d + 1);
			ast_print_relational(l->relate, d + 1);
			break;
		case LOGICAL_CONJ_TYPE_RELATE:
			printf(TREE_FMT "[LogicalConj Relate]\n", TREE_ARG(0));
			ast_print_relational(l->relate, d + 1);
			break;
	}
}
void ast_print_relational(Relational* r, int d) {
	if (r == NULL)
		return;
	switch (r->type) {
		case RELATIONAL_TYPE_MATH_EXPR:
			printf(TREE_FMT "[Relational MathExpr]\n", TREE_ARG(0));
			ast_print_math_expr(r->mathexpr, d + 1);
			break;
		case RELATIONAL_TYPE_GT...RELATIONAL_TYPE_DEQ:
			printf(TREE_FMT "[Relational " SV_Fmt "]\n", TREE_ARG(0), SV_Arg(r->op));
			ast_print_relational(r->relate, d + 1);
			ast_print_math_expr(r->mathexpr, d + 1);
			break;
	}
}
void ast_print_expr(Expression* e, int d) {
	printf(TREE_FMT "[Expression]\n", TREE_ARG(0));
	switch (e->type) {
		case EXPRESSION_TYPE_LOGIC_DISJ:
			ast_print_logical_disj(e->disj, d + 1);
			break;
	}
}
void ast_print_math_expr(MathExpression* m, int d) {
	if (!m) return;
	printf(TREE_FMT "[MathExpression %c]\n", TREE_ARG(0), m ? m->op : '_');
	ast_print_math_expr(m->left, d + 1);
	ast_print_term(m->right, d + 1);
}
void ast_print_term(Term* t, int d) {
	printf(TREE_FMT "[Term %c]\n", TREE_ARG(0), t->op);
	if (t == NULL) {
		printf("Null\n");
		return;
	}
	switch (t->type) {
		case TERM_TYPE_FACTOR: 
			ast_print_factor(t->right, d + 1);
			break;
		case TERM_TYPE_TERM_OP_FACTOR: 
			ast_print_term(t->left, d + 1);
			ast_print_factor(t->right, d + 1);
			break;
	}
}

void ast_print_block(Block b, int d) {
	printf(TREE_FMT "[Block]\n", TREE_ARG(0));
	ast_print_stmt_list(b.stmts, d + 1);
}

void ast_print_stmt_list_rec(StatementList* stmt, int d, int i) {
	if (stmt == NULL) {
		printf(TREE_FMT "StatmentList(NULL)\n", TREE_ARG(0));
		return;
	}

	ast_print_stmt(stmt->stmt, d + 1);
	ast_print_stmt_list_rec(stmt->next, d, i + 1);
}

void ast_print_stmt_list(StatementList* stmts, int d) {
	printf(TREE_FMT "[StatementList]\n", TREE_ARG(0));
	ast_print_stmt_list_rec(stmts, d, 0);
}

void ast_print_stmt(Statement stmt, int d) {
	printf(TREE_FMT "[Statement]\n", TREE_ARG(0));
	switch (stmt.type) {
		case STATEMENT_TYPE_RETURN:
			printf(TREE_FMT "[Return]\n", TREE_ARG(1));
			ast_print_expr(stmt.Return.expr, d + 2);
			break;
		case STATEMENT_TYPE_IF:
			ast_print_if(stmt.iff, d + 1);
			break;
		case STATEMENT_TYPE_ASSIGN:
			ast_print_assignment(stmt.assign, d + 1);
			break;
	}
}

void ast_print_if(IfStatement iff, int d) {
	printf(TREE_FMT "[If]\n", TREE_ARG(0));
	ast_print_expr(iff.expr, d + 1);
	ast_print_block(iff.block, d + 1);
}

void ast_print_assignment(AssignStatement assign, int d) {
	printf(TREE_FMT "[Assign]\n", TREE_ARG(0));
	switch (assign.type) {
		case ASSIGN_TYPE_UNTYPED_ID:
			printf(TREE_FMT "[UntypedId " SV_Fmt "]\n", TREE_ARG(1), SV_Arg(assign.untypedId));
			ast_print_expr(assign.expr, d + 1);
			break;
		case ASSIGN_TYPE_TYPED_ID:
			ast_print_typed_id(assign.typedId, d + 1);
			ast_print_expr(assign.expr, d + 1);
			break;
	}
}

void ast_print_expr_list_rec(ExpressionList* eList, int d, int index) {
	if (eList == NULL) {
		printf(TREE_FMT "ExpressionList(NULL)\n", TREE_ARG(0));
		return;
	}

	// printf("%d:", index);
	ast_print_expr(eList->expr, d + 1);
	ast_print_expr_list_rec(eList->next, d, index + 1);
}

void ast_print_expr_list(ExpressionList * eList, int d) {
	printf(TREE_FMT "[ExpressionList]\n", TREE_ARG(0));
	ast_print_expr_list_rec(eList, d, 0);
}

void ast_print_func_call(FuncCall funcCall, int d) {
	printf(TREE_FMT "[FuncCall]\n", TREE_ARG(0));
	printf(TREE_FMT "[Id " SV_Fmt "]\n", TREE_ARG(1), SV_Arg(funcCall.id));
	ast_print_expr_list(funcCall.exprList, d + 1);
}

void ast_print_function_header(FunctionHeader header, int d) {
	printf(TREE_FMT "[FunctionHeader]\n", TREE_ARG(0));
	ast_print_typed_idlist(header.params, d + 1);
	ast_print_vartype(header.returnType, d + 1);
}

void ast_print_function(Function function, int d) {
	printf(TREE_FMT "[Function]\n", TREE_ARG(0));
	printf(TREE_FMT "[Id " SV_Fmt "]\n", TREE_ARG(1), SV_Arg(function.id));
	ast_print_function_header(function.header, d + 1);
	ast_print_block(function.block, d + 1);
}
