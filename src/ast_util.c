#include "ast_util.h"
#include "ast.h"
#include "tokenizer.h"
#include <stdio.h>

#define TREE_FMT "%*c"
#define TREE_ARG(offset) (d + (offset)) * 2, ' '

void ast_print_node(AST_Node n, int d) {
	switch (n.type) {
		case NT_UNDEF:      printf("UNDEFINED\n"); break;
		case NT_TOKEN:      printf(TREE_FMT "[Token] %s\n", TREE_ARG(0), token_str(n.token.type)); break;
		case NT_FACTOR:     ast_print_factor(n.factor, d); break;
		case NT_NUMBER:     ast_print_number(n.number, d); break;
		case NT_EXPR:       ast_print_expr(n.expr, d); break;
		case NT_LOGIC_DISJ: ast_print_logical_disj(&n.logicdisj, d); break;
		case NT_LOGIC_CONJ: ast_print_logical_conj(&n.logicconj, d); break;
		case NT_RELATE:     ast_print_relational(&n.relate, d); break;
		case NT_MATH_EXPR:  ast_print_math_expr(n.mathexpr, d); break;
		case NT_TERM: 			ast_print_term(&n.term, d); break;
	}
}

void ast_print_factor(Factor f, int d) {
	printf(TREE_FMT "[Factor]\n", TREE_ARG(0));
	// printf("%*c[Factor]\n", d * 2, ' ');
	switch (f.type) {
		case FACTOR_TYPE_ID:     printf(TREE_FMT "[Id] " SV_Fmt "\n", TREE_ARG(1), SV_Arg(f.id));  break;
		case FACTOR_TYPE_STR:    printf("[Str]  " SV_Fmt "\n", SV_Arg(f.str)); break;
		case FACTOR_TYPE_NUMBER: ast_print_number(f.number, d + 1);  break;
		case FACTOR_TYPE_EXPR:   ast_print_expr(f.expr, d + 1);      break; 
	}
}
void ast_print_number(Number n, int d) {
	printf(TREE_FMT "[Number]", TREE_ARG(0));
	// printf("%*c[Number]", d * 2, ' ');
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
	printf(TREE_FMT "[LogicalDisj]", TREE_ARG(0));
	// printf("%*c[LogicalDisj]", d * 2, ' ');
	ast_print_logical_conj(&l->conj, d + 1);
}
void ast_print_logical_conj(LogicalConj* l, int d) {
	if (l == NULL)
		return;
	ast_print_logical_conj(l->conj, d + 1);
	printf(TREE_FMT "[LogicalConj]", TREE_ARG(0));
	// printf("%*c[LogicalConj]", d * 2, ' ');
	ast_print_relational(&l->relate, d + 1);

}
void ast_print_relational(Relational* r, int d) {
	if (r == NULL)
		return;
	printf(TREE_FMT "[Relational]", TREE_ARG(0));
	// printf("%*c[Relational]", d * 2, ' ');
	ast_print_relational(r->relate, d + 1);
	ast_print_math_expr(r->mathexpr, d + 1);
}
void ast_print_expr(Expression* e, int d) {
	printf(TREE_FMT "[Expression]", TREE_ARG(0));
	// printf("%*c[Expression]", d * 2, ' ');
	ast_print_logical_disj(e->disj, d + 1);
}
void ast_print_math_expr(MathExpression* m, int d) {
	printf(TREE_FMT "[MathExpression]", TREE_ARG(0));
	// printf("%*c[MathExpression]", d * 2, ' ');
	ast_print_math_expr(m->left, d + 1);
	ast_print_term(&m->right, d + 1);
}
void ast_print_term(Term* t, int d) {
	printf(TREE_FMT "[Term]\n", TREE_ARG(0));
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
