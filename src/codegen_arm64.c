#include "codegen_arm64.h"
#define ARCH arm64

void codegen_entry_point(ARCH, node) {
	assert(0 && "Arm64 codegen not supported");
}

void codegen_header(ARCH, header, arm64_codegen_ctx* ctx) {

}

void codegen_assign(ARCH, assign, arm64_codegen_ctx* ctx) {

}

void codegen_expr(ARCH, expr, arm64_codegen_ctx* ctx) {

}

void codegen_stmt(ARCH, stmt, arm64_codegen_ctx* ctx) {

}

void codegen_if_stmt(ARCH, iff, arm64_codegen_ctx* ctx) {

}

void codegen_function(ARCH, func, arm64_codegen_ctx* ctx) {

}
