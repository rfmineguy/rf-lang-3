#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <stddef.h>
#include <regex.h>
#include "lib/sv.h"
#include "lib/cvector.h"

typedef enum token_type {
	// Unreserved tokens
	T_ID = 0, 
	T_DOUBLE_LIT, T_DECIMAL_LIT, T_HEX_LIT, T_STRING_LIT, T_CHAR_LIT,
	T_SINGLE_LINE_COMMENT, T_MULTI_LINE_COMMENT,

	// Reserved tokens
	T_RESERVE_BEG = 100,
	T_FN, T_IF, T_ELSE,
	T_SWITCH, T_BREAK, T_DEFAULT,
	T_SQUOTE, T_DQUOTE,
	T_LP, T_RP,
	T_LBRC, T_RBRC,
	T_LBRK, T_RBRK,
	T_RETURN,
	T_ARROW,
	T_RESERVE_END,

	T_BOR = 200, T_BAND, // bitwise

	// Assign
	T_EQ,

	// LogicOp
	T_LOGIC_BEG = 300,
	T_DEQ = 301, T_GT = 302, T_LT = 303, T_GTEQ = 304, T_LTEQ = 305,
	T_LOR, T_LAND, // logical
	T_LOGIC_END,

	// StackOp
	T_STACK_BEG = 400,
	T_COLON, T_COMMA, T_PERIOD, T_SEMI,
	T_STACK_END,

	// ArithOp
	T_ARITH_BEG = 500,
	T_MINUS, T_PLUS,T_MUL, T_DIV, T_MOD,
	T_ARITH_END,

	// Internal tokens
	T_EOF = 600, T_SPACE, T_UNKNOWN
} token_type;

typedef struct tokenizer_state {
	char const *cursor;
	int line, col, index;
} tokenizer_state;

typedef union tokenizer_regex_store {
	regex_t *r_store;
	struct {
		regex_t r_string_lit; // 1
		regex_t r_char_lit; // 1
		regex_t r_fn, r_if, r_else, r_switch, r_break, r_default, r_module; // 7
		regex_t r_hexlit, r_dbllit, r_declit, r_id; // 4
		regex_t r_lor, r_land, r_gteq, r_lteq, r_deq; // 5
		regex_t r_comma, r_period, r_semi; // 3
		regex_t r_return, r_arrow; // 1

		// Total : 1 + 1 + 7 + 4 + 5 + 3 + 1 = 22
	};
} tokenizer_regex_store;

typedef struct token {
	token_type type;
	String_View text;
	tokenizer_state state;
} token;

typedef struct tokenizer_ctx {
	bool fail;
	char const *content;
	size_t content_length;
	tokenizer_state state;
	tokenizer_regex_store regex_store;
} tokenizer_ctx;

regex_t       rnew(const char*);
int           rmatch(const char*, regex_t, int*);

#define TOKEN_STR_FMT "%-20s " SV_Fmt
#define TOKEN_STR_ARG(token) token_str(token.type), SV_Arg(token.text)
const char*   token_str(token_type);

bool 					is_token_terminal(token*);

tokenizer_ctx tctx_from_file(const char*);
tokenizer_ctx tctx_from_cstr(const char*);
void          tctx_free(tokenizer_ctx*);

token         tctx_advance(tokenizer_ctx*);
token         tctx_get_next(tokenizer_ctx*);
#define       tctx_show_next(t) { tctx_show_next_internal(t, __LINE__); }
void          tctx_show_next_internal(tokenizer_ctx*, int);

tokenizer_state tctx_save(tokenizer_ctx*);
void            tctx_restore(tokenizer_ctx*, tokenizer_state);

#endif
