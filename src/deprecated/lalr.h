#ifndef LALR_H
#define LALR_H
#include "../ast.h"
#include "../tokenizer.h"
#include "../lib/arena.h"

typedef struct lalr_ctx {
	// lalr data
	token current;
	token lookahead;
	int skip_next;

	// parse stack
	AST_Node stack[255];
	int stack_top;

	Arena arena;
} lalr_ctx;

lalr_ctx  lalr_create();

void      lalr_show_stack(lalr_ctx*);

int       lalr_reduce_tok_to_term(token, AST_Node*);
int       lalr_reduce(lalr_ctx*, AST_Node*);

void      lalr_push(lalr_ctx*, AST_Node);
AST_Node  lalr_peek(lalr_ctx*, AST_Node);
AST_Node  lalr_peek_n(lalr_ctx*, int);
AST_Node* lalr_peek_pn(lalr_ctx*, int);
void      lalr_pop(lalr_ctx*);
void      lalr_pop_n(lalr_ctx*, int);

void      lalr_print_node(AST_Node);

#endif
