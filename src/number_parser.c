#include "number_parser.h"
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define ERR(type) (type) {.base.status=0}
#define OK(type, ok_, length) (type) {.base.status=1, .ok=ok_, .base.length=length}

bool number_parse_isvalidbase(char c, int base) {
	c = tolower(c);
	if (isdigit(c) && c - '0' >= base) return false;
	if (c - '0' < 0) return false;
	if (isalpha(c)) {
		int value = c - 'a' + 10;
		if (value >= base) return false;
	}
	return true;
}

np_integer_result number_parse_integer_base(const char* cursor, int base) {
	int sign = 1;
	long v = 0;
	int length = 0;
	if (*cursor == '-') { sign = -1; cursor++; }
	const char* c = cursor;
	while (*c && number_parse_isvalidbase(*c, base) && !isspace(*c)) { c++; length++; }

	for (int i = 0; i < length; i++) {
		int digit = *(cursor + i) - '0';
		if (digit >= 0 && digit <= 9) {
			v += digit * pow(base, length - i - 1);
		}
		else {
			digit = *(cursor + i) - 'a' + 10;
			v += digit * pow(base, length - i - 1);
		}
	}
	return OK(np_integer_result, v * sign, length);
}

/**
 *   4598 = (4 * 1000) + (5 * 100) + (9 * 10) + (8 * 1)
 */
np_decimal_result number_parse_decimal(const char* cursor){
	return number_parse_integer_base(cursor, 10);
}

/*
 *  4a = 4 * 16 + 10 * 1 = 64 + 10
 */
np_hex_result number_parse_hex(const char* cursor){
	bool startokis0x = (*cursor == '0' && *(cursor + 1) == 'x');
	if (startokis0x)
		cursor += 2;
	return number_parse_integer_base(cursor, 16);
}

np_float_result number_parse_float(const char* cursor){
	return (np_float_result) {.base.status=0};
}

np_double_result  number_parse_double(const char* cursor){
	return (np_double_result) {.base.status=0};
}

np_str_result     number_parse_strlit(const char* cursor){
	return (np_str_result) {.base.status=0};
}

np_str_result     texnparse_id(const char* cursor){
	return (np_str_result) {.base.status=0};
}
