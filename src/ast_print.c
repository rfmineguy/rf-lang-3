#include "ast_print.h"
#include "ast.h"
#include "tokenizer.h"
#include <stdio.h>

#define TREE_FMT "%*c"
#define TREE_ARG(offset) (d + (offset)) * 2, ' '
#define LOC_FMT "{line: %4d, col: %3d}"
#define LOC_ARG(n) n.loc.line, n.loc.col

void ast_print_node(AST_Node n, int d) {
	switch (n.type) {
		case NT_UNDEF:           printf("UNDEFINED\n"); break;
		case NT_TOKEN:           printf(TREE_FMT "[Token] %s '" SV_Fmt "'\n", TREE_ARG(0), token_str(n.token.type), SV_Arg(n.token.text)); break;
		case NT_FACTOR:          ast_print_factor(n.factor, d); break;
		case NT_NUMBER:          ast_print_number(n.number, d); break;
		case NT_EXPRESSION:      ast_print_expr(n.expr, d); break;
		case NT_LOGIC_DISJ:      ast_print_logical_disj(n.logicdisj, d); break;
		case NT_LOGIC_CONJ:      ast_print_logical_conj(n.logicconj, d); break;
		case NT_RELATE:          ast_print_relational(n.relate, d); break;
		case NT_MATH_EXPR:       ast_print_math_expr(n.mathexpr, d); break;
		case NT_TERM: 			     ast_print_term(n.term, d); break;
		case NT_STATEMENT:       ast_print_stmt(n.stmt, d); break;
		case NT_EXPRESSION_LIST: ast_print_expr_list(n.exprList, d); break;
		case NT_FUNC_CALL:       ast_print_func_call(n.funcCall, d); break;
		case NT_HEADER: 				 ast_print_header(n.header, d); break;
		case NT_TYPED_ID:				 ast_print_typed_id(n.typed_id, d); break;
		case NT_DEREF:           ast_print_deref(n.deref, d); break;
		case NT_ARRAY_INDEX:  	 ast_print_array_index(n.arrayIndex, d); break;
		case NT_VAR_TYPE:        ast_print_vartype(n.var_type, d); break;
		case NT_TYPED_ID_LIST:   ast_print_typed_idlist(n.typed_idlist, d); break;
		case NT_FUNC_HEADER:     ast_print_function_header(n.funcHeader, d); break;
		case NT_BLOCK:           ast_print_block(n.block, d); break;
		case NT_IF: 						 ast_print_if(n.iff, d); break;
		case NT_STATEMENT_LIST:  ast_print_stmt_list(n.stmtList, d); break;
		case NT_FUNCTION:        ast_print_function(n.function, d); break;
		case NT_ASSIGNMENT:      ast_print_assignment(n.assign, d); break;
	}
}

void ast_print_header(Header h, int d) {
	printf(LOC_FMT TREE_FMT "[Header]\n", LOC_ARG(h), TREE_ARG(0));
	switch (h.type) {
		case HEADER_TYPE_MODULE:
			printf(LOC_FMT TREE_FMT "[Module " SV_Fmt "]", LOC_ARG(h), TREE_ARG(1), SV_Arg(h.module.name));
			break;
		case HEADER_TYPE_USE:
			assert(0 && "Header Use not implemented");
			break;
	}
}

void ast_print_factor(Factor f, int d) {
	printf(LOC_FMT TREE_FMT "[Factor]\n", LOC_ARG(f), TREE_ARG(0));
	// printf("%*c[Factor]\n", d * 2, ' ');
	switch (f.type) {
		case FACTOR_TYPE_UND:       printf(LOC_FMT TREE_FMT "[UNDEFINED]\n", LOC_ARG(f), TREE_ARG(1));                      break;
		case FACTOR_TYPE_ID:        printf(LOC_FMT TREE_FMT "[Id] " SV_Fmt "\n", LOC_ARG(f), TREE_ARG(1), SV_Arg(f.id));    break;
		case FACTOR_TYPE_STR:       printf(LOC_FMT TREE_FMT "[Str]  " SV_Fmt "\n", LOC_ARG(f), TREE_ARG(1), SV_Arg(f.str)); break;
		case FACTOR_TYPE_NUMBER:    ast_print_number(f.number, d + 1);  																break;
		case FACTOR_TYPE_EXPR:      ast_print_expr(f.expr, d + 1); 																			break; 
		case FACTOR_TYPE_LOGIC_DISJ:assert(0 && "No factor for logic disjunction");											break;
		case FACTOR_TYPE_LOGIC_CONJ:assert(0 && "No factor for logic conjunction"); 										break;
		case FACTOR_TYPE_FUNC_CALL: ast_print_func_call(f.funcCall, d + 1); 														break;
		case FACTOR_TYPE_ARRAY_INDEX:ast_print_array_index(f.arrayIndex, d + 1);  																	break;
	}
}

