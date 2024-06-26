#ifndef CODEGEN_X8632
#define CODEGEN_X8632
#include "ast.h"
#include "lalr.h"
#include <stdio.h>

typedef struct {
	FILE* file;
} codegen_x86_32_ctx;

void codegen_entry_x86_32(lalr_ctx*, FILE*);

void codegen_header_x86_32 	  (codegen_x86_32_ctx*, Header);
void codegen_assign_x86_32 	  (codegen_x86_32_ctx*, AssignStatement);
void codegen_expr_x86_32 		  (codegen_x86_32_ctx*, Expression*);
void codegen_term_x86_32      (codegen_x86_32_ctx*, Term);
void codegen_function_x86_32  (codegen_x86_32_ctx*, Function);
void codegen_block_x86_32     (codegen_x86_32_ctx*, Block);
void codegen_stmt_x86_32 		  (codegen_x86_32_ctx*, Statement);
void codegen_stmt_list_x86_32	(codegen_x86_32_ctx*, StatementList*);
void codegen_if_stmt_x86_32   (codegen_x86_32_ctx*, IfStatement);

#endif
