#ifndef CODEGEN_ARM64
#define CODEGEN_ARM64
#include "ast.h"
#include "lalr.h"
#include <stdio.h>

typedef struct {
	FILE* f;
} codegen_arm64_ctx;

void codegen_entry_arm64(lalr_ctx*, FILE*);

void codegen_header_arm64 	(codegen_arm64_ctx*, Header);
void codegen_assign_arm64 	(codegen_arm64_ctx*, AssignStatement);
void codegen_expr_arm64 		(codegen_arm64_ctx*, Expression*);
void codegen_stmt_arm64 		(codegen_arm64_ctx*, Statement);
void codegen_if_stmt_arm64 (codegen_arm64_ctx*, IfStatement);
void codegen_function_arm64(codegen_arm64_ctx*, Function);

#endif
