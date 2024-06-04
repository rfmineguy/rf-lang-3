#ifndef CODEGEN_ARM64
#define CODEGEN_ARM64
#include "ast.h"

#define ARCH arm64
#include "codegen_tplt.h"

typedef struct {
} arm64_codegen_ctx;

void codegen_entry_point(ARCH,);

void codegen_header(ARCH,,arm64_codegen_ctx*);
void codegen_assign(ARCH,,arm64_codegen_ctx*);
void codegen_expr(ARCH,,arm64_codegen_ctx*);
void codegen_stmt(ARCH,,arm64_codegen_ctx*);
void codegen_if_stmt(ARCH,,arm64_codegen_ctx*);
void codegen_function(ARCH,,arm64_codegen_ctx*);

#undef ARCH

#endif
