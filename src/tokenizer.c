#include "tokenizer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>

regex_t rnew(const char* r) {
	regex_t reg;
	if (regcomp(&reg, r, REG_EXTENDED) != 0) {
		printf("Failed to compile regex [%s]\n", r);
		exit(1);
	}
	return reg;
}

int rmatch(const char* s, regex_t r, int* length_out) {
	regmatch_t match[1];
	if (regexec(&r, s, 1, match, 0) == 0) {
		// make sure the match was immediately at s
		if (match[0].rm_so != 0)
			return -1;
		const char* start = s + match[0].rm_so;
		const char* end = s + match[0].rm_eo;
		int len = end - start;
		*length_out = len;
		return 0;
	}
	return -1;
}

char* read_file(const char* filename, size_t* length) {
	FILE* f = fopen(filename, "r");
	if (!f) {
		fprintf(stderr, "Failed to open file: %s\n", filename);
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	size_t l = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (length) {
		*length = l;
	}
	char *content = calloc(l + 1, sizeof(char));
	size_t read_bytes = fread(content, sizeof(char), l, f);
	content[read_bytes] = 0;
	if (read_bytes != l) {
		fprintf(stderr, "Something went wrong reading: %lu != %lu\n", read_bytes, l);	
		free(content);
		content = NULL;
	}
	fclose(f);
	return content;
}

const char* token_str(token_type t) {
	switch (t) {
		case T_ID:      		return "ID";
		case T_FN:      		return "FN";
		case T_IF:      		return "IF";
		case T_ELSE: 				return "ELSE";
		case T_SWITCH:      return "SWITCH";
		case T_BREAK: 			return "BREAK";
		case T_DEFAULT: 		return "DEFAULT";
		case T_LP:      		return "LP";
		case T_RP:      		return "RP";
		case T_LBRK:				return "LBRK";
		case T_RBRK:				return "RBRK";
		case T_LBRC:				return "LBRC";
		case T_RBRC:				return "RBRC";
		case T_DEQ:					return "DEQ"; 					 
		case T_GT: 					return "GT"; 
		case T_LT: 					return "LT";
		case T_GTEQ:				return "GTEQ"; 
		case T_LTEQ: 				return "LTEQ";
		case T_LOR:  				return "LOR";
		case T_LAND:   			return "LAND";
		case T_BOR:    			return "BOR";
		case T_BAND:   			return "BAND";
		case T_EQ: 					return "EQ";
		case T_COLON:   		return "COLON";
		case T_COMMA: 	    return "COMMA";
		case T_PERIOD:	    return "PERIOD";
		case T_SEMI:        return "SEMI";
		case T_MINUS:   		return "MINUS";
		case T_PLUS:    		return "PLUS";
		case T_MUL:     		return "MUL";
		case T_DIV:     		return "DIV";
		case T_MOD:  				return "MOD";
		case T_DOUBLE_LIT:  return "DOUBLE_LIT";
		case T_DECIMAL_LIT: return "DECIMAL_LIT";
		case T_HEX_LIT:   	return "HEX_LIT";
		case T_STRING_LIT:  return "STRING_LIT";
		case T_CHAR_LIT:    return "CHAR_LIT";
		case T_SQUOTE: 			return "SQUOTE";
		case T_DQUOTE:      return "DQUOTE";
		case T_EOF: 				return "EOF (Shouldnt be printed)";
		case T_WHITESPACE:  return "WHITESPACE (Shouldnt be printed)";
		case T_NEWLINE:     return "NEWLINE (Shouldnt be printed)";
		case T_RETURN:      return "RETURN";
		case T_ARROW:  			return "ARROW";
		case T_UNDERSCORE:  return "UNDERSCORE";
		case T_UNKNOWN:  		return "UNKNOWN";
		case T_SINGLELINE_COMMENT: return "SINGLELINE_COMMENT";
		case T_BEGIN_SINGLELINE_COMMENT: return "BEGIN_SINGLELINE_COMMENT";
		case T_BEGIN_MULTILINE_COMMENT: return "BEGIN_MULTILINE_COMMENT";
		case T_END_MULTILINE_COMMENT: return "END_MULTILINE_COMMENTT";
		case T_RESERVE_BEG: return "RESERVE_BEG";
		case T_RESERVE_END: return "RESERVE_END";
		case T_ARITH_BEG:   return "ARITH_BEG"; 
		case T_ARITH_END:   return "ARITH_END"; 
		case T_STACK_BEG:   return "STACK_BEG"; 
		case T_STACK_END:   return "STACK_END"; 
		case T_LOGIC_BEG:   return "LOGIC_BEG"; 
		case T_LOGIC_END:   return "LOGIC_END"; 
	}
}

void tctx_internal_init_regex(tokenizer_ctx* ctx) {
	memset(&ctx->regex_store, 0, sizeof(tokenizer_regex_store));

	/* Multiline Comment
	 *     \/\(*.(\r\n|\r|\n)+\*\/
	 * Singleline Comment
	 *     \/\/.*(\n|\r|\r\n)
	 */
	ctx->regex_store.store.r_begin_multicomment   = rnew("\\/\\*");
	ctx->regex_store.store.r_end_multicomment   	= rnew("\\*\\/");
	// ctx->regex_store.store.r_begin_singlecomment  = rnew("\\/\\/");
	ctx->regex_store.store.r_string_lit    				= rnew("\\\"([^\\\"]|\r\n)*\\\"");
	ctx->regex_store.store.r_char_lit      				= rnew("\\\'(.)\\\'");
	ctx->regex_store.store.r_fn            				= rnew("fn");
	ctx->regex_store.store.r_if            				= rnew("if");
	ctx->regex_store.store.r_else          				= rnew("else");
	ctx->regex_store.store.r_switch        				= rnew("switch");
	ctx->regex_store.store.r_break         				= rnew("break");
	ctx->regex_store.store.r_default       				= rnew("default");
	ctx->regex_store.store.r_module        				= rnew("module");
	ctx->regex_store.store.r_return        				= rnew("return");
	ctx->regex_store.store.r_hexlit        				= rnew("0x[0-9a-fA-F]+");
	ctx->regex_store.store.r_dbllit        				= rnew("[0-9]+\\.[0-9]+");
	ctx->regex_store.store.r_declit        				= rnew("[0-9]+");
	ctx->regex_store.store.r_id            				= rnew("[a-zA-Z][a-zA-Z0-9_]*");
	ctx->regex_store.store.r_lor           				= rnew("\\|\\|");
	ctx->regex_store.store.r_land          				= rnew("&&");
	ctx->regex_store.store.r_gteq          				= rnew(">=");
	ctx->regex_store.store.r_lteq          				= rnew("<=");
	ctx->regex_store.store.r_deq           				= rnew("==");
	ctx->regex_store.store.r_comma         				= rnew(",");
	ctx->regex_store.store.r_period        				= rnew(".");
	ctx->regex_store.store.r_semi          				= rnew(";");
	ctx->regex_store.store.r_arrow         				= rnew("->");
	ctx->regex_store.store.r_newline              = rnew("\r|\r\n|\n");
}

void tctx_internal_free_regex(tokenizer_ctx* ctx) {
	int count = sizeof(tokenizer_regex_store) / sizeof(regex_t);
	for (int i = 0; i < count; i++) {
		regfree(&ctx->regex_store.r_store[i]);
	}
}

tokenizer_ctx tctx_from_file(const char* filename) {
	tokenizer_ctx ctx = {0};
	char* content = read_file(filename, &ctx.content_length);
	if (content == NULL) ctx.fail = true;
	ctx.content = content;
	ctx.state.cursor = content;
	tctx_internal_init_regex(&ctx);
	return ctx;
}

tokenizer_ctx tctx_from_cstr(const char* cstr) {
	tokenizer_ctx ctx = {0};
	ctx.content = strdup(cstr);
	ctx.content_length = strlen(cstr);
	ctx.state.cursor = ctx.content;
	tctx_internal_init_regex(&ctx);
	return ctx;
}

void tctx_free(tokenizer_ctx* ctx) {
	tctx_internal_free_regex(ctx);
	free((void*) ctx->content);
}

#define RMATCH(str, t) \
	do {\
		int length;\
		if (rmatch(ctx->state.cursor, str, &length) != -1) {\
			const char* was = ctx->state.cursor;\
			return (token) {\
				.type = t,\
				.text=(sv_from_parts(was, length)),\
				.state = ctx->state,\
				.loc = ctx->state.loc\
			};\
		}\
	} while(0)

#define CHMATCH(c, t) \
	do {\
		if (*ctx->state.cursor == c) {\
			const char* was = ctx->state.cursor;\
			return (token) {\
				.type = t,\
				.text=(sv_from_parts(was, 1)),\
				.state = ctx->state,\
				.loc = ctx->state.loc\
			};\
		}\
	} while(0)

void tctx_advance_internal(tokenizer_ctx* ctx, token t) {
	ctx->state.cursor += t.text.count;
	ctx->state.loc = t.loc;
}

void tctx_advance(tokenizer_ctx* ctx) {
	token t = tctx_get_next(ctx);
	tctx_advance_internal(ctx, t);
}

size_t isnewline(const char* s) {
	if (*s == '\r') return 1;
	if (*s == '\n') return 1;
	if (*s == '\r' && *(s + 1) == '\n') return 2;
	return 0;
}

token tctx_get_next(tokenizer_ctx* ctx) {
	// Detect EOF
	if (ctx->state.cursor >= ctx->content + ctx->content_length - 1 ||
			*ctx->state.cursor == '\0')
		return (token) {.type=T_EOF };

	// Deal with whitespace and newlines
	while (*ctx->state.cursor == ' ' || *ctx->state.cursor == '\t' || *ctx->state.cursor == '\n') {
		ctx->state.loc.col++;
		ctx->state.loc.index++;
		if (*ctx->state.cursor == '\n') {
			ctx->state.loc.line ++;
			ctx->state.loc.col = 0;
		}
		ctx->state.cursor++;
	}

	// Detect comment
	bool skippedComments = false;
	while (strncmp(ctx->state.cursor, "//", 2) == 0) {
		size_t len = 0;
		while ((len = isnewline(ctx->state.cursor)) == 0) {
			// printf("skipping %c\n", *ctx->state.cursor);
			ctx->state.cursor++;
		}
		ctx->state.cursor += len;
		// printf("comment-next: %c\n", *ctx->state.cursor);
		skippedComments = true;
	}

	// Deal with whitespace and newlines
	while (*ctx->state.cursor == ' ' || *ctx->state.cursor == '\t' || *ctx->state.cursor == '\n') {
		ctx->state.loc.col++;
		ctx->state.loc.index++;
		if (*ctx->state.cursor == '\n') {
			ctx->state.loc.line ++;
			ctx->state.loc.col = 0;
		}
		ctx->state.cursor++;
	}
	// if (skippedComments)
	// 	printf("Next : %c\n", *ctx->state.cursor);

	// Match code
	//   To see the actual regex strings, view tctx_internal_init_regex(..)
	RMATCH(ctx->regex_store.store.r_string_lit, T_STRING_LIT);
	RMATCH(ctx->regex_store.store.r_char_lit, T_CHAR_LIT);
	RMATCH(ctx->regex_store.store.r_fn, T_FN);
	RMATCH(ctx->regex_store.store.r_if, T_IF);
	RMATCH(ctx->regex_store.store.r_else, T_ELSE);
	RMATCH(ctx->regex_store.store.r_switch, T_SWITCH);
	RMATCH(ctx->regex_store.store.r_break, T_BREAK);
	RMATCH(ctx->regex_store.store.r_default, T_DEFAULT);
	RMATCH(ctx->regex_store.store.r_return, T_RETURN);
	RMATCH(ctx->regex_store.store.r_arrow, T_ARROW);
	RMATCH(ctx->regex_store.store.r_hexlit, T_HEX_LIT);
	RMATCH(ctx->regex_store.store.r_dbllit, T_DOUBLE_LIT);
	RMATCH(ctx->regex_store.store.r_declit, T_DECIMAL_LIT);
	RMATCH(ctx->regex_store.store.r_id, T_ID);
	RMATCH(ctx->regex_store.store.r_lor, T_LOR);
	RMATCH(ctx->regex_store.store.r_land, T_LAND);
	RMATCH(ctx->regex_store.store.r_gteq, T_GTEQ);
	RMATCH(ctx->regex_store.store.r_lteq, T_LTEQ);
	RMATCH(ctx->regex_store.store.r_deq, T_DEQ);
	RMATCH(ctx->regex_store.store.r_newline, T_NEWLINE);
	CHMATCH('|', T_BOR);
	CHMATCH('&', T_BAND);
	CHMATCH('>', T_GT);
	CHMATCH('<', T_LT);
	CHMATCH(':', T_COLON);
	CHMATCH(';', T_SEMI);
	CHMATCH(',', T_COMMA);
	CHMATCH('.', T_PERIOD);
	CHMATCH('(', T_LP);
	CHMATCH(')', T_RP);
	CHMATCH('{', T_LBRC);
	CHMATCH('}', T_RBRC);
	CHMATCH('[', T_LBRK);
	CHMATCH(']', T_RBRK);
	CHMATCH('_', T_UNDERSCORE);
	CHMATCH('-', T_MINUS);
	CHMATCH('+', T_PLUS);
	CHMATCH('*', T_MUL);
	// CHMATCH('/', T_DIV);
	CHMATCH('%', T_MOD);
	CHMATCH('=', T_EQ);
	CHMATCH('\'', T_SQUOTE);
	CHMATCH('\"', T_DQUOTE);

	return (token) {.type=T_UNKNOWN, .text=sv_from_parts(ctx->state.cursor, 1)};
}

void tctx_show_next_internal(tokenizer_ctx* ctx, int line) {
	token next = tctx_get_next(ctx);
	printf("[%d] Next: %s\n", line, token_str(next.type));
}

tokenizer_state tctx_save(tokenizer_ctx* ctx) {
	return ctx->state;
}

void tctx_restore(tokenizer_ctx* ctx, tokenizer_state state) {
	ctx->state = state;
}
