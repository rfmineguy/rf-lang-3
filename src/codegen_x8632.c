#include "codegen_x8632.h"
#include "ast.h"
#include "ast_util.h"
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
			case NT_STATEMENT_LIST:
				codegen_stmt_list_x86_32(&ctx, n.stmtList);
				break;
				// NOTE: temporary
				printf("Expression codegen\n");
				codegen_expr_x86_32(&ctx, n.expr);
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
	codegen_expr_x86_32(ctx, assign.expr);
}

void codegen_expr_x86_32(codegen_x86_32_ctx* ctx, Expression* expr) {
	printf("Expr\n");
	LogicalDisj* disj = expr->disj;
	switch (disj->type) {
		case LOGICAL_DISJ_TYPE_DISJ_CONJ: {
			LogicalDisj* d = disj->disj;
			LogicalConj* c = disj->conj;
		}
		case LOGICAL_DISJ_TYPE_CONJ: assert(0 && "Disjunction Conjunction not supported");
	}
}

void codegen_term_x86_32(codegen_x86_32_ctx* ctx, Term t) {
	char op = t.op;
	if (t.type == TERM_TYPE_TERM_OP_FACTOR) {
		Term t = *t.left;
		Factor f = t.right;
		char op = t.op;
		switch (f.type) {
			case FACTOR_TYPE_NUMBER:
				switch (op) {
					case '*': {
						fprintf(ctx->file, "mov eax, %d\n", f.number.i);
					}
				}
				break;
			default: assert(0 && "Factor type unsupported");
		}
		switch (op) {
			case '*': {
				// fprintf(ctx->file, "", ...)
			}
		}
	}
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
	codegen_stmt_list_x86_32(ctx, block.stmts);
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

void codegen_stmt_list_x86_32	(codegen_x86_32_ctx* ctx, StatementList* stmts) {
	StatementList* curr = stmts;
	while (curr) {
		codegen_stmt_x86_32(ctx, curr->stmt);
		curr = curr->next;
	}
}

void codegen_if_stmt_x86_32(codegen_x86_32_ctx* ctx, IfStatement iff) {
	assert(0 && "If statement codegen not implemented");
}