void ast_print_vartype(VarType* type, int d) {
	printf(LOC_FMT TREE_FMT "[VarType]\n", LOC_ARG((*type)), TREE_ARG(0));
	printf(LOC_FMT TREE_FMT "[ptrdepth=%d]\n", LOC_ARG((*type)), TREE_ARG(1), type->pointerDepth);
	switch (type->type) {
		case VAR_TYPE_ID:
			printf(LOC_FMT  TREE_FMT "[Id " SV_Fmt "]\n", LOC_ARG((*type)), TREE_ARG(1), SV_Arg(type->Id.id));
			break;
		case VAR_TYPE_ARRAY:
			printf(LOC_FMT TREE_FMT "[Id " SV_Fmt "]\n", LOC_ARG((*type)), TREE_ARG(1), SV_Arg(type->Array.id));
			ast_print_expr_list(type->Array.exprList, d + 1);
			break;
		case VAR_TYPE_ARRAY_NESTED:
			ast_print_vartype(type->nested, d + 1);
			ast_print_expr_list(type->Array.exprList, d + 1);
			break;
		case VAR_TYPE_NONE: 
			printf(LOC_FMT  TREE_FMT "None\n", LOC_ARG((*type)), TREE_ARG(0));
			break;
	}
}

void ast_print_typed_id(TypedId id, int d) {
	printf(LOC_FMT  TREE_FMT "[TypedId]\n", LOC_ARG(id), TREE_ARG(0));
	printf(LOC_FMT  TREE_FMT "[Id " SV_Fmt "]\n", LOC_ARG(id), TREE_ARG(1), SV_Arg(id.id));
	ast_print_vartype(id.type, d + 1);
}

void ast_print_typed_idlist_rec(TypedIdList* list, int i, int d, LocationInfo loc) {
	if (list == NULL) {
		printf(LOC_FMT TREE_FMT "[TypedIdList(NULL)]\n", loc.line, loc.col, TREE_ARG(0));
		return;
	}

	ast_print_typed_id(list->typedId, d + 1);
	ast_print_typed_idlist_rec(list->next, i + 1, d, list->loc);
}

void ast_print_typed_idlist(TypedIdList* list, int d) {
	if (list->typedId.type->type == VAR_TYPE_NONE) printf(LOC_FMT TREE_FMT "[TypedIdList] Empty\n", LOC_ARG((*list)), TREE_ARG(d));
	printf(LOC_FMT TREE_FMT "[TypedIdList]\n", LOC_ARG((*list)), TREE_ARG(0));
	ast_print_typed_idlist_rec(list, 0, d, list->loc);
}

void ast_print_deref(Deref* deref, int d) {
	printf(LOC_FMT TREE_FMT "[Deref]\n", LOC_ARG((*deref)), TREE_ARG(0));
	printf(LOC_FMT TREE_FMT "[depth = %d]\n",LOC_ARG((*deref)), TREE_ARG(1), deref->depth);
	switch (deref->type) {
		case DEREF_TYPE_DEREF:
			ast_print_factor(deref->f, d + 1);
			break;
		case DEREF_TYPE_FACTOR: 
			ast_print_factor(deref->f, d + 1);
			break;
	}
}

void ast_print_array_index(ArrayIndex* array, int d) {
	printf(LOC_FMT TREE_FMT "[ArrayIndex]\n", LOC_ARG((*array)), TREE_ARG(0));
	printf(LOC_FMT TREE_FMT SV_Fmt "\n", LOC_ARG((*array)), TREE_ARG(1), SV_Arg(array->Brkt.id));
	ast_print_expr_list(array->Brkt.exprList, d + 1);
}

