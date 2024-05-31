#include "ast.h"
#include "ast_free.h"
#include "lalr.h"
#include "tokenizer.h"
#include <stdio.h>
#include "../getopt/cmdline.h"

int compile(const char* file);

int main(int argc, char **argv) {
	struct gengetopt_args_info args_info;
	if (cmdline_parser(argc, argv, &args_info) != 0) {
		return 1;
	}

	if (args_info.file_given) {
		return compile(args_info.file_arg);	
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
		printf("t = %s; lookahead = %s\n", token_str(t.type), token_str(pctx.lookahead.type));

		AST_Node n;
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
		// lalr_show_stack(&pctx);
	}
	printf("End Stack\n");
	lalr_show_stack(&pctx);

	for (int i = 0; i < pctx.stack_top; i++) {
		ast_free_node(pctx.stack[i]);
	}

	tctx_free(&ctx);
	return 1;
}
