CC = gcc 
CFLAGS = -Wall # -Werror  -Wextra 
GCOV_FLAGS = --coverage 
CHECK_FLAGS = -D_CRT_SECURE_NO_WARNINGS

SOURCE_FILES = s21_decimal_transform.c s21_auxfunct.c
OBJECT_FILES = s21_decimal_transform.o s21_auxfunct.o
SOURCE_TEST = DecTest.c

GCOV_DIR = report

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CHECK_LIBS = -lcheck -lm -lpthread -lsubunit
else
CHECK_LIBS = -lcheck -lm -lpthread
endif


# цель по умолчанию 
all: clean s21_decimal.a test #gcov_report

# создание библиотеки
s21_decimal.a : $(OBJECT_FILES)
	ar crs s21_decimal.a $(OBJECT_FILES)
	rm -f *.o

$(OBJECT_FILES): %.o: %.c
	$(CC) $(CFLAGS) -c $<

# тест без gcov отчета
test: clean s21_decimal.a s21_test_run
	./s21_test_run

s21_test_run: $(SOURCE_TEST)
#	$(CC) $(CFLAGS) $(CHECK_FLAGS) $(SOURCE_TEST) s21_string.a $(CHECK_LIBS) -o $@
	$(CC) $(CFLAGS) $(SOURCE_TEST) s21_decimal.a -lm -o $@

clean:
	rm -f *.o s21_decimal.a s21_test_run


.PHONY: all clean test 