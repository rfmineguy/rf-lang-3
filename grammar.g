id     := [a-zA-Z][a-zA-Z0-9]+
strlit := ".*"
declit := [0-9]+
hexlit := 0x

number 					 := <declit>
									| <hexlit>

module_header    := <id="module"> <id>
use_header       := <id="use"> <id> "{" <idlist> "}"
idlist           := <id> | <idlist> "," <id>

vartype          := <id>
									| <id="_">
									| "[" <id> ";" <expression_list> "]"
									| "[" <vartype> ";" <expression_list> "]"
									| "*" <vartype>

typed_id         := <id> ":" <var_type>
typed_id_list    := <typed_id> | <typed_id_list> "," <typed_id>

generic     		 := "<" <id> ">"
function_header  := <typed_id_list> "->" <vartype>
									| <vartype>
function         := <id> { <generic> } "=" { <function_header> } <block>
block 					 := "{" <statements> "}"
statements       := <statements> <statement>
									| <statement>
statement        := <assignment>
									| <return>
									| <if>
									| <for>
									| <while>
									| <switch>
									| <expression>

expression       := <logical_disj>

logical_disj     := <logical_disj> "||" <logical_conj>
								  | <logical_conj>

logical_conj     := <logical_conj> "&&" <relational>
									| <relational>

relational       := <relational> ">=" <math_expression>
									| <relational> "<=" <math_expression>
									| <relational> ">"  <math_expression>
									| <relational> "<"  <math_expression>
									| <relational> "==" <math_expression>
									| <math_expression>

math_expression  := <math_expression> "+" <term>
									| <math_expression> "-" <term>
									| <term>

term 						 := <term> "*" <deref>
									| <term> "/" <deref>
									| <term> "%" <deref>
									| <deref>

deref            := "*" <deref> // if there is any operator preceding the "*"
									| <factor>

factor 					 := "(" <expression> ")"
									| <func_call>
									| <array_index>
									| <number>
									| <strlit>
									| <id>

func_call        := <id> "(" <expression_list> ")"
array_index      := <id> "[" <expression_list> "]"
expression_list  := <expression_list> "," <expression>
									| <expression>

assignment 			 := { <typed_id> | <id> } "=" <expression>
return 					 := "return" <expression>
if 							 := "if" <expression> <block>
for              := "for" <assignment> ";" <expression> ";" <expression> <block>
