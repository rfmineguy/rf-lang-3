#ifndef AST_H
#define AST_H

#include "tokenizer.h"

typedef struct Header Header;
typedef enum {
	HEADER_TYPE_MODULE, HEADER_TYPE_USE
} HeaderType;

typedef struct TypedId TypedId;
typedef struct TypedIdList TypedIdList;
typedef enum {
	TYPED_ID_TYPE_UNUSED
} TypedIdListType;

typedef struct FunctionHeader FunctionHeader;
typedef struct Function Function;

typedef struct VarType VarType;
typedef enum {
	VAR_TYPE_ID, VAR_TYPE_ARRAY, VAR_TYPE_NONE
} VarTypeType;

typedef struct Deref Deref;
typedef enum {
	DEREF_TYPE_BRKT, DEREF_TYPE_ASTERISK
} DerefType;

typedef struct Factor Factor;
typedef enum {
	FACTOR_TYPE_UND, FACTOR_TYPE_ID, FACTOR_TYPE_STR, FACTOR_TYPE_NUMBER, FACTOR_TYPE_EXPR, FACTOR_TYPE_LOGIC_CONJ, FACTOR_TYPE_LOGIC_DISJ, FACTOR_TYPE_FUNC_CALL, FACTOR_TYPE_DEREF 
} FactorType;

typedef struct Number Number;
typedef enum {
	NUMBER_TYPE_INT, NUMBER_TYPE_FLOAT, NUMBER_TYPE_DOUBLE
} NumberType;

typedef struct Expression Expression;
typedef enum {
	EXPRESSION_TYPE_LOGIC_DISJ
} ExpressionType;

typedef struct ExpressionList ExpressionList;
typedef enum {
	EXPRESSION_LIST_TYPE_EXPR, 
	EXPRESSION_LIST_TYPE_EXPR_LIST_EXPR
} ExpressionListType;

typedef struct LogicalDisj LogicalDisj;
typedef enum LogicalDisjType {
	LOGICAL_DISJ_TYPE_CONJ,
	LOGICAL_DISJ_TYPE_DISJ_CONJ
} LogicalDisjType;

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

typedef struct Statement Statement;
typedef enum {
	STATEMENT_TYPE_RETURN, STATEMENT_TYPE_IF
} StatementType;

typedef struct StatementList StatementList;
typedef enum {
	STATEMENT_LIST_TYPE_STMT_LIST_STMT,
	STATEMENT_LIST_TYPE_STMT
} StatementListType;

typedef struct IfStatement IfStatement;
typedef struct Block Block;

typedef struct FuncCall FuncCall;
typedef enum {
	FUNC_CALL_TYPE_GENERIC
} FuncCallType;

typedef struct AST_Node AST_Node;
typedef enum AST_NodeType {
	NT_UNDEF, NT_TOKEN, NT_FACTOR, NT_NUMBER, NT_EXPRESSION, NT_EXPRESSION_LIST, NT_STATEMENT, NT_LOGIC_DISJ, NT_LOGIC_CONJ, NT_RELATE, NT_MATH_EXPR, NT_TERM, NT_FUNC_CALL, NT_HEADER, NT_TYPED_ID, NT_DEREF, NT_VAR_TYPE, NT_TYPED_ID_LIST, NT_FUNC_HEADER, NT_IF, NT_BLOCK, NT_STATEMENT_LIST, NT_FUNCTION
} AST_NodeType;

struct Header {
	HeaderType type;
	union {
		struct {
			String_View name;
		} module;
	};
};
struct Number {
	NumberType type;
	union {
		int i;
		double d;
		float f;
	};
};
struct VarType {
	VarTypeType type;
	union {
		struct {
			String_View id;
		} Id;
		struct {
			String_View id;
			ExpressionList* exprList;
		} Array;
	};
};
struct TypedId {
	String_View id;
	VarType type;
};
struct TypedIdList {
	TypedId typedId;
	TypedIdList* next;
};
struct FunctionHeader {
	TypedIdList* params;
	VarType returnType;
};

struct Block {
	StatementList* stmts;
};
struct Function {
	String_View id;
	FunctionHeader header;
	Block block;
};
struct FuncCall {
	String_View id;
	ExpressionList* exprList;
};

struct Deref {
	DerefType type;
	union {
		struct {
			String_View id;
			ExpressionList* exprList;
		} Brkt;
	};
};
struct Factor {
	FactorType type;
	union {
		String_View id;
		String_View str;
		Number number;
		Expression* expr;
		LogicalDisj* logicdisj;
		FuncCall funcCall;
		Deref deref;
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
	ExpressionType type;
	LogicalDisj* disj;
	FuncCall funcCall;
};

struct ExpressionList {
	ExpressionListType type;
	Expression* expr;
	ExpressionList* next;
};

struct LogicalConj {
	LogicalConjType type;
	LogicalConj* conj;
	Relational* relate;
};

struct LogicalDisj {
	LogicalDisjType type;
	LogicalDisj* disj;
	LogicalConj* conj;
};

struct IfStatement {
	Expression* expr;
	Block block;
};

struct Statement {
	StatementType type;
	union {
		struct {
			Expression* expr;
		} Return;
		IfStatement iff;
	};
};

struct StatementList {
	StatementListType type;
	Statement stmt;
	StatementList* next;
};

struct AST_Node {
	AST_NodeType type;
	union {
		VarType var_type;
		token token;
		Factor factor;
		Number number;
		Expression* expr;
		ExpressionList* exprList;
		LogicalDisj* logicdisj;
		LogicalConj* logicconj;
		Relational* relate;
		MathExpression* mathexpr;
		Term* term;
		Block block;
		Statement stmt;
		StatementList* stmtList;
		IfStatement iff;
		FuncCall funcCall;
		FunctionHeader funcHeader;
		Function function;
		Header header;
		TypedId typed_id;
		TypedIdList* typed_idlist;
		Deref deref;
	};
};

#endif
