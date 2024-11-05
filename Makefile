SRC=src/*.c
CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c99 -pedantic
LDFLAGS=-g -fsanitize=address
TESTS=tests/add.bf tests/comments.bf tests/hello_world.bf tests/mul.bf tests/nine.bf
BIN=zombie

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

debug:
	$(CC) $(CFLAGS) -o $(BIN)_debug $(SRC) $(LDFLAGS)

check: $(TESTS) $(BIN) tests/check.sh
	@./tests/check.sh

clean:
	$(RM) $(BIN) $(BIN)_debug tests/stderr tests/stdout
