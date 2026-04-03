#include "s21_decimal.h"
#include "s21_helpers_funcs.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    // Функция для вычитания одного числа s21_decimal из другого. Результат
    // сохраняется в переменной, на которую указывает result. Возвращает 0 при
    // успешном выполнении, 1 при переполнении, 2 при недопустимом масштабе, 3
    // при недопустимом знаке и 4 при недопустимом указателе. Вычитание
    // происходит по следующим шагам:
    // 1. Проверка на ноль: если второе число равно нулю, то результатом будет
    // первое число с сохранением его знака.
    // 2. Проверка на ноль: если первое число равно нулю, то результатом будет
    // второе число с противоположным знаком.
    // 3. Нормализация масштабов: если знаки чисел различаются, то их мантиссы
    // нормализуются до одинакового масштаба (количество десятичных знаков).
    // 4. Вычитание мантиссы: после нормализации, вычитаются мантиссы чисел.
    // Если первое число положительное, то результатом будет разность мантиссы
    // первого числа и мантиссы второго числа с сохранением знака первого числа.
    // Если первое число отрицательное, то результатом будет разность мантиссы
    // второго числа и мантиссы первого числа с противоположным знаком первого
    // числа. Если результат вычитания мантиссы превышает максимально допустимое
    // значение, то возвращается код ошибки переполнения.
    if (result == NULL) {
        printf("Результат не может быть равен NULL");
        return 4;
    }

    s21_zero_decimal(result);

    if (s21_is_zero(value_2)) {
        *result = value_1;
        return 0;
    }

    if (s21_is_zero(value_1)) {
        *result = value_2;
        s21_set_sign(result, !s21_get_sign(value_2));
        return 0;
    }

    s21_decimal first = value_1;
    s21_decimal second = value_2;

    int sign_1 = s21_get_sign(first);
    int sign_2 = s21_get_sign(second);

    if (s21_normalize_scales(&first, &second) != 0) {
        return sign_1 ? 2 : 1;
    }

    // После нормализации мантиссы scale(first) == scale(second)
    int scale = s21_get_scale(first);

    if (sign_1 != sign_2) {
        int overflow = s21_add_over_big_mantissa(first, second, result);

        if (overflow) {
            return sign_1 ? 2 : 1;
        }
        s21_set_scale(result, scale);
        s21_set_sign(result, sign_1);
        return 0;
    }

    int diff_mantissa = s21_compare_mantissa(first, second);

    if (diff_mantissa == 0) {
        return 0;
    }

    if (diff_mantissa > 0) {
        s21_sub_over_big_mantissa(first, second, result);
        s21_set_scale(result, scale);
        s21_set_sign(result, sign_1);
    } else {
        s21_sub_over_big_mantissa(first, second, result);
        s21_set_scale(result, scale);
        s21_set_sign(result, sign_1 ? 0 : 1);
    }

    return 0;
}