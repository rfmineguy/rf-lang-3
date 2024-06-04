#include "codegen_arm64.h"
#include "ast.h"
#include "lalr.h"
#define ARCH arm64

void codegen_entry_arm64(lalr_ctx* pctx, FILE* f) {
	codegen_arm64_ctx ctx = {0};
	ctx.f = f;
	assert(0 && "Arm64 codegen not supported");
}

void codegen_header(codegen_arm64_ctx* ctx, Header header) {

}

void codegen_assign(codegen_arm64_ctx* ctx, AssignStatement assign) {

}

void codegen_expr(codegen_arm64_ctx* ctx, Expression expr) {

}

void codegen_stmt(codegen_arm64_ctx* ctx, Statement stmt) {

}

void codegen_if_stmt(codegen_arm64_ctx* ctx, IfStatement iff) {

}

void codegen_function(codegen_arm64_ctx* ctx, Function func) {

}
