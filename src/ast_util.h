#ifndef AST_UTIL_H
#define AST_UTIL_H

#include "ast.h"
typedef struct Operands {
	AST_Node left, right;
} Operands;

Operands ast_util_get_expr_operands(Expression);
Operands ast_util_get_logic_disj_operands(LogicalDisj);
Operands ast_util_get_logic_conj_operands(LogicalConj);

#endif
