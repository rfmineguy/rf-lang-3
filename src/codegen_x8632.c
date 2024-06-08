#include "codegen_x8632.h"
#include "ast.h"
#include "lalr.h"
#include <assert.h>
#include <stdio.h>
#define ARCH x86_32

void codegen_entry_x86_32(lalr_ctx* lctx, FILE* f) {
	codegen_x86_32_ctx ctx = {0};
	printf("codegen x86_32\n");
	ctx.file = f;
	for (int i = 0; i <= lctx->stack_top; i++) {
		AST_Node n = lctx->stack[i];
		switch (n.type) {
			case NT_HEADER:
				codegen_header_x86_32(&ctx, n.header);
				break;
			case NT_FUNCTION: 
				codegen_function_x86_32(&ctx, n.function);
				break;
			default: break;
		}
	}
}

void codegen_header_x86_32(codegen_x86_32_ctx* ctx, Header header) {
	switch (header.type) {
		case HEADER_TYPE_MODULE: 
			fprintf(ctx->file, "; module " SV_Fmt "\n", SV_Arg(header.module.name));
			break;
		case HEADER_TYPE_USE:
			fprintf(ctx->file, "; use \n"); 
			break;
	}	
}

void codegen_assign_x86_32(codegen_x86_32_ctx* ctx, AssignStatement assign) {
	assert(0 && "Assign codegen not implemented");
}

void codegen_expr_x86_32(codegen_x86_32_ctx* ctx, Expression* expr) {
	assert(0 && "Expression codegen not implemented");
}

void codegen_function_x86_32(codegen_x86_32_ctx* ctx, Function func) {
	// func name template -> [name]_[type1]_[type2]_..._[typeRet]
	fprintf(ctx->file, "" SV_Fmt "_", SV_Arg(func.id));

	// Codegen FuncHeader
	TypedIdList* curr = func.header.params;
	while (curr) {
		switch (curr->typedId.type.type) {
			case VAR_TYPE_ID:
				fprintf(ctx->file, "%c_", curr->typedId.type.Id.id.data[0]);
				break;
			case VAR_TYPE_ARRAY:
				fprintf(ctx->file, "A%c_", curr->typedId.type.Array.id.data[0]);
				break;
			case VAR_TYPE_NESTED:
				assert(0 && "VAR_TYPE_NESTED unsupported in codegen");
				break;
			case VAR_TYPE_NONE:
				fprintf(ctx->file, "_");
				break;
		}
		curr = curr->next;
	}
	fprintf(ctx->file, ":\n");

	// Codegen block 
	codegen_block_x86_32(ctx, func.block);

	// Codegen return
	fprintf(ctx->file, "ret\n");
}

void codegen_block_x86_32(codegen_x86_32_ctx* ctx, Block block) {
	StatementList* curr = block.stmts;
	while (curr) {
		codegen_stmt_x86_32(ctx, curr->stmt);
		curr = curr->next;
	}
}

void codegen_stmt_x86_32(codegen_x86_32_ctx* ctx, Statement stmt) {
	switch (stmt.type) {
		case STATEMENT_TYPE_IF:
			codegen_if_stmt_x86_32(ctx, stmt.iff);
			break;
		case STATEMENT_TYPE_ASSIGN:
			codegen_assign_x86_32(ctx, stmt.assign);
			break;
		case STATEMENT_TYPE_RETURN:
			assert(0 && "Return statement codegen not implemented");
			break;
	}

}

void codegen_if_stmt_x86_32(codegen_x86_32_ctx* ctx, IfStatement iff) {
	assert(0 && "If statement codegen not implemented");
}
