MAIN := src/main.c
TEST_MAIN := tests/test_main.c
GEN_SOURCES  := src/tokenizer.c \
						 src/svimpl.c\
						 src/arenaimpl.c\
						 src/number_parser.c\
						 src/lalr.c\
						 src/ast_util.c
MAIN_SOURCES := $(MAIN)\
								$(GEN_SOURCES)
TEST_SOURCES := $(TEST_MAIN)\
								$(GEN_SOURCES)\
								tests/munit.c
GETOPT_SOURCE := getopt/cmdline.c
BIN          := rfc
# CFLAGS       := -ggdb
CFLAGS       :=

.PHONY: always clean build debug
.PHONY: build-test build-project build-getopt
.PHONY: install

# =====================
# PHONY targets
# =====================
build: always build-getopt build-project
build-project: always out/$(BIN)
build-test: always out/test
build-getopt:
	gengetopt < config.ggo
	mv cmdline.* getopt
always:
	mkdir -p getopt
	mkdir -p out
clean:
	rm -r out
debug: always
	docker run --rm -it -v $(shell pwd):$(shell pwd) -w $(shell pwd) alpine sh -c "gcc $(MAIN_SOURCES) $(GETOPT_SOURCE) -ggdb -lm -o out/$(BIN)_x86"
	docker run --rm -it -e DISPLAY=192.168.1.142:0 -v $(shell pwd):$(shell pwd) -w $(shell pwd) alpine gf2 ./out/$(BIN)_x86

# =====================
# BUILD targets
# =====================
out/$(BIN): $(MAIN_SOURCES) $(GETOPT_SOURCE)
	clang $^ -o $@ -lm $(CFLAGS)
out/test: $(TEST_SOURCES) $(GETOPT_SOURCE)
	clang $^ -o $@ -lm $(CFLAGS)

# =====================
# INSTALL targets
# =====================
install:
ifdef INSTALL_DIR
	install -m 557 out/$(BIN) $(INSTALL_DIR)
else
	install -m 557 out/$(BIN) /usr/local/bin
endif
