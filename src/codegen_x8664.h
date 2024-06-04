#ifndef CODEGEN_X8664
#define CODEGEN_X8664
#include "ast.h"
#include <stdio.h>

#define ARCH x86_64
#include "codegen_tplt.h"

typedef struct {
	FILE* f;
} x86_64_codegen_ctx;

void codegen_entry_point(ARCH,,FILE*);

void codegen_header(ARCH,,x86_64_codegen_ctx*);
void codegen_assign(ARCH,,x86_64_codegen_ctx*);
void codegen_expr(ARCH,,x86_64_codegen_ctx*);
void codegen_stmt(ARCH,,x86_64_codegen_ctx*);
void codegen_if_stmt(ARCH,,x86_64_codegen_ctx*);
void codegen_function(ARCH,,x86_64_codegen_ctx*);

#undef ARCH

#endif
