
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
BIN          := rfc

.PHONY: always clean build debug
.PHONY: build-test build-project
build: always build-project build-test
build-project: always out/$(BIN)
build-test: always out/test
always:
	mkdir -p out
clean:
	rm -r out
		printf("[%d]: %d -> %d -> %ul\n", i, digit, length - i - 1, digit * (int)pow(10, length - i - 1));
debug: always
	docker run --rm -it -v $(shell pwd):$(shell pwd) -w $(shell pwd) alpine sh -c "gcc $(SOURCES) -ggdb -lm -o out/$(BIN)_x86"
	docker run --rm -it -e DISPLAY=192.168.1.142:0 -v $(shell pwd):$(shell pwd) -w $(shell pwd) alpine gf2 ./out/$(BIN)_x86
out/$(BIN): $(SOURCES)
	gcc $^ -o $@ -lm -ggdb
out/test: $(TEST_SOURCES)
	gcc $^ -o $@ -lm
