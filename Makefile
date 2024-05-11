
MAIN := src/main.c
TEST_MAIN := tests/test_main.c
SOURCES 	:= $(MAIN)\
						 src/tokenizer.c \
						 src/svimpl.c\
						 src/number_parser.c\
						 src/lalr.c\
						 src/ast_util.c
TEST_SOURCES := $(TEST_MAIN)\
						 src/tokenizer.c \
						 src/svimpl.c\
						 src/lalr.c\
						 src/ast_util.c\
						 src/number_parser.c\
						 tests/munit.c

.PHONY: always clean build
.PHONY: build-test build-project
build: always build-project build-test
build-project: out/main
build-test: out/test
always:
	mkdir -p out
clean:
	rm -r out
		printf("[%d]: %d -> %d -> %ul\n", i, digit, length - i - 1, digit * (int)pow(10, length - i - 1));
out/main: $(SOURCES)
	gcc $^ -o $@ -lm -ggdb
out/test: $(TEST_SOURCES)
	gcc $^ -o $@ -lm
