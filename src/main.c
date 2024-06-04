#include "ast.h"
#include "codegen.h"
#include "lalr.h"
#include "lib/arena.h"
#include "tokenizer.h"
#include <stdio.h>
#include <string.h>
#include "../getopt/cmdline.h"

int tokenize(const char* file);
int compile(const char* file);
int codegen(const char* file, const char* target);

int main(int argc, char **argv) {
	struct gengetopt_args_info args_info;
	if (cmdline_parser(argc, argv, &args_info) != 0) {
		return 1;
	}

	// getopt is setup so that `args_info.file_arg` is a required field
	//    it also enforces a set of supported arguments
	//    see config.ggo
	if (args_info.tokenize_flag) {
		int ret = tokenize(args_info.file_arg);
		cmdline_parser_free(&args_info);
		return ret;
	}
	else if (args_info.parse_flag) {
		int ret = compile(args_info.file_arg);
		cmdline_parser_free(&args_info);
		return ret;
	}
	else if (args_info.codegen_flag) {
		if (!args_info.target_given) {
			fprintf(stderr, "Must provide a target for codegen\n");
			return -1;
		}
		int ret = codegen(args_info.file_arg, args_info.target_arg);
		cmdline_parser_free(&args_info);
		return ret;
	}

	return 2;
}

int tokenize(const char* file) {
	tokenizer_ctx ctx = tctx_from_file(file);
	if (ctx.fail) {
		fprintf(stderr, "Failed to open file\n");
		tctx_free(&ctx);
		return 2;
	}

	lalr_ctx pctx = lalr_create();

	token t;
	while ((t = tctx_get_next(&ctx)).type != T_EOF) {
		tctx_advance(&ctx);
		printf(TOKEN_STR_FMT "\n", TOKEN_STR_ARG(t));
		if (t.type == T_EOF) break;
	}

	return 0;
}

int compile(const char* file) {
	tokenizer_ctx ctx = tctx_from_file(file);
	if (ctx.fail) {
		fprintf(stderr, "Failed to open file\n");
		tctx_free(&ctx);
		return 2;
	}

	lalr_ctx pctx = lalr_create();

	token t;
	while ((t = tctx_get_next(&ctx)).type != T_EOF) {
		tctx_advance(&ctx);
		pctx.lookahead = tctx_get_next(&ctx);
		if (t.type == T_EOF) break;

		AST_Node n = {0};
		lalr_reduce_tok_to_term(t, &n);
		lalr_push(&pctx, n);

		int popped = 0;
		while ((popped = lalr_reduce(&pctx, &n)) != 0) {
			lalr_pop_n(&pctx, popped);
			lalr_push(&pctx, n);
			if (pctx.skip_next == 1) {
				tctx_advance(&ctx);
				pctx.lookahead = tctx_get_next(&ctx);
				pctx.skip_next = 0;
				break;
			}
		}
	}
	printf("End Stack\n");
	lalr_show_stack(&pctx);

	printf("bytes used : %lu\n", arena_bytes(&pctx.arena));
	arena_free(&pctx.arena);

	tctx_free(&ctx);
	return 1;
}

int codegen(const char* file, const char* target) {
	if (strcmp(target, "x86") != 0) {
		fprintf(stderr, "Target [%s] not supported for codegen\n", target);
		return -1;
	}

	tokenizer_ctx ctx = tctx_from_file(file);
	if (ctx.fail) {
		fprintf(stderr, "Failed to open file\n");
		tctx_free(&ctx);
		return 2;
	}

	lalr_ctx pctx = lalr_create();

	token t;
	while ((t = tctx_get_next(&ctx)).type != T_EOF) {
		tctx_advance(&ctx);
		pctx.lookahead = tctx_get_next(&ctx);
		if (t.type == T_EOF) break;

		AST_Node n = {0};
		lalr_reduce_tok_to_term(t, &n);
		lalr_push(&pctx, n);

		int popped = 0;
		while ((popped = lalr_reduce(&pctx, &n)) != 0) {
			lalr_pop_n(&pctx, popped);
			lalr_push(&pctx, n);
			if (pctx.skip_next == 1) {
				tctx_advance(&ctx);
				pctx.lookahead = tctx_get_next(&ctx);
				pctx.skip_next = 0;
				break;
			}
		}
	}

	codegen_x86(pctx.stack[0]);

	arena_free(&pctx.arena);

	tctx_free(&ctx);
	
	return 0;
}