void ast_print_number(Number n, int d) {
	printf(LOC_FMT TREE_FMT "[Number]\n", LOC_ARG(n), TREE_ARG(0));
	switch (n.type) {
		case NUMBER_TYPE_INT:    printf(LOC_FMT TREE_FMT "[Integer %d]\n", LOC_ARG(n), TREE_ARG(1), n.i); break;
		case NUMBER_TYPE_DOUBLE: printf(LOC_FMT TREE_FMT "[Double %0.4f]\n", LOC_ARG(n), TREE_ARG(1), n.d); break;
		case NUMBER_TYPE_FLOAT:  printf(LOC_FMT TREE_FMT "[Float %0.4f]\n", LOC_ARG(n), TREE_ARG(1), n.f); break;
	}
}
void ast_print_logical_disj(LogicalDisj* l, int d) {
	if (l == NULL)
		return;
	ast_print_logical_disj(l->disj, d + 1);
	printf(LOC_FMT TREE_FMT "[LogicalDisj]\n", LOC_ARG((*l)), TREE_ARG(0));
	ast_print_logical_conj(l->conj, d + 1);
}
void ast_print_logical_conj(LogicalConj* l, int d) {
	if (l == NULL)
		return;
	switch (l->type) {
		case LOGICAL_CONJ_TYPE_CONJ_RELATE:
			printf(LOC_FMT TREE_FMT "[LogicalConj &&]\n", LOC_ARG((*l)), TREE_ARG(0));
			ast_print_logical_conj(l->conj, d + 1);
			ast_print_relational(l->relate, d + 1);
			break;
		case LOGICAL_CONJ_TYPE_RELATE:
			printf(LOC_FMT TREE_FMT "[LogicalConj Relate]\n", LOC_ARG((*l)), TREE_ARG(0));
			ast_print_relational(l->relate, d + 1);
			break;
	}
}
void ast_print_relational(Relational* r, int d) {
	if (r == NULL)
		return;
	switch (r->type) {
		case RELATIONAL_TYPE_MATH_EXPR:
			printf(LOC_FMT TREE_FMT "[Relational MathExpr]\n", LOC_ARG((*r)), TREE_ARG(0));
			ast_print_math_expr(r->mathexpr, d + 1);
			break;
		case RELATIONAL_TYPE_GT...RELATIONAL_TYPE_DEQ:
			printf(LOC_FMT TREE_FMT "[Relational " SV_Fmt "]\n", LOC_ARG((*r)), TREE_ARG(0), SV_Arg(r->op));
			ast_print_relational(r->relate, d + 1);
			ast_print_math_expr(r->mathexpr, d + 1);
			break;
	}
}
void ast_print_expr(Expression* e, int d) {
	printf(LOC_FMT TREE_FMT "[Expression]\n", LOC_ARG((*e)), TREE_ARG(0));
	switch (e->type) {
		case EXPRESSION_TYPE_LOGIC_DISJ:
			ast_print_logical_disj(e->disj, d + 1);
			break;
	}
}
void ast_print_math_expr(MathExpression* m, int d) {
	if (!m) return;
	printf(LOC_FMT TREE_FMT "[MathExpression %c]\n", LOC_ARG((*m)), TREE_ARG(0), m ? m->op : '_');
	ast_print_math_expr(m->left, d + 1);
	ast_print_term(m->right, d + 1);
}
void ast_print_term(Term* t, int d) {
	printf(LOC_FMT TREE_FMT "[Term %c]\n", LOC_ARG((*t)), TREE_ARG(0), t->op);
	if (t == NULL) {
		printf("Null\n");
		return;
	}
	switch (t->type) {
		case TERM_TYPE_DEREF: 
			ast_print_deref(t->right, d + 1);
			break;
		case TERM_TYPE_TERM_OP_DEREF: 
			ast_print_term(t->left, d + 1);
			ast_print_deref(t->right, d + 1);
			break;
	}
}

void ast_print_block(Block b, int d) {
	printf(LOC_FMT TREE_FMT "[Block]\n", LOC_ARG(b), TREE_ARG(0));
	if (b.stmts)
		ast_print_stmt_list(b.stmts, d + 1);
	else
		printf(LOC_FMT TREE_FMT "[StatementList] Empty", LOC_ARG(b), TREE_ARG(1));
}

void ast_print_stmt_list_rec(StatementList* stmt, int d, int i) {
	if (stmt == NULL) {
		// printf(LOC_FMT TREE_FMT "StatmentList(NULL)\n", 0, 0, TREE_ARG(0));
		return;
	}

	ast_print_stmt(stmt->stmt, d + 1);
	ast_print_stmt_list_rec(stmt->next, d, i + 1);
}

void ast_print_stmt_list(StatementList* stmts, int d) {
	printf(LOC_FMT TREE_FMT "[StatementList]\n", LOC_ARG((*stmts)), TREE_ARG(0));
	ast_print_stmt_list_rec(stmts, d, 0);
}

void ast_print_stmt(Statement stmt, int d) {
	printf(LOC_FMT TREE_FMT "[Statement]\n", LOC_ARG(stmt), TREE_ARG(0));
	switch (stmt.type) {
		case STATEMENT_TYPE_RETURN:
			printf(LOC_FMT TREE_FMT "[Return]\n", LOC_ARG(stmt), TREE_ARG(1));
			ast_print_expr(stmt.Return.expr, d + 2);
			break;
		case STATEMENT_TYPE_IF:
			ast_print_if(stmt.iff, d + 1);
			break;
		case STATEMENT_TYPE_ASSIGN:
			ast_print_assignment(stmt.assign, d + 1);
			break;
		case STATEMENT_TYPE_EXPR:
			ast_print_expr(stmt.expr, d + 1);
			break;
	}
}

void ast_print_if(IfStatement iff, int d) {
	printf(LOC_FMT TREE_FMT "[If]\n", LOC_ARG(iff), TREE_ARG(0));
	ast_print_expr(iff.expr, d + 1);
	ast_print_block(iff.block, d + 1);
}

void ast_print_assignment(AssignStatement assign, int d) {
	printf(LOC_FMT TREE_FMT "[Assign]\n", LOC_ARG(assign), TREE_ARG(0));
	switch (assign.type) {
		case ASSIGN_TYPE_UNTYPED_ID:
			printf(LOC_FMT TREE_FMT "[UntypedId " SV_Fmt "]\n", LOC_ARG(assign), TREE_ARG(1), SV_Arg(assign.untypedId));
			ast_print_expr(assign.expr, d + 1);
			break;
		case ASSIGN_TYPE_TYPED_ID:
			ast_print_typed_id(assign.typedId, d + 1);
			ast_print_expr(assign.expr, d + 1);
			break;
	}
}

void ast_print_expr_list_rec(ExpressionList* eList, int d, int index) {
	if (eList == NULL) {
		return;
	}

	// printf(LOC_FMT "%d:", LOC_ARG(), index);
	ast_print_expr(eList->expr, d + 1);
	ast_print_expr_list_rec(eList->next, d, index + 1);
}

void ast_print_expr_list(ExpressionList * eList, int d) {
	if (eList) {
		printf(LOC_FMT TREE_FMT "[ExpressionList]\n", LOC_ARG((*eList)), TREE_ARG(0));
		ast_print_expr_list_rec(eList, d, 0);
	}
}

void ast_print_func_call(FuncCall funcCall, int d) {
	printf(LOC_FMT TREE_FMT "[FuncCall]\n", LOC_ARG(funcCall), TREE_ARG(0));
	printf(LOC_FMT TREE_FMT "[Id " SV_Fmt "]\n", LOC_ARG(funcCall), TREE_ARG(1), SV_Arg(funcCall.id));
	ast_print_expr_list(funcCall.exprList, d + 1);
}

void ast_print_function_header(FunctionHeader header, int d) {
	printf(LOC_FMT TREE_FMT "[FunctionHeader]\n", LOC_ARG(header), TREE_ARG(0));
	ast_print_typed_idlist(header.params, d + 1);
	ast_print_vartype(header.returnType, d + 1);
}

void ast_print_function(Function function, int d) {
	printf(LOC_FMT TREE_FMT "[Function]\n", LOC_ARG(function), TREE_ARG(0));
	printf(LOC_FMT TREE_FMT "[Id " SV_Fmt "]\n", LOC_ARG(function), TREE_ARG(1), SV_Arg(function.id));
	ast_print_function_header(function.header, d + 1);
	ast_print_block(function.block, d + 1);
}
