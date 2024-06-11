#ifndef AST_ERROR_CHECKER_H
#define AST_ERROR_CHECKER_H
#include "ast.h"
#include "lib/cvector.h"

typedef struct error {
	int line, col, index;
} error;

typedef struct error_ctx {
	cvector_vector_type(error) errors;
} error_ctx;

void error_check_lalr(AST_Node, error_ctx*);

#endif
