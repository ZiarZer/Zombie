SRC=src/main.c src/*/*.c
CC=gcc
CFLAGS=-Isrc/cli -Isrc/execution -Isrc/parse -Isrc/utils -Wall -Werror -Wextra -std=c99 -pedantic
LDFLAGS=-g -fsanitize=address
TESTS=tests/add.bf tests/comments.bf tests/hello_world.bf tests/mul.bf tests/nine.bf
BIN=zombie

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

debug: $(BIN)_debug

$(BIN)_debug:
	$(CC) $(CFLAGS) -o $(BIN)_debug $(SRC) $(LDFLAGS)

check: $(TESTS) $(BIN)_debug tests/check.sh
	@./tests/check.sh

format:
	clang-format -i ${SRC} src/*/*.h

clean:
	$(RM) $(BIN) $(BIN)_debug tests/stderr tests/stdout
