#ifndef AST_UTIL_H
#define AST_UTIL_H

#include "ast.h"
typedef struct Operands {
	AST_Node left, right;
} Operands;

Operands ast_util_get_expr_operands(Expression);
Operands ast_util_get_logic_disj_operands(LogicalDisj);
Operands ast_util_get_logic_conj_operands(LogicalConj);

/**
 * AST Reconstruction
 *    Purpose: Rebuilds the original source from the parse tree
 *    Input:   The ast node you wish to start reconstruction from
 */
void ast_util_reconstruct_ast_node(AST_Node);
void ast_util_reconstruct_header(Header);
void ast_util_reconstruct_factor(Factor);
void ast_util_reconstruct_number(Number);
void ast_util_reconstruct_vartype(VarType);
void ast_util_reconstruct_typed_id(TypedId);
void ast_util_reconstruct_deref(Deref);
void ast_util_reconstruct_logical_disj(LogicalDisj*);
void ast_util_reconstruct_logical_conj(LogicalConj*);
void ast_util_reconstruct_relational(Relational*);
void ast_util_reconstruct_expr(Expression*);
void ast_util_reconstruct_math_expr(MathExpression*);
void ast_util_reconstruct_term(Term*);
void ast_util_reconstruct_block(Block);
void ast_util_reconstruct_stmt(Statement);
void ast_util_reconstruct_if(IfStatement);
void ast_util_reconstruct_assignment(AssignStatement);
void ast_util_reconstruct_stmt_list(StatementList*);
void ast_util_reconstruct_expr_list(ExpressionList*);
void ast_util_reconstruct_func_call(FuncCall);
void ast_util_reconstruct_typed_idlist(TypedIdList*);
void ast_util_reconstruct_function_header(FunctionHeader);
void ast_util_reconstruct_function(Function);

#endif
