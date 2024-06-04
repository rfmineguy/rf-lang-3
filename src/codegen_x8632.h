#ifndef CODEGEN_X8632
#define CODEGEN_X8632
#include "ast.h"

#define ARCH x86_32
#include "codegen_tplt.h"

typedef struct {
} x86_32_codegen_ctx;

void codegen_entry_point(ARCH,);

void codegen_header(ARCH,,x86_32_codegen_ctx*);
void codegen_assign(ARCH,,x86_32_codegen_ctx*);
void codegen_expr(ARCH,,x86_32_codegen_ctx*);
void codegen_stmt(ARCH,,x86_32_codegen_ctx*);
void codegen_if_stmt(ARCH,,x86_32_codegen_ctx*);
void codegen_function(ARCH,,x86_32_codegen_ctx*);

#undef ARCH

#endif
