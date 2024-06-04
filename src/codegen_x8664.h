#ifndef CODEGEN_X8664
#define CODEGEN_X8664
#include "ast.h"
#include "lalr.h"
#include <stdio.h>

typedef struct {
	FILE* f;
} codegen_x86_64_ctx;

void codegen_entry_x86_64(lalr_ctx*, FILE*);

void codegen_header_x86_64 	(codegen_x86_64_ctx*, Header);
void codegen_assign_x86_64 	(codegen_x86_64_ctx*, AssignStatement);
void codegen_expr_x86_64 		(codegen_x86_64_ctx*, Expression*);
void codegen_stmt_x86_64 		(codegen_x86_64_ctx*, Statement);
void codegen_if_stmt_x86_64 (codegen_x86_64_ctx*, IfStatement);
void codegen_function_x86_64(codegen_x86_64_ctx*, Function);

#undef ARCH

#endif
