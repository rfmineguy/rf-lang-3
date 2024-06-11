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
	VAR_TYPE_ID, VAR_TYPE_ARRAY, VAR_TYPE_NESTED, VAR_TYPE_NONE
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
	STATEMENT_TYPE_RETURN, STATEMENT_TYPE_IF, STATEMENT_TYPE_ASSIGN, STATEMENT_TYPE_EXPR
} StatementType;

typedef struct StatementList StatementList;
typedef enum {
	STATEMENT_LIST_TYPE_STMT_LIST_STMT,
	STATEMENT_LIST_TYPE_STMT
} StatementListType;

typedef struct IfStatement IfStatement;
typedef struct AssignStatement AssignStatement;
typedef enum {
	ASSIGN_TYPE_UNTYPED_ID,
	ASSIGN_TYPE_TYPED_ID
} AssignStatementType;
typedef struct Block Block;

typedef struct FuncCall FuncCall;
typedef enum {
	FUNC_CALL_TYPE_GENERIC
} FuncCallType;

typedef struct AST_Node AST_Node;
typedef enum AST_NodeType {
	NT_UNDEF, NT_TOKEN, NT_FACTOR, NT_NUMBER, NT_EXPRESSION, NT_EXPRESSION_LIST, NT_STATEMENT, NT_LOGIC_DISJ, NT_LOGIC_CONJ, NT_RELATE, NT_MATH_EXPR, NT_TERM, NT_FUNC_CALL, NT_HEADER, NT_TYPED_ID, NT_DEREF, NT_VAR_TYPE, NT_TYPED_ID_LIST, NT_FUNC_HEADER, NT_IF, NT_BLOCK, NT_STATEMENT_LIST, NT_FUNCTION, NT_ASSIGNMENT
} AST_NodeType;

/* header
 *  module_header    := <id="module"> <id>
 *  use_header       := <id="use"> <id> "{" <idlist> "}"
 */
struct Header {
	HeaderType type;
	LocationInfo loc;
	union {
		struct {
			String_View name;
		} module;
	};
};

struct Number {
	NumberType type;
	LocationInfo loc;
	union {
		int i;
		double d;
		float f;
	};
};

/* vartype
 * vartype          := <id>
 * 									| <id="_">
 * 									| "[" <id> ";" <expression_list> "]"
 * 									| "[" <vartype> ";" <expression_list> "]"
 * 									| <vartype> "*"
 */
struct VarType {
	VarTypeType type;
	LocationInfo loc;
	union {
		struct {
			String_View id;
		} Id;
		struct {
			String_View id;
			ExpressionList* exprList;
		} Array;
	};
	VarType* nested;
	int pointerDepth;
};

/* typed_id
 *	typed_id         := <id> ":" <var_type>
 */
struct TypedId {
	String_View id;
	VarType type;
	LocationInfo loc;
};

/* typedid_list
 *	typed_id_list    := <typed_id> 
 *	                  | <typed_id_list> "," <typed_id>
 */
struct TypedIdList {
	TypedId typedId;
	TypedIdList* next;
	LocationInfo loc;
};

/* function_header
 *	function_header  := <typed_id_list> "->" <vartype>
 *										| <vartype>
 */
struct FunctionHeader {
	TypedIdList* params;
	VarType returnType;
	LocationInfo loc;
};

/* block
 *  block := '{' <statement_list> '}'
 */
struct Block {
	StatementList* stmts;
	LocationInfo loc;
};

/* function
 *    function         := <id> { <generic> } "=" { <function_header> } <block>
 */
struct Function {
	String_View id;
	FunctionHeader header;
	Block block;
	LocationInfo loc;
};

/* func_call
 * func_call        := <id> "(" <expression_list> ")"
 */
struct FuncCall {
	String_View id;
	ExpressionList* exprList;
	LocationInfo loc;
};

/* deref
 *   deref := <id> "[" <expression_list> "]"
 */
struct Deref {
	DerefType type;
	union {
		struct {
			String_View id;
			ExpressionList* exprList;
		} Brkt;
	};
	LocationInfo loc;
};

/* factor
 * factor := "(" <expression> ")"
 *  			 | <func_call>
 *  			 | <deref>
 *  			 | <number>
 *  			 | <strlit>
 *  			 | <id>
 */
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
	LocationInfo loc;
};

/* term
 * term		 := <term> "*" <factor>
 * 					| <term> "/" <factor>
 * 					| <term> "%" <factor>
 * 					| <factor>
 */
struct Term {
	TermType type;
	Term* left;
	Factor right;
	char op;
	LocationInfo loc;
};

/* math_expression
 * math_expression  := <math_expression> "+" <term>
 *  		  					 | <math_expression> "-" <term>
 *	    						 | <term>
 *
 */
struct MathExpression {
	MathExpressionType type;

	MathExpression* left;
	Term* right;
	char op;
	LocationInfo loc;
};

/* relational
 * relational       := <relational> ">=" <math_expression>
 *									| <relational> "<=" <math_expression>
 *									| <relational> ">"  <math_expression>
 *									| <relational> "<"  <math_expression>
 *									| <relational> "==" <math_expression>
 *									| <math_expression>
 */
struct Relational {
	RelationalType type;
	String_View op;
	Relational* relate;
	MathExpression* mathexpr;
	LocationInfo loc;
};

/* expression
 * expression       := <logical_disj>
*/
struct Expression {
	ExpressionType type;
	LogicalDisj* disj;
	LocationInfo loc;
};

/* expression_list
 * expression_list  := <expression_list> "," <expression>
 *  		  					 | <expression>
 */
struct ExpressionList {
	ExpressionListType type;
	Expression* expr;
	ExpressionList* next;
	LocationInfo loc;
};

/* logical_disj
 * logical_disj     := <logical_disj> "||" <logical_conj>
 * 								  | <logical_conj>
 */
struct LogicalConj {
	LogicalConjType type;
	LogicalConj* conj;
	Relational* relate;
	LocationInfo loc;
};

/* logical_conj
 * logical_conj     := <logical_conj> "&&" <relational>
 *									| <relational>
 */
struct LogicalDisj {
	LogicalDisjType type;
	LogicalDisj* disj;
	LogicalConj* conj;
	LocationInfo loc;
};

/* if_stmt
 * if 							 := "if" <expression> <block>
 */
struct IfStatement {
	Expression* expr;
	Block block;
	LocationInfo loc;
};

/* assignment
 *		assignment 			 := { <typed_id> | <id> } "=" <expression>
 */
struct AssignStatement {
	AssignStatementType type;
	union {
		TypedId typedId;
		String_View untypedId;
	};
	Expression* expr;
	LocationInfo loc;
};

/* statement
 * statement        := <assignment>
 * 									| <return>
 * 									| <if>
 * 									| <for>
 * 									| <while>
 * 									| <switch>
 * 									| <expression>
 */
struct Statement {
	StatementType type;
	union {
		struct {
			Expression* expr;
		} Return;
		IfStatement iff;
		AssignStatement assign;
		Expression* expr;
	};
	LocationInfo loc;
};

/* statements
 * statements        := <statements> <statement>
 *  									| <statement>
 */
struct StatementList {
	StatementListType type;
	Statement stmt;
	StatementList* next;
	LocationInfo loc;
};

struct AST_Node {
	AST_NodeType type;
	LocationInfo loc;
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
		AssignStatement assign;
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
