CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11

SRC     = s21_decimal_utils.c s21_decimal_other.c
OBJ     = $(SRC:.c=.o)
LIB     = s21_decimal.a

TEST_SRC = s21_decimal_test.c
TEST_BIN = test_runner

.PHONY: all clean test s21_decimal.a

all: s21_decimal.a

s21_decimal.a: $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: s21_decimal.a
	$(CC) $(CFLAGS) $(TEST_SRC) -L. -l:s21_decimal.a -lcheck -lm -lpthread -lsubunit -o $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(OBJ) $(LIB) $(TEST_BIN)

rebuild: clean all