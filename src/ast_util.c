#include "ast_util.h"
#include "ast.h"
#define INTERNAL

Operands ast_util_get_expr_operands(Expression e) {
	Operands op = {0};
	switch (e.type) {
		case EXPRESSION_TYPE_LOGIC_DISJ:
			return ast_util_get_logic_disj_operands(*e.disj);
	}
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
}

Operands ast_util_get_logic_conj_operands(LogicalConj conj) {
	
}
