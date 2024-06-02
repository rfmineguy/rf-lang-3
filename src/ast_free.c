#include "ast_free.h"
#include "malloc_trace.h"
#include "ast.h"
#include <stdio.h>

static int depth = 0;
#if 0
#define FREE_BEGIN printf("%*c%s\n", depth * 2, ' ', __FUNCTION__); depth++
#define FREE_NOTIF printf("%*c%s\n", (depth - 1) * 2, ' ', __FUNCTION__); depth--
#else
#define FREE_BEGIN {}
#define FREE_NOTIF {}
#endif
void ast_free_node(AST_Node n){
	FREE_BEGIN;
	switch (n.type) {
		case NT_UNDEF:      break;
		case NT_TOKEN:      break;
		case NT_FACTOR:     ast_free_factor(n.factor); break;
		case NT_NUMBER:     ast_free_number(n.number); break;
		case NT_EXPRESSION: ast_free_expr(n.expr); break;
		case NT_LOGIC_DISJ: ast_free_logical_disj(n.logicdisj); break;
		case NT_LOGIC_CONJ: ast_free_logical_conj(n.logicconj); break;
		case NT_RELATE:     ast_free_relational(n.relate); break;
		case NT_MATH_EXPR:  ast_free_math_expr(n.mathexpr); break;
		case NT_TERM: 			ast_free_term(n.term); break;
		case NT_STATEMENT:  ast_free_stmt(n.stmt); break;
	}
	FREE_NOTIF;
}
void ast_free_factor(Factor f){
	FREE_BEGIN;
	switch (f.type) {
		case FACTOR_TYPE_UND:       break;
		case FACTOR_TYPE_ID:        break;
		case FACTOR_TYPE_STR:       break;
		case FACTOR_TYPE_NUMBER:    ast_free_number(f.number);  					break;
		case FACTOR_TYPE_LOGIC_CONJ:break; //ast_free_logical_conj(f.logical_test);break;
		case FACTOR_TYPE_LOGIC_DISJ:ast_free_logical_disj(f.logicdisj);break;
		case FACTOR_TYPE_EXPR:      ast_free_expr(f.expr); 								break; 
	}
	FREE_NOTIF;
}
void ast_free_number(Number n){
	FREE_BEGIN;
	// nothing to free
	FREE_NOTIF;
}
void ast_free_logical_disj(LogicalDisj* disj){
	FREE_BEGIN;
	if (disj == NULL) return;
	if (disj->conj) ast_free_logical_conj(disj->conj);
	if (disj->disj) ast_free_logical_disj(disj->disj);
	free(disj);
	FREE_NOTIF;
}
void ast_free_logical_conj(LogicalConj* conj){
	FREE_BEGIN;
	if (conj == NULL) return;
	switch (conj->type) {
		case LOGICAL_CONJ_TYPE_RELATE:
			ast_free_relational(conj->relate);
			break;
		case LOGICAL_CONJ_TYPE_CONJ_RELATE:
			ast_free_logical_conj(conj->conj);
			ast_free_relational(conj->relate);
			break;
	}
	free(conj);
	FREE_NOTIF;
}
void ast_free_relational(Relational* relate){
	FREE_BEGIN;
	if (relate == NULL) return;
	switch (relate->type) {
		case RELATIONAL_TYPE_MATH_EXPR:
			ast_free_math_expr(relate->mathexpr);
			break;
		case RELATIONAL_TYPE_GT...RELATIONAL_TYPE_DEQ:
			ast_free_math_expr(relate->mathexpr);
			ast_free_relational(relate->relate);
			break;
	}
	free(relate);
	FREE_NOTIF;
}
void ast_free_expr(Expression* expr){
	FREE_BEGIN;
	if (expr == NULL) return;
	ast_free_logical_disj(expr->disj);
	free(expr);
	FREE_NOTIF;
}
void ast_free_math_expr(MathExpression* mexpr){
	FREE_BEGIN;
	if (mexpr == NULL) return;
	ast_free_term(mexpr->right);
	ast_free_math_expr(mexpr->left);
	free(mexpr);
	FREE_NOTIF;
}
void ast_free_term(Term* term){
	FREE_BEGIN;
	switch (term->type) {
		case TERM_TYPE_FACTOR: ast_free_factor(term->right); break;
		case TERM_TYPE_TERM_OP_FACTOR: ast_free_term(term->left);
																	 ast_free_factor(term->right); break;
	}
	free(term);
	FREE_NOTIF;
}
void ast_free_stmt(Statement* stmt) {
	FREE_BEGIN;
	switch (stmt->type) {
		case STATEMENT_TYPE_RETURN: 
			ast_free_expr(stmt->Return.expr);
			break;
	}
	free(stmt);
	FREE_NOTIF;
}
