CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pthread -I.
GCOV_FLAGS =
CHECK_LIBS = -lcheck -lm -lpthread -lrt -lsubunit

SRC = \
	s21_add.c \
	s21_sub.c \
	s21_is_equal.c \
	s21_is_less.c \
	s21_is_not_equal.c \
	s21_is_greater.c \
	s21_is_less_or_equal.c \
	s21_is_greater_or_equal.c \
	s21_helpers_funcs.c \
	s21_big_decimal.c

TEST_SRC = \
	Tests/test_func.c \
	Tests/test_main.c

OBJ = $(SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

TARGET = test_runner

all: test

test: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $^ -o $@ $(CHECK_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $@

Tests/%.o: Tests/%.c
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $@

gcov_report: clean
	$(MAKE) GCOV_FLAGS="--coverage" test
	gcovr -r . --exclude 'Tests/*' --html --html-details -o gcov_report.html
	xdg-open gcov_report.html 2>/dev/null || true

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET)
	rm -f *.gcno *.gcda Tests/*.gcno Tests/*.gcda
	rm -f gcov_report.html

re: clean all