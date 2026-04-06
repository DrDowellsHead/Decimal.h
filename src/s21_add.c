#include "s21_decimal.h"
#include "s21_helpers_funcs.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    // Функция для сложения двух чисел s21_decimal. Результат сохраняется в
    // переменной, на которую указывает result. Возвращает 0 при успешном
    // выполнении, 1 при переполнении, 2 при недопустимом масштабе, 3 при
    // недопустимом знаке и 4 при недопустимом указателе. Сложение происходит по
    // следующим шагам:
    // 1. Проверка на ноль: если одно из чисел равно нулю, то результатом будет
    // другое число с сохранением его знака.
    // 2. Нормализация масштабов: если знаки чисел различаются, то их мантиссы
    // нормализуются до одинакового масштаба (количество десятичных знаков).
    // 3. Сложение мантиссы: после нормализации, складываются мантиссы чисел.
    // Если знаки чисел одинаковые, то результатом будет сумма мантиссы двух
    // чисел с сохранением знака. Если знаки чисел различаются, то результатом
    // будет разность мантиссы большего числа и мантиссы меньшего числа с
    // сохранением знака большего числа. Если результат сложения мантиссы
    // превышает максимально допустимое значение, то возвращается код ошибки
    // переполнения.
    if (result == NULL) {
        printf("Результат не может быть равен NULL");
        return 4;
    }

    s21_zero_decimal(result);

    if (s21_is_zero(value_1)) {
        *result = value_2;
        return 0;
    }

    if (s21_is_zero(value_2)) {
        *result = value_1;
        return 0;
    }

    int sign_1 = s21_get_sign(value_1);
    int sign_2 = s21_get_sign(value_2);

    if (sign_1 != sign_2) {
        s21_decimal first = value_1;
        s21_decimal second = value_2;

        if (sign_1) {
            s21_set_sign(&first, 0);
            return s21_sub(value_2, first, result);
        } else {
            s21_set_sign(&second, 0);
            return s21_sub(value_1, second, result);
        }
    }

    s21_decimal first = value_1;
    s21_decimal second = value_2;

    if (s21_normalize_scales(&first, &second) != 0) {
        return sign_1 ? 2 : 1;
    }

    int scale = s21_get_scale(first);
    int overflow = s21_add_over_big_mantissa(first, second, result);

    if (overflow) {
        return sign_1 ? 2 : 1;
    }

    s21_set_scale(result, scale);
    s21_set_sign(result, sign_1);

    return 0;
}