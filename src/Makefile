CC = gcc 
CFLAGS = -Wall -Werror  -Wextra 
GCOV_FLAGS = --coverage 
CHECK_FLAGS = -D_CRT_SECURE_NO_WARNINGS

SOURCE_FILES = s21_decimal_transform.c s21_auxfunct.c
OBJECT_FILES = $(SOURCE_FILES:.c=.o)
SOURCE_TEST = s21_decimal_transform_test.c

GCOV_DIR = report

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CHECK_LIBS = -lcheck -lm -lpthread -lsubunit
else
CHECK_LIBS = -lcheck -lm -lpthread
endif


# цель по умолчанию 
all: clean s21_decimal.a test gcov_report

# создание библиотеки
s21_decimal.a : $(OBJECT_FILES)
	ar crs s21_decimal.a $(OBJECT_FILES)
	rm -f *.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# тест без gcov отчета
test: s21_decimal.a 
	$(CC) $(CFLAGS) $(CHECK_FLAGS) $(SOURCE_TEST) s21_decimal.a $(CHECK_LIBS) -o s21_test_run
	./s21_test_run
	rm -f s21_test_run

clean:
	rm -rf $(GCOV_DIR)
	rm -f *.o s21_decimal.a s21_test_run s21_test_run_cov

# создание отчёта gcov 
gcov_report: clean
  # Компилируем все исходные файлы для сборки библиотеки с флагами покрытия для прохождения тестов с gcov
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_decimal_transform.c -o s21_decimal_transform.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c s21_auxfunct.c -o s21_auxfunct.o
	ar crs s21_decimal.a $(OBJECT_FILES)
	rm -f *.o

  # Запуск тестов с покрытием
	$(CC) $(CFLAGS) $(CHECK_FLAGS) $(SOURCE_TEST) s21_decimal.a $(CHECK_LIBS) -lgcov -o s21_test_run_cov
	./s21_test_run_cov

  # Создаём отчет
	mkdir -p $(GCOV_DIR)
	lcov --capture --directory . --output-file $(GCOV_DIR)/coverage.info --ignore-errors mismatch
	genhtml $(GCOV_DIR)/coverage.info --output-directory $(GCOV_DIR)

  # Убираем временные файлы
	rm -f *.gcda *.gcno *.o s21_decimal.a s21_test_run_cov

  # Компилируем все исходные файлы без флагов покрытия для создания архива библиотеки без флагов покрытия 
	$(CC) $(CFLAGS) -c s21_decimal_transform.c -o s21_decimal_transform.o
	$(CC) $(CFLAGS) -c s21_auxfunct.c -o s21_auxfunct.o
	ar crs s21_decimal.a $(OBJECT_FILES)
	rm -f *.o

  # Открываем gcov отчёт 
	@if [ "$(UNAME)" = "Linux" ]; then \
		xdg-open $(GCOV_DIR)/index.html 2>/dev/null || echo "Отчёт доступен в $(GCOV_DIR)"; \
	elif [ "$(UNAME)" = "Darwin" ]; then \
		open $(GCOV_DIR)/index.html 2>/dev/null || echo "Отчёт доступен в $(GCOV_DIR)"; \
	else \
		echo "Отчёт доступен в $(GCOV_DIR)"; \
	fi

.PHONY: all clean test gcov_report