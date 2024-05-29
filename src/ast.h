#ifndef AST_H
#define AST_H

#include "sv.h"
#include "tokenizer.h"

typedef struct Factor Factor;
typedef enum {
	FACTOR_TYPE_UND, FACTOR_TYPE_ID, FACTOR_TYPE_STR, FACTOR_TYPE_NUMBER, FACTOR_TYPE_EXPR, FACTOR_TYPE_LOGIC_CONJ 
} FactorType;

typedef struct Number Number;
typedef enum {
	NUMBER_TYPE_INT, NUMBER_TYPE_FLOAT, NUMBER_TYPE_DOUBLE
} NumberType;

typedef struct Expression Expression;

typedef struct LogicalDisj LogicalDisj;

typedef struct LogicalConj LogicalConj;
typedef enum LogicalConjType {
	LOGICAL_CONJ_TYPE_RELATE,
	LOGICAL_CONJ_TYPE_CONJ_RELATE
} LogicalConjType;

typedef struct Relational Relational;
typedef enum {
	RELATIONAL_TYPE_MATH_EXPR,
	RELATIONAL_TYPE_GT, RELATIONAL_TYPE_GTEQ,
	RELATIONAL_TYPE_LT, RELATIONAL_TYPE_LTEQ,
	RELATIONAL_TYPE_DEQ,
} RelationalType;

typedef struct MathExpression MathExpression;
typedef enum {
	MATH_EXPR_TYPE_ADD, MATH_EXPR_TYPE_SUB, MATH_EXPR_TYPE_TERM
} MathExpressionType;

typedef struct Term Term;
typedef enum {
	TERM_TYPE_FACTOR, TERM_TYPE_TERM_OP_FACTOR
} TermType;

typedef struct AST_Node AST_Node;
typedef enum AST_NodeType {
	NT_UNDEF, NT_TOKEN, NT_FACTOR, NT_NUMBER, NT_EXPR, NT_LOGIC_DISJ, NT_LOGIC_CONJ, NT_RELATE, NT_MATH_EXPR, NT_TERM
} AST_NodeType;


struct Number {
	NumberType type;
	union {
		int i;
		double d;
		float f;
	};
};

struct Factor {
	FactorType type;
	union {
		String_View id;
		String_View str;
		Number number;
		Expression* expr;
		LogicalConj* logical_test;
	};
};

struct Term {
	TermType type;
	Term* left;
	Factor right;
	char op;
};

struct MathExpression {
	MathExpressionType type;

	MathExpression* left;
	Term* right;
	char op;
};

struct Relational {
	RelationalType type;
	String_View op;
	Relational* relate;
	MathExpression* mathexpr;
};

struct Expression {
	LogicalDisj* disj;
};

struct LogicalConj {
	LogicalConjType type;
	LogicalConj* conj;
	Relational* relate;
};

struct LogicalDisj {
	LogicalDisj* disj;
	LogicalConj* conj;
};

struct AST_Node {
	AST_NodeType type;
	union {
		token token;
		Factor factor;
		Number number;
		Expression* expr;
		LogicalDisj logicdisj;
		LogicalConj* logicconj;
		Relational* relate;
		MathExpression* mathexpr;
		Term* term;
	};
};

#endif
