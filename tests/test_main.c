#include "munit.h"
#include <stdio.h>
#include "../src/number_parser.h"

static MunitResult validbase(const MunitParameter params[], void* fixture);
static MunitResult base_n_parsing(const MunitParameter params[], void* fixture);

MunitTest tests[] = {
	{ "/valid-base", validbase, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "/base_n_parsing", base_n_parsing, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ NULL,     NULL,  NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
	"/lalr-tests", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, const char* argv[]) {
	return munit_suite_main(&suite, NULL, argc, (char* const*)argv);
}

MunitResult validbase(const MunitParameter params[], void* fixture) {
	// base 16
	const char* hexdigits = "0123456789abcdefABCDEF";
	int length = strlen(hexdigits);
	for (int i = 0; i < length; i++) {
		munit_assert_true(number_parse_isvalidbase(hexdigits[i], 16));
	}
	// base 10
	const char* decimaldigits = "0123456789";
	length = strlen(decimaldigits);
	for (int i = 0; i < length; i++) {
		munit_assert_true(number_parse_isvalidbase(decimaldigits[i], 10));
	}
	// base 13
	munit_assert_false(number_parse_isvalidbase('d', 13));
	return MUNIT_OK;
}

MunitResult base_n_parsing(const MunitParameter params[], void* fixture) {
	// Convert from base n to base 10
	munit_assert_long(number_parse_integer_base("011001", 2).ok,  ==, 25);
	munit_assert_long(number_parse_integer_base("011001", 3).ok,  ==, 109);
	munit_assert_long(number_parse_integer_base("011001", 4).ok,  ==, 321);
	munit_assert_long(number_parse_integer_base("011001", 5).ok,  ==, 751);
	munit_assert_long(number_parse_integer_base("011001", 6).ok,  ==, 1513);
	munit_assert_long(number_parse_integer_base("011001", 7).ok,  ==, 2745);
	munit_assert_long(number_parse_integer_base("011001", 8).ok,  ==, 4609);
	munit_assert_long(number_parse_integer_base("011001", 9).ok,  ==, 7291);
	munit_assert_long(number_parse_integer_base("011001", 10).ok, ==, 11001);
	munit_assert_long(number_parse_integer_base("011001", 11).ok, ==, 15973);
	munit_assert_long(number_parse_integer_base("011001", 12).ok, ==, 22465);
	munit_assert_long(number_parse_integer_base("011001", 13).ok, ==, 30759);
	munit_assert_long(number_parse_integer_base("011001", 14).ok, ==, 41161);

	// Hexadecimal parsing
	munit_assert_long(number_parse_hex("0x456").ok, ==, 1110);
	munit_assert_long(number_parse_hex("0xfffffffe").ok, ==, 4294967294);
	munit_assert_long(number_parse_hex("0xffffffff").ok, ==, 4294967295);
	return MUNIT_OK;
}
