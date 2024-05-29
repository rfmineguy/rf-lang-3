#include "ast_free.h"

void ast_free_node(AST_Node n){
	switch (n.type) {
		case NT_UNDEF:      break;
		case NT_TOKEN:      break;
		case NT_FACTOR:     ast_free_factor(n.factor); break;
		case NT_NUMBER:     ast_free_number(n.number); break;
		case NT_EXPR:       ast_free_expr(n.expr); break;
		case NT_LOGIC_DISJ: ast_free_logical_disj(n.logicdisj); break;
		case NT_LOGIC_CONJ: ast_free_logical_conj(n.logicconj); break;
		case NT_RELATE:     ast_free_relational(n.relate); break;
		case NT_MATH_EXPR:  ast_free_math_expr(n.mathexpr); break;
		case NT_TERM: 			ast_free_term(n.term); break;
	}

}
void ast_free_factor(Factor f){
	switch (f.type) {
		case FACTOR_TYPE_UND:       break;
		case FACTOR_TYPE_ID:        break;
		case FACTOR_TYPE_STR:       break;
		case FACTOR_TYPE_NUMBER:    ast_free_number(f.number);  					break;
		case FACTOR_TYPE_LOGIC_CONJ:ast_free_logical_conj(f.logical_test);break;
		case FACTOR_TYPE_EXPR:      ast_free_expr(f.expr); 								break; 
	}
}
void ast_free_number(Number n){
	// nothing to free
}
void ast_free_logical_disj(LogicalDisj* disj){
	ast_free_logical_conj(disj->conj);
	free(disj);
}
void ast_free_logical_conj(LogicalConj* conj){
	ast_free_relational(conj->relate);
	free(conj);
}
void ast_free_relational(Relational* relate){
	if (relate == NULL) return;
	switch (relate->type) {
		case RELATIONAL_TYPE_MATH_EXPR:
			ast_free_math_expr(relate->mathexpr);
			break;
		case RELATIONAL_TYPE_GT...RELATIONAL_TYPE_DEQ:
			ast_free_relational(relate->relate);
			ast_free_math_expr(relate->mathexpr);
			break;
	}
	free(relate);
}
void ast_free_expr(Expression* expr){

}
void ast_free_math_expr(MathExpression* mexpr){
	if (mexpr == NULL) return;
	ast_free_term(mexpr->right);
	ast_free_math_expr(mexpr->left);
	free(mexpr);
}
void ast_free_term(Term* term){
	switch (term->type) {
		case TERM_TYPE_FACTOR: ast_free_factor(term->right); break;
		case TERM_TYPE_TERM_OP_FACTOR: ast_free_term(term->left);
																	 ast_free_factor(term->right); break;
	}
	free(term);
}
