#ifndef LALR_H
#define LALR_H
#include "ast.h"
#include "tokenizer.h"
#include "lib/arena.h"

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

int       lalr_try_reduce_number(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_header(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_vartype(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_typed_id(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_typed_id_list(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_deref(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_addressof(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_factor(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_term(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_math_expr(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_relate(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_logical_conj(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_logical_disj(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_expr(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_return(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_expr_list(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_array_index(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_func_call(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_function_header(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_if(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_assignment(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_statement_list(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_statement(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_block(lalr_ctx*, AST_Node*);
int       lalr_try_reduce_function(lalr_ctx*, AST_Node*);

void      lalr_push(lalr_ctx*, AST_Node);
AST_Node  lalr_peek(lalr_ctx*, AST_Node);
AST_Node  lalr_peek_n(lalr_ctx*, int);
AST_Node* lalr_peek_pn(lalr_ctx*, int);
void      lalr_pop(lalr_ctx*);
void      lalr_pop_n(lalr_ctx*, int);

void      lalr_print_node(AST_Node);

#endif
