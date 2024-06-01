#ifndef AST_FREE_H
#define AST_FREE_H
#include "ast.h"

void ast_free_node(AST_Node);
void ast_free_factor(Factor);
void ast_free_number(Number);
void ast_free_logical_disj(LogicalDisj*);
void ast_free_logical_conj(LogicalConj*);
void ast_free_relational(Relational*);
void ast_free_expr(Expression*);
void ast_free_math_expr(MathExpression*);
void ast_free_term(Term*);
void ast_free_stmt(Statement*);

#endif
