#include "ast.h"
#include "lalr.h"
#include "lib/arena.h"
#include "tokenizer.h"
#include <stdio.h>
#include "../getopt/cmdline.h"

int tokenize(const char* file);
int compile(const char* file);

int main(int argc, char **argv) {
	struct gengetopt_args_info args_info;
	if (cmdline_parser(argc, argv, &args_info) != 0) {
		return 1;
	}

	if (strcmp(args_info.action_arg, "tokenize") == 0) {
		return tokenize(args_info.file_arg);
	}

	if (args_info.file_given) {
		int ret = compile(args_info.file_arg);
		cmdline_parser_free(&args_info);
		return ret;
	}

	return 0;
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
