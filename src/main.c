#include "ast.h"
#include "ast_util.h"
#include "lalr.h"
#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>

int main() {
	// printf("%*c\rText\n", 0, ' ');
	// int d = 0;
	// printf(d==0?"%*c\r":"%*c", d * 2, ' ');
	// printf("Text\n");
	tokenizer_ctx ctx = tctx_from_file("ex/simpler_test.rf");
	if (ctx.fail) {
		fprintf(stderr, "Failed to open file\n");
		tctx_free(&ctx);
		return 2;
	}

	lalr_ctx pctx = lalr_create();

	token t;
	while ((t = tctx_get_next(&ctx)).type != T_EOF) {
		// printf("%d: %s\n", t.type, token_str(t.type));
		tctx_advance(&ctx);
		if (t.type == T_EOF) break;

		AST_Node n;
		lalr_reduce_tok_to_term(t, &n);
		lalr_push(&pctx, n);

		int popped = 0;
		while ((popped = lalr_reduce(&pctx, &n)) != 0) {
			lalr_pop_n(&pctx, popped);
			lalr_push(&pctx, n);
		}
	}

	for (int i = 0; i <= pctx.stack_top; i++) {
		printf("(%d)==========\n", i);
		ast_print_node(pctx.stack[i], 1);
	}

	tctx_free(&ctx);
}
