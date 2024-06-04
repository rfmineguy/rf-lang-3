#include "codegen_x8632.h"
#include "ast.h"
#include <stdio.h>
#define ARCH x86_32

void codegen_entry_point(ARCH, node, FILE* f) {
	x86_32_codegen_ctx ctx = {0};
	ctx.file = f;
	switch (node.type) {
		case NT_HEADER: codegen_header_x86_32(node.header, &ctx);
										break;
		default: break;
	}
	printf("codegen x86_32\n");
}

void codegen_header(ARCH, header, x86_32_codegen_ctx* ctx) {
	switch (header.type) {
		case HEADER_TYPE_MODULE: 
			fprintf(ctx->file, "; module " SV_Fmt "\n", SV_Arg(header.module.name));
			break;
		case HEADER_TYPE_USE:
			fprintf(ctx->file, "; use "); 
			break;
	}	
}

void codegen_assign(ARCH, assign, x86_32_codegen_ctx* ctx) {

}

void codegen_expr(ARCH, expr, x86_32_codegen_ctx* ctx) {

}

void codegen_stmt(ARCH, stmt, x86_32_codegen_ctx* ctx) {

}

void codegen_if_stmt(ARCH, iff, x86_32_codegen_ctx* ctx) {

}

void codegen_function(ARCH, func, x86_32_codegen_ctx* ctx) {

}
