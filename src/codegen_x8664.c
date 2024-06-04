#include "codegen_x8664.h"

void codegen_entry_x86_64(lalr_ctx* pctx, FILE* f) {
	codegen_x86_64_ctx ctx = {0};
	ctx.f = f;
	assert(0 && "x86_64 codegen not supported");
}

void codegen_header_x86_64(codegen_x86_64_ctx* ctx, Header header) {

}

void codegen_assign_x86_64(codegen_x86_64_ctx* ctx, AssignStatement assign) {

}

void codegen_expr_x86_64(codegen_x86_64_ctx* ctx, Expression* expr) {

}

void codegen_stmt_x86_64(codegen_x86_64_ctx* ctx, Statement stmt) {

}

void codegen_if_stmt_x86_64(codegen_x86_64_ctx* ctx, IfStatement iff) {

}

void codegen_function_x86_64(codegen_x86_64_ctx* ctx, Function func) {

}
