#include "ast_reconstruct.h"
#include "ast.h"
#include "lib/sv.h"
#include <assert.h>
#include <stdio.h>

static int nest_depth = 1;
#define INDENT_FMT "%*c"
#define INDENT_ARG nest_depth * 2, ' '

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
	switch (header.type) {
		case HEADER_TYPE_MODULE: 
			printf(INDENT_FMT "module " SV_Fmt "\n", INDENT_ARG, SV_Arg(header.module.name));
			break;
		case HEADER_TYPE_USE:
			assert("Header type 'use' not implemented");
	}
}

void ast_util_reconstruct_factor(Factor factor){
	switch (factor.type) {
		case FACTOR_TYPE_ID: 
			printf(SV_Fmt, SV_Arg(factor.id));
			break;
		case FACTOR_TYPE_NUMBER:
			ast_util_reconstruct_number(factor.number);
			break;
		case FACTOR_TYPE_EXPR:
			ast_util_reconstruct_expr(factor.expr);
			break;
		case FACTOR_TYPE_FUNC_CALL:
			ast_util_reconstruct_func_call(factor.funcCall);
			break;
		case FACTOR_TYPE_DEREF:
			ast_util_reconstruct_deref(factor.deref);
			break;
		default: assert(0 && "Reconstruct factor unfinished");
	}
}

void ast_util_reconstruct_number(Number number){
	switch (number.type) {
		case NUMBER_TYPE_INT:
			printf("%d", number.i);
			break;
		case NUMBER_TYPE_FLOAT:
			printf("%.4f", number.f);
			break;
		case NUMBER_TYPE_DOUBLE:
			printf("%.8f", number.d);
			break;
	}
}

void ast_util_reconstruct_vartype(VarType* var_type){
	if (!var_type) {
		return;
	}
	switch (var_type->type) {
		case VAR_TYPE_ID:
			printf(SV_Fmt, SV_Arg(var_type->Id.id));
			for (int i = 0; i < var_type->pointerDepth; i++)	printf("*");
			break;
		case VAR_TYPE_NONE:
			printf("_");
			break;
		case VAR_TYPE_ARRAY:
			printf("[" SV_Fmt ";", SV_Arg(var_type->Array.id));
			ast_util_reconstruct_expr_list(var_type->Array.exprList);
			printf("]");
			break;
		case VAR_TYPE_ARRAY_NESTED:
			printf("[");
			ast_util_reconstruct_vartype(var_type->nested);
			printf(";");
			ast_util_reconstruct_expr_list(var_type->nested->Array.exprList);
			printf("]");
			break;
	}
}

void ast_util_reconstruct_typed_id(TypedId typed_id){
	printf(SV_Fmt ": ", SV_Arg(typed_id.id));
	ast_util_reconstruct_vartype(typed_id.type);
}

void ast_util_reconstruct_deref(Deref deref){
	switch (deref.type) {
		case DEREF_TYPE_BRKT:
			printf(SV_Fmt "[", SV_Arg(deref.Brkt.id));
			ast_util_reconstruct_expr_list(deref.Brkt.exprList);
			printf("]");
			break;
		default: assert(0 && "Deref type fully implemented");
	}
}

void ast_util_reconstruct_logical_disj(LogicalDisj* disj){
	if (!disj) {
		printf("_");
		return;
	}
	switch (disj->type) {
		case LOGICAL_DISJ_TYPE_CONJ:
			ast_util_reconstruct_logical_conj(disj->conj);
			break;
		case LOGICAL_DISJ_TYPE_DISJ_CONJ:
			printf("(");
			ast_util_reconstruct_logical_disj(disj->disj);
			printf("||");
			ast_util_reconstruct_logical_conj(disj->conj);
			printf(")");
			break;
	}
}

void ast_util_reconstruct_logical_conj(LogicalConj* conj){
	if (!conj) return;
	switch (conj->type) {
		case LOGICAL_CONJ_TYPE_RELATE:
			ast_util_reconstruct_relational(conj->relate);
			break;
		case LOGICAL_CONJ_TYPE_CONJ_RELATE:
			printf("(");
			ast_util_reconstruct_logical_conj(conj->conj);
			printf("&&");
			ast_util_reconstruct_relational(conj->relate);
			printf(")");
			break;
	}
}

void ast_util_reconstruct_relational(Relational* relate){
	if (!relate) return;
	switch (relate->type) {
		case RELATIONAL_TYPE_MATH_EXPR:
			ast_util_reconstruct_math_expr(relate->mathexpr);
			break;
		case RELATIONAL_TYPE_GT...RELATIONAL_TYPE_DEQ:
			printf("(");
			ast_util_reconstruct_relational(relate->relate);
			printf(SV_Fmt, SV_Arg(relate->op));
			ast_util_reconstruct_math_expr(relate->mathexpr);
			printf(")");
			break;
	}
}

