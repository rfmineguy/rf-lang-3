#ifndef CODEGEN_TPLT_H
#define CODEGEN_TPLT_H

#define JOIN2(a, b) a ## b

#define codegen_entry_point(arch,name,...) JOIN2(codegen_entry_, arch)(AST_Node name, ##__VA_ARGS__)

#define codegen_header(arch,name,...)   JOIN2(codegen_header_, arch)(Header name, ##__VA_ARGS__)
#define codegen_assign(arch,name,...)   JOIN2(codegen_assign_, arch)(AssignStatement name, ##__VA_ARGS__)
#define codegen_expr(arch,name,...)     JOIN2(codegen_expr_, arch)(Expression name, ##__VA_ARGS__)
#define codegen_stmt(arch,name,...)     JOIN2(codegen_stmt_, arch)(Statement name, ##__VA_ARGS__)
#define codegen_if_stmt(arch,name,...)  JOIN2(codegen_if_stmt_, arch)(IfStatement name, ##__VA_ARGS__)
#define codegen_function(arch,name,...) JOIN2(codegen_function_, arch)(Function name, ##__VA_ARGS__)

#endif
