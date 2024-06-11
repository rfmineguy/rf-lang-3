#include "ast.h"
#include "ast_print.h"
#include "ast_util.h"
#include "codegen_x8632.h"
#include "codegen_x8664.h"
#include "codegen_arm64.h"
#include "lalr.h"
#include "lib/arena.h"
#include "number_parser.h"
#include "tokenizer.h"
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include "../getopt/cmdline.h"

lalr_ctx parse(const char* file);

int tokenize(const char* file);
int compile(const char* file);
int codegen(const char* file, const char* output, const char* target);
int reconstruct(const char* file);

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
	else if (args_info.reconstruct_flag) {
		int ret = reconstruct(args_info.file_arg);
		cmdline_parser_free(&args_info);
		return ret;
	}
	else if (args_info.codegen_flag) {
		if (!args_info.target_given) {
			fprintf(stderr, "Must provide a target for codegen\n");
			return -1;
		}
		int ret = codegen(args_info.file_arg, args_info.output_arg, args_info.target_arg);
		cmdline_parser_free(&args_info);
		return ret;
	}

	return 2;
}

lalr_ctx parse(const char* file) {	
	tokenizer_ctx ctx = tctx_from_file(file);
	if (ctx.fail) {
		fprintf(stderr, "Failed to open file\n");
		tctx_free(&ctx);
		return (lalr_ctx) {};
	}

	lalr_ctx lctx = lalr_create();

	token t;
	while ((t = tctx_get_next(&ctx)).type != T_EOF) {
		tctx_advance(&ctx);
		lctx.lookahead = tctx_get_next(&ctx);
		if (t.type == T_EOF) break;

		AST_Node n = {0};
		lalr_reduce_tok_to_term(t, &n);
		lalr_push(&lctx, n);

		int popped = 0;
		while ((popped = lalr_reduce(&lctx, &n)) != 0) {
			lalr_pop_n(&lctx, popped);
			lalr_push(&lctx, n);
			if (lctx.skip_next == 1) {
				tctx_advance(&ctx);
				lctx.lookahead = tctx_get_next(&ctx);
				lctx.skip_next = 0;
				break;
			}
		}
	}

	return lctx;
}

int tokenize(const char* file) {
	tokenizer_ctx ctx = tctx_from_file(file);
	if (ctx.fail) {
		fprintf(stderr, "Failed to open file\n");
		tctx_free(&ctx);
		return 2;
	}

	token t;
	while ((t = tctx_get_next(&ctx)).type != T_EOF) {
		tctx_advance(&ctx);
		printf(TOKEN_STR_FMT "\n", TOKEN_STR_ARG(t));
		if (t.type == T_EOF) break;
	}
	tctx_free(&ctx);

	return 0;
}

int compile(const char* file) {	
	tokenizer_ctx ctx = tctx_from_file(file);
	if (ctx.fail) {
		fprintf(stderr, "Failed to open file\n");
		tctx_free(&ctx);
		return 2;
	}

	lalr_ctx lctx = lalr_create();

	token t;
	while ((t = tctx_get_next(&ctx)).type != T_EOF) {
		tctx_advance(&ctx);
		// printf(TOKEN_STR_FMT "\n", TOKEN_STR_ARG(t));
		lctx.lookahead = tctx_get_next(&ctx);
		if (t.type == T_EOF) break;

		AST_Node n = {0};
		lalr_reduce_tok_to_term(t, &n);
		lalr_push(&lctx, n);

		int popped = 0;
		while ((popped = lalr_reduce(&lctx, &n)) != 0) {
			lalr_pop_n(&lctx, popped);
			lalr_push(&lctx, n);
			if (lctx.skip_next == 1) {
				tctx_advance(&ctx);
				lctx.lookahead = tctx_get_next(&ctx);
				lctx.skip_next = 0;
				break;
			}
		}
	}

	lalr_show_stack(&lctx);

	printf("bytes used : %lu\n", arena_bytes(&lctx.arena));
	arena_free(&lctx.arena);
	return 1;
}

int codegen(const char* file, const char* output, const char* target) {
	lalr_ctx pctx = lalr_create();

	FILE* f = fopen(output, "w");
	if (!f) {
		fprintf(stderr, "Failed to open output.s\n");
	}
	else {
		if (strcmp(target, "x86_32") == 0) {
			codegen_entry_x86_32(&pctx, f);
		}
		else if (strcmp(target, "x86_64") == 0) {
			codegen_entry_x86_64(&pctx, f);
		}
		else if (strcmp(target, "arm64") == 0) {
			codegen_entry_arm64(&pctx, f);
		}
		else {
			fprintf(stderr, "%s not a supported target\n", target);
		}
	}
	fclose(f);

	arena_free(&pctx.arena);
	
	return 0;
}

int reconstruct(const char* file) {
	lalr_ctx pctx = parse(file);

	for (int i = 0; i <= pctx.stack_top; i++) {
		AST_Node n = pctx.stack[i];
		ast_print_node(n, 0);
		ast_util_reconstruct_ast_node(n);
	}
	return 0;
}