void ast_util_reconstruct_expr(Expression* expr){
	if (!expr) return;
	switch (expr->type) {
		case EXPRESSION_TYPE_LOGIC_DISJ:
			ast_util_reconstruct_logical_disj(expr->disj);
			break;
	}
}

void ast_util_reconstruct_math_expr(MathExpression* mexpr){
	if (!mexpr) return;
	switch (mexpr->type) {
		case MATH_EXPR_TYPE_ADD:
			printf("(");
				ast_util_reconstruct_math_expr(mexpr->left);
				printf("%c", mexpr->op);
				ast_util_reconstruct_term(mexpr->right);
			printf(")");
			break;
		case MATH_EXPR_TYPE_SUB:
			printf("(");
				ast_util_reconstruct_math_expr(mexpr->left);
				printf("%c", mexpr->op);
				ast_util_reconstruct_term(mexpr->right);
			printf(")");
			break;
		case MATH_EXPR_TYPE_TERM:
			ast_util_reconstruct_term(mexpr->right);
			break;
	}
}

void ast_util_reconstruct_term(Term* term){
	switch (term->type) {
		case TERM_TYPE_FACTOR:
			ast_util_reconstruct_factor(term->right);
			break;
		case TERM_TYPE_TERM_OP_FACTOR:
			printf("(");
				ast_util_reconstruct_term(term->left);
				printf("%c", term->op);
				ast_util_reconstruct_factor(term->right);
			printf(")");
			break;
	}
}

void ast_util_reconstruct_block(Block block){
	printf(" {\n");
	nest_depth++;
	ast_util_reconstruct_stmt_list(block.stmts);
	nest_depth--;
	printf(INDENT_FMT "}\n", INDENT_ARG);
}

void ast_util_reconstruct_stmt(Statement stmt){
	switch (stmt.type) {
		case STATEMENT_TYPE_ASSIGN: 
			ast_util_reconstruct_assignment(stmt.assign);
			printf("\n");
			break;
		case STATEMENT_TYPE_IF: 
			ast_util_reconstruct_if(stmt.iff);
			printf("\n");
			break;
		case STATEMENT_TYPE_EXPR:
			printf(INDENT_FMT, INDENT_ARG);
			ast_util_reconstruct_expr(stmt.expr);
			printf("\n");
			break;
		case STATEMENT_TYPE_RETURN:
			printf(INDENT_FMT "return ", INDENT_ARG);
			ast_util_reconstruct_expr(stmt.Return.expr);
			printf("\n");
			break;
	}
}

void ast_util_reconstruct_if(IfStatement if_stmt){
	printf(INDENT_FMT "if ", INDENT_ARG);
	ast_util_reconstruct_expr(if_stmt.expr);
	ast_util_reconstruct_block(if_stmt.block);
}

void ast_util_reconstruct_assignment(AssignStatement assign_stmt){
	switch (assign_stmt.type) {
		case ASSIGN_TYPE_TYPED_ID:
			printf(INDENT_FMT, INDENT_ARG);
			ast_util_reconstruct_typed_id(assign_stmt.typedId);
			break;
		case ASSIGN_TYPE_UNTYPED_ID:
			printf(INDENT_FMT "" SV_Fmt, INDENT_ARG, SV_Arg(assign_stmt.untypedId));
			break;
	}
	printf(" = ");
	ast_util_reconstruct_expr(assign_stmt.expr);
}

void ast_util_reconstruct_stmt_list(StatementList* stmts) {
	StatementList* curr = stmts;
	while (curr) {
		ast_util_reconstruct_stmt(curr->stmt);
		curr = curr->next;
	}
}

void ast_util_reconstruct_expr_list(ExpressionList* expr_list){
	if (expr_list == NULL)
		printf("NULL");
	ExpressionList* curr = expr_list;
	while (curr) {
		ast_util_reconstruct_expr(curr->expr);
		curr = curr->next;
		if (curr)
			printf(",");
	}
}

void ast_util_reconstruct_func_call(FuncCall func_call){
	printf(SV_Fmt, SV_Arg(func_call.id));
	printf("(");
	ast_util_reconstruct_expr_list(func_call.exprList);
	printf(")");

}

void ast_util_reconstruct_typed_idlist(TypedIdList* typed_id_list){
	TypedIdList* curr = typed_id_list;
	while (curr) {
		ast_util_reconstruct_typed_id(curr->typedId);
		curr = curr->next;
		if (curr)
			printf(", ");
	}
}

void ast_util_reconstruct_function_header(FunctionHeader func_header){
	ast_util_reconstruct_typed_idlist(func_header.params);
	printf(" -> ");
	ast_util_reconstruct_vartype(func_header.returnType);
}

void ast_util_reconstruct_function(Function function){
	printf(INDENT_FMT SV_Fmt " = ", INDENT_ARG, SV_Arg(function.id));
	ast_util_reconstruct_function_header(function.header);
	ast_util_reconstruct_block(function.block);
}
