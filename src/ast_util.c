#include "ast_util.h"
#include "ast.h"
#include <stdio.h>
#define INTERNAL

Operands ast_util_get_expr_operands(Expression e) {
	Operands op = {0};
	switch (e.type) {
		case EXPRESSION_TYPE_LOGIC_DISJ:
			return ast_util_get_logic_disj_operands(*e.disj);
	}
	return (Operands) {};
}

Operands ast_util_get_logic_disj_operands(LogicalDisj disj) {
	switch (disj.type) {
		case LOGICAL_DISJ_TYPE_DISJ_CONJ: {
			break;
		}
		case LOGICAL_DISJ_TYPE_CONJ: {
			Operands op = ast_util_get_logic_conj_operands(*disj.conj);
			return op;
		}
	}
	return (Operands) {};
}

Operands ast_util_get_logic_conj_operands(LogicalConj conj) {
	return (Operands) {};
}


void ast_util_reconstruct_ast_node(AST_Node n) {
	switch (n.type) {
		case NT_UNDEF:           assert(0 && "Can't reconstruct UNDEFINED"); break;
		case NT_TOKEN:           assert(0 && "No individual token generation"); break;
		case NT_FACTOR:          ast_util_reconstruct_factor(n.factor); break;
		case NT_TERM: 			     ast_util_reconstruct_term(n.term); break;
		case NT_NUMBER:          ast_util_reconstruct_number(n.number); break;
		case NT_EXPRESSION:      ast_util_reconstruct_expr(n.expr); break;
		case NT_LOGIC_DISJ:      ast_util_reconstruct_logical_disj(n.logicdisj); break;
		case NT_LOGIC_CONJ:      ast_util_reconstruct_logical_conj(n.logicconj); break;
		case NT_RELATE:          ast_util_reconstruct_relational(n.relate); break;
		case NT_MATH_EXPR:       ast_util_reconstruct_math_expr(n.mathexpr); break;
		case NT_STATEMENT:       ast_util_reconstruct_stmt(n.stmt); break;
		case NT_EXPRESSION_LIST: ast_util_reconstruct_expr_list(n.exprList); break;
		case NT_FUNC_CALL:       ast_util_reconstruct_func_call(n.funcCall); break;
		case NT_HEADER: 				 ast_util_reconstruct_header(n.header); break;
		case NT_TYPED_ID:				 ast_util_reconstruct_typed_id(n.typed_id); break;
		case NT_DEREF:           ast_util_reconstruct_deref(n.deref); break;
		case NT_VAR_TYPE:        ast_util_reconstruct_vartype(n.var_type); break;
		case NT_TYPED_ID_LIST:   ast_util_reconstruct_typed_idlist(n.typed_idlist); break;
		case NT_FUNC_HEADER:     ast_util_reconstruct_function_header(n.funcHeader); break;
		case NT_BLOCK:           ast_util_reconstruct_block(n.block); break;
		case NT_IF: 						 ast_util_reconstruct_if(n.iff); break;
		case NT_STATEMENT_LIST:  ast_util_reconstruct_stmt_list(n.stmtList); break;
		case NT_FUNCTION:        ast_util_reconstruct_function(n.function); break;
		case NT_ASSIGNMENT:      ast_util_reconstruct_assignment(n.assign); break;
	}

}

void ast_util_reconstruct_header(Header header){

}
void ast_util_reconstruct_factor(Factor factor){

}
void ast_util_reconstruct_number(Number number){

}
void ast_util_reconstruct_vartype(VarType var_type){

}
void ast_util_reconstruct_typed_id(TypedId typed_id){

}
void ast_util_reconstruct_deref(Deref deref){

}
void ast_util_reconstruct_logical_disj(LogicalDisj* disj){
	printf("(");
	switch (disj->type) {
		case LOGICAL_DISJ_TYPE_CONJ:
			ast_util_reconstruct_logical_conj(disj->conj);
			break;
		case LOGICAL_DISJ_TYPE_DISJ_CONJ:
			ast_util_reconstruct_logical_disj(disj->disj);
			printf("||");
			ast_util_reconstruct_logical_conj(disj->conj);
			break;
	}
	printf(")");

}
void ast_util_reconstruct_logical_conj(LogicalConj* conj){
	printf("(");
	switch (conj->type) {
		case LOGICAL_CONJ_TYPE_RELATE:
			ast_util_reconstruct_relational(conj->relate);
			break;
		case LOGICAL_CONJ_TYPE_CONJ_RELATE:
			ast_util_reconstruct_logical_conj(conj->conj);
			printf("&&");
			ast_util_reconstruct_relational(conj->relate);
			break;
	}
	printf(")");

}
void ast_util_reconstruct_relational(Relational* relate){
	printf("(");
	switch (relate->type) {
		case RELATIONAL_TYPE_MATH_EXPR:
			ast_util_reconstruct_math_expr(relate->mathexpr);
			break;
		case RELATIONAL_TYPE_GT...RELATIONAL_TYPE_DEQ:
			ast_util_reconstruct_relational(relate->relate);
			printf(SV_Fmt, SV_Arg(relate->op));
			break;
	}
	printf(")");
}
void ast_util_reconstruct_expr(Expression* expr){
	printf("(");
	switch (expr->type) {
		case EXPRESSION_TYPE_LOGIC_DISJ:
			ast_util_reconstruct_logical_disj(expr->disj);
			break;
	}
	printf(")");
}
void ast_util_reconstruct_math_expr(MathExpression* mexpr){
	printf("(");
	switch (mexpr->type) {
		case MATH_EXPR_TYPE_ADD:
			ast_util_reconstruct_math_expr(mexpr->left);
			printf("%c", mexpr->op);
			ast_util_reconstruct_term(mexpr->right);
			break;
		case MATH_EXPR_TYPE_SUB:
			ast_util_reconstruct_math_expr(mexpr->left);
			printf("%c", mexpr->op);
			ast_util_reconstruct_term(mexpr->right);
			break;
		case MATH_EXPR_TYPE_TERM:
			ast_util_reconstruct_math_expr(mexpr->left);
			break;
	}
	printf(")");
}
void ast_util_reconstruct_term(Term* term){
	printf("t");
}
void ast_util_reconstruct_block(Block block){

}
void ast_util_reconstruct_stmt(Statement stmt){

}
void ast_util_reconstruct_if(IfStatement if_stmt){

}
void ast_util_reconstruct_assignment(AssignStatement assign_stmt){
	switch (assign_stmt.type) {
		case ASSIGN_TYPE_TYPED_ID:
			ast_util_reconstruct_typed_id(assign_stmt.typedId);
			break;
		case ASSIGN_TYPE_UNTYPED_ID:
			printf(SV_Fmt, SV_Arg(assign_stmt.untypedId));
			break;
	}
	printf(" = ");
	ast_util_reconstruct_expr(assign_stmt.expr);
}
void ast_util_reconstruct_stmt_list(StatementList* stmt_list){

}
void ast_util_reconstruct_expr_list(ExpressionList* expr_list){

}
void ast_util_reconstruct_func_call(FuncCall func_call){

}
void ast_util_reconstruct_typed_idlist(TypedIdList* typed_id_list){

}
void ast_util_reconstruct_function_header(FunctionHeader func_header){

}
void ast_util_reconstruct_function(Function function){

}
