#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H
#include "lib/sv.h"
#include <stdbool.h>

// Args:   where to save the parsed value
// Return: success state

typedef struct np_result {
	int status;
	int length;
} np_result;

typedef struct np_integer_result {
	long ok;
	np_result base;
} np_integer_result;

typedef struct np_integer_result np_decimal_result;
typedef struct np_integer_result np_hex_result; 

typedef struct np_double_result {
	double ok;
	np_result base;
} np_double_result;

typedef struct np_float_result {
	float ok;
	np_result base;
} np_float_result;

// For strlit and id
typedef struct np_str_result {
	String_View ok;
	np_result base;
} np_str_result;

bool  						number_parse_isvalidbase(char, int);
bool 							number_parse_ishexdigit(char);

np_integer_result number_parse_integer_base(const char*, int);

np_decimal_result number_parse_decimal(const char*);
np_hex_result     number_parse_hex(const char*);
np_float_result   number_parse_float(const char*);
np_double_result  number_parse_double(const char*);
np_str_result     number_parse_strlit(const char*); // i.e "Hello World"
np_str_result     number_parse_id(const char*);

#endif
