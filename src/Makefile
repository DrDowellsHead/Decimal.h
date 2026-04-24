CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = --coverage

SOURCE_FILES = s21_decimal_utils.c \
               s21_decimal_other.c \
               s21_helpers_funcs.c \
               s21_auxfunct.c \
               s21_decimal_transform.c \
               s21_big_decimal.c \
               s21_mul.c \
               s21_add.c \
               s21_sub.c \
               s21_div.c \
               s21_is_equal.c \
               s21_is_not_equal.c \
               s21_is_less.c \
               s21_is_less_or_equal.c \
               s21_is_greater.c \
               s21_is_greater_or_equal.c

OBJECT_FILES = $(SOURCE_FILES:.c=.o)
SOURCE_TEST = s21_decimal_test.c
GCOV_DIR = report

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CHECK_LIBS = -lcheck -lm -lpthread -lsubunit
else
CHECK_LIBS = -lcheck -lm -lpthread
endif

all: clean s21_decimal.a test gcov_report

s21_decimal.a: $(OBJECT_FILES)
	ar crs s21_decimal.a $(OBJECT_FILES)
	rm -f *.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

test: s21_decimal.a
	$(CC) $(CFLAGS) $(SOURCE_TEST) s21_decimal.a $(CHECK_LIBS) -o s21_test_run
	./s21_test_run
	rm -f s21_test_run

gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_decimal_utils.c -o s21_decimal_utils.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_decimal_other.c -o s21_decimal_other.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_helpers_funcs.c -o s21_helpers_funcs.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_auxfunct.c -o s21_auxfunct.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_decimal_transform.c -o s21_decimal_transform.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_big_decimal.c -o s21_big_decimal.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_mul.c -o s21_mul.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_add.c -o s21_add.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_sub.c -o s21_sub.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_div.c -o s21_div.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_is_equal.c -o s21_is_equal.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_is_not_equal.c -o s21_is_not_equal.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_is_less.c -o s21_is_less.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_is_less_or_equal.c -o s21_is_less_or_equal.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_is_greater.c -o s21_is_greater.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_is_greater_or_equal.c -o s21_is_greater_or_equal.o
	ar crs s21_decimal.a $(OBJECT_FILES)
	rm -f *.o
	$(CC) $(CFLAGS) $(SOURCE_TEST) s21_decimal.a $(CHECK_LIBS) -lgcov -o s21_test_run_cov
	./s21_test_run_cov
	mkdir -p $(GCOV_DIR)
	lcov --capture --directory . --output-file $(GCOV_DIR)/coverage.info --ignore-errors mismatch
	genhtml $(GCOV_DIR)/coverage.info --output-directory $(GCOV_DIR)
	rm -f *.gcda *.gcno *.o s21_decimal.a s21_test_run_cov
	$(CC) $(CFLAGS) -c s21_decimal_utils.c -o s21_decimal_utils.o
	$(CC) $(CFLAGS) -c s21_decimal_other.c -o s21_decimal_other.o
	$(CC) $(CFLAGS) -c s21_helpers_funcs.c -o s21_helpers_funcs.o
	$(CC) $(CFLAGS) -c s21_auxfunct.c -o s21_auxfunct.o
	$(CC) $(CFLAGS) -c s21_decimal_transform.c -o s21_decimal_transform.o
	$(CC) $(CFLAGS) -c s21_big_decimal.c -o s21_big_decimal.o
	$(CC) $(CFLAGS) -c s21_mul.c -o s21_mul.o
	$(CC) $(CFLAGS) -c s21_add.c -o s21_add.o
	$(CC) $(CFLAGS) -c s21_sub.c -o s21_sub.o
	$(CC) $(CFLAGS) -c s21_div.c -o s21_div.o
	$(CC) $(CFLAGS) -c s21_is_equal.c -o s21_is_equal.o
	$(CC) $(CFLAGS) -c s21_is_not_equal.c -o s21_is_not_equal.o
	$(CC) $(CFLAGS) -c s21_is_less.c -o s21_is_less.o
	$(CC) $(CFLAGS) -c s21_is_less_or_equal.c -o s21_is_less_or_equal.o
	$(CC) $(CFLAGS) -c s21_is_greater.c -o s21_is_greater.o
	$(CC) $(CFLAGS) -c s21_is_greater_or_equal.c -o s21_is_greater_or_equal.o
	ar crs s21_decimal.a $(OBJECT_FILES)
	rm -f *.o
	@if [ "$(UNAME)" = "Linux" ]; then \
		xdg-open $(GCOV_DIR)/index.html 2>/dev/null || echo "Отчёт доступен в $(GCOV_DIR)"; \
	elif [ "$(UNAME)" = "Darwin" ]; then \
		open $(GCOV_DIR)/index.html 2>/dev/null || echo "Отчёт доступен в $(GCOV_DIR)"; \
	else \
		echo "Отчёт доступен в $(GCOV_DIR)"; \
	fi

rebuild: clean all

clean:
	rm -rf $(GCOV_DIR)
	rm -f *.o *.gcda *.gcno s21_decimal.a s21_test_run s21_test_run_cov

.PHONY: all clean test rebuild gcov_report