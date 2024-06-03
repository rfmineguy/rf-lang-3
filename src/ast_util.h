#ifndef AST_UTIL_H
#define AST_UTIL_H
#include "ast.h"

void ast_print_node(AST_Node, int);
void ast_print_header(Header, int);
void ast_print_factor(Factor, int);
void ast_print_number(Number, int);
void ast_print_vartype(VarType, int);
void ast_print_typed_id(TypedId, int);
void ast_print_deref(Deref, int);
void ast_print_logical_disj(LogicalDisj*, int);
void ast_print_logical_conj(LogicalConj*, int);
void ast_print_relational(Relational*, int);
void ast_print_expr(Expression*, int);
void ast_print_math_expr(MathExpression*, int);
void ast_print_term(Term*, int);
void ast_print_stmt(Statement*, int);
void ast_print_expr_list(ExpressionList*, int);
void ast_print_func_call(FuncCall, int);
void ast_print_typed_idlist(TypedIdList*, int);
void ast_print_function_header(FunctionHeader, int);

#endif
