#include "s21_helpers_funcs.h"

#include "s21_decimal.h"

void s21_zero_decimal(s21_decimal *dst) {
    // Функция для приведения данных s21_decimal к нулям во избежания
    // использования мусорных данных
    if (dst != NULL) {
        dst->bits[0] = 0;
        dst->bits[1] = 0;
        dst->bits[2] = 0;
        dst->bits[3] = 0;
    }
}

int s21_is_zero(s21_decimal value) {
    // Функция для проверки, является ли s21_decimal нулём (т.е. все биты
    // мантиссы равны нулю)
    return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0;
}

int s21_get_sign(s21_decimal value) {
    // Получение показателя знака мантиссы
    return ((unsigned int)value.bits[3] >> 31) & 1u;
}

int s21_set_sign(s21_decimal *value, int sign) {
    // Установка знака мантиссы (0 - положительный, 1 - отрицательный)
    if (value != NULL) {
        if (sign) {
            // В Шестнадцетиричном виде 0x80000000
            // соответствует 1000 0000 0000 0000 0000
            // 0000 0000 0000, что устанавливает старший
            // бит в единицу (отрицательное число)
            value->bits[3] |= (int)0x80000000u;
        } else {
            // В Шестнадцатеричном виде 0x7FFFFFFF
            // соответствует 0111 1111 1111 1111 1111
            // 1111 1111 1111, что устанавливает старший
            // бит в ноль (положительное число)
            value->bits[3] &= (int)0x7FFFFFFFu;
        }
    }
}

int s21_get_scale(s21_decimal value) {
    // Получение показателя точности числа
    return ((unsigned int)value.bits[3] >> 16) &
           0xFFu;  // Маска, которая не позволит прочитать больше 8 бит
}

int s21_set_scale(s21_decimal *value, int scale) {
    // Установка показателя точности мантиссы
    int status = 0;

    if (value == NULL || scale < 0 || scale > 28) {
        status = 1;
    } else {
        unsigned int service = (unsigned int)value->bits[3];

        service &= 0xFF00FFFFu;  // Маска для битов с 16 до 23
        service |= ((unsigned int)scale << 16);

        value->bits[3] = (int)service;
    }

    return status;
}

int s21_compare_mantissa(s21_decimal a, s21_decimal b) {
    // Функция для сравнения мантиссы двух чисел s21_decimal. Возвращает 1, если
    // мантисса a больше, -1, если мантисса b больше, и 0, если мантиссы равны.
    for (int i = 2; i >= 0; i--) {
        unsigned int first = (unsigned int)a.bits[i];
        unsigned int second = (unsigned int)b.bits[i];

        if (first > second) return 1;
        if (first < second) return -1;
    }
    return 0;
}

int s21_add_over_big_mantissa(s21_decimal a, s21_decimal b, s21_decimal *result) {
    // Функция для сложения мантиссы в 96 битов (3 элемента массива bits), двух
    // чисел s21_decimal. Возвращает 1, если результат сложения превышает 96
    // битов, и 0 в противном случае.
    unsigned long long over = 0;

    if (result == NULL) {
        printf("Результат не может быть равен NULL");
        return 4;  // Код ошибки 4, потому что 1 возвращается в случае
                   // переполнения 3 слов bits
    }

    s21_zero_decimal(result);

    for (int i = 0; i < 3; i++) {
        unsigned long long first = (unsigned int)a.bits[i];
        unsigned long long second = (unsigned int)b.bits[i];
        unsigned long long sum = first + second + over;

        result->bits[i] = (int)(sum & 0xFFFFFFFFu);
        over = sum >> 32;
    }

    return (int)over;
}

int s21_sub_over_big_mantissa(s21_decimal a, s21_decimal b,
                              s21_decimal *result) {
    // Реализует вычитание из 96 битного слова. Для реализации вычитания чисел с
    // разным знаком. Используется алгоритм, аналогичный алгоритму сложения, но
    // с учётом заимствования. Возвращает 1, если результат вычитания меньше
    // нуля (т.е. мантисса a меньше мантиссы b), и 0 в противном случае.
    unsigned int borrow = 0;

    if (result == NULL) {
        printf("Результат не может быть равен NULL");
        return 4;  // Такая же логика, что и в s21_over_big_mantissa
    }

    s21_zero_decimal(result);

    for (int i = 0; i < 3; i++) {
        unsigned long long first = (unsigned int)a.bits[i];
        unsigned long long second = (unsigned int)b.bits[i] + borrow;

        if (first < second) {
            // 1ULL - 2^32 - одно 32-битное слово
            result->bits[i] = (int)((1ULL << 32) + first - second);
            borrow = 1;
        } else {
            result->bits[i] = (int)(first - second);
            borrow = 0;
        }
    }

    return (int)borrow;
}

int s21_multi_10_mantissa(s21_decimal *value) {
    // При разнице двух мантисс, приводит их общему виду для дальнейших
    // операций, умножая на 10 мантиссу одного из чисел, которая меньше другой.
    // Возвращает 1, если результат умножения превышает 96 битов, и 0 в
    // противном случае. Это вспомогательная функция для нормализации мантиссы.
    unsigned long long over = 0;

    if (value == NULL) {
        printf("Value не может быть равным NULL");
        return 4;
    }

    for (int i = 0; i < 3; i++) {
        unsigned long long part = (unsigned int)value->bits[i];
        unsigned long long temp = part * 10u + over;

        value->bits[i] = (int)(temp & 0xFFFFFFFFu);
        over = temp >> 32;
    }

    return (int)over;
}

int s21_normalize_scales(s21_decimal *a, s21_decimal *b) {
    // Функция сравнивает мантиссы двух чисел. Если они не равны, то применяет к
    // ним s21_multi_mantissa до тех пор, пока не сравняются
    if (a == NULL || b == NULL) {
        return 4;
    }

    int scale_a = s21_get_scale(*a);
    int scale_b = s21_get_scale(*b);

    while (scale_a < scale_b) {
        if (s21_multi_10_mantissa(a) != 0) {
            return 1;
        }
        scale_a++;
        s21_set_scale(a, scale_a);
    }

    while (scale_b < scale_a) {
        if (s21_multi_10_mantissa(b) != 0) {
            return 1;
        }
        scale_b++;
        s21_set_scale(b, scale_b);
    }

    return 0;
}