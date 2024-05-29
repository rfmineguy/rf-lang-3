#include "ast.h"
#include "ast_util.h"
#include "ast_free.h"
#include "lalr.h"
#include "number_parser.h"
#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>

int main() {
	tokenizer_ctx ctx = tctx_from_file("ex/simpler_test.rf");
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
}
