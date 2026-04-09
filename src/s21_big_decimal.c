#include "s21_big_decimal.h"

#include "s21_decimal.h"

void s21_big_zero(s21_big_decimal *rst) {
  // Очистка структуры от мусорных значений
  if (rst != NULL) {
    for (int i = 0; i < 7; i++) {  // Цикл по всем 7 словам мантиссы
      rst->bits[i] = 0;
    }
    rst->scale = 0;
    rst->sign = 0;
  }
}

void s21_big_from_decimal(s21_decimal src, s21_big_decimal *rst) {
  // Приводит обычный s21_decimal к 196-битной структуре
  // scale и sign получают значения из s21_decimal и хранят значение в
  // отдельных соответствующих переменных
  if (rst != NULL) {
    s21_big_zero(rst);

    rst->bits[0] = (unsigned int)src.bits[0];
    rst->bits[1] = (unsigned int)src.bits[1];
    rst->bits[2] = (unsigned int)src.bits[2];

    rst->scale = s21_get_scale(src);
    rst->sign = s21_get_sign(src);
  }
}

int s21_big_is_zero(s21_big_decimal value) {
  // Проверяет равняется ли большая мантисса нулю. 1 - ноль, 0 - не ноль.
  int result = 1;

  for (int i = 0; i < 7 && result; i++) {
    if (value.bits[i] != 0u) {
      result = 0;
    }
  }

  return result;
}

int s21_big_compare_mantissa(s21_big_decimal a, s21_big_decimal b) {
  // Сравнивает мантиссы двух big_decimal. 1 - a больше. -1 - a меньше. 0 -
  // равны
  for (int i = 6; i >= 0; i--) {  // От страшего слова к младшему
    if (a.bits[i] > b.bits[i]) {
      return 1;
    }
    if (a.bits[i] < b.bits[i]) {
      return -1;
    }
  }

  return 0;
}

int s21_big_add_mantissa(s21_big_decimal a, s21_big_decimal b,
                         s21_big_decimal *result) {
  // Складывает две большие мантиссы, как одно целое число. 0 - всё
  // уместитлось в 224 бита. Не 0 - был перенос за пределы массива
  unsigned long long over = 0;

  if (result == NULL) {
    return 1;
  }

  s21_big_zero(result);

  for (int i = 0; i < 7; i++) {
    unsigned long long first = a.bits[i];
    unsigned long long second = b.bits[i];
    unsigned long long sum = first + second + over;

    // Записывает в текущее слово результата только младшие 32 бита суммы
    result->bits[i] = (unsigned int)(sum & 0xFFFFFFFFu);
    // Берёт всё, что не влезло в нижние 32 бита, и делает это новым
    // переносом
    over = sum >> 32;
  }

  return (int)over;
}

int s21_big_sub_mantissa(s21_big_decimal a, s21_big_decimal b,
                         s21_big_decimal *result) {
  // Вычитает мантиссу b из мантиссы a. На случай, если a >= b. Если return !=
  // 0, то оастался заём

  unsigned long long borrow = 0;

  if (result == NULL) {
    return 1;
  }

  s21_big_zero(result);

  for (int i = 0; i < 7; i++) {
    unsigned long long first = a.bits[i];
    unsigned long long second = b.bits[i] + borrow;

    if (first < second) {
      result->bits[i] = (unsigned int)((1ULL << 32) + first - second);
      borrow = 1;
    } else {
      result->bits[i] = (unsigned int)(first - second);
      borrow = 0;
    }
  }

  return (int)borrow;
}

int s21_big_mul10_mantissa(s21_big_decimal *value) {
  // Функция умножает мантиссу на 10
  unsigned long long over = 0;

  if (value == NULL) {
    return 1;
  }

  for (int i = 0; i < 7; i++) {
    unsigned long long part = value->bits[i];
    unsigned long long temp = part * 10u + over;

    value->bits[i] = (unsigned int)(temp & 0xFFFFFFFFu);
    over = temp >> 32;
  }

  return (int)over;
}

int s21_big_div10_mantissa(s21_big_decimal *value, unsigned int *remainder) {
  // Делит большую мантиссу на 10 и возвращает остаток
  unsigned long long rem = 0;

  if (value == NULL || remainder == NULL) {
    return 1;
  }

  for (int i = 6; i >= 0; i--) {
    unsigned long long current = (rem << 32) | value->bits[i];

    value->bits[i] = (unsigned int)(current / 10u);
    rem = current % 10u;
  }

  *remainder = (unsigned int)rem;
  return 0;
}

int s21_big_normalize_scales(s21_big_decimal *a, s21_big_decimal *b) {
  // Приводит мантиссы двух чисел к одному значению
  if (a == NULL || b == NULL) {
    return 1;
  }

  while (a->scale < b->scale) {
    if (s21_big_mul10_mantissa(a) != 0) {
      return 1;
    }
    a->scale++;
  }

  while (b->scale < a->scale) {
    if (s21_big_mul10_mantissa(b) != 0) {
      return 1;
    }
    b->scale++;
  }

  return 0;
}

int s21_big_fits_decimal(s21_big_decimal value) {
  // Проверка помещается ли большая мантисса в обычную мантиссу

  // Проверка, что все старшие слова большой мантиссы не нулевые
  return value.bits[3] == 0U && value.bits[4] == 0U && value.bits[5] == 0U &&
         value.bits[6] == 0U;
}

int s21_big_add_one_mantissa(s21_big_decimal *value) {
  // Добавляет единицу к большой мантиссе. Если return != 0, то был перенос за
  // пределы массива
  unsigned long long over = 1;

  if (value == NULL) {
    return 1;
  }

  for (int i = 0; i < 7 && over; i++) {
    unsigned long long sum = (unsigned long long)value->bits[i] + over;
    value->bits[i] = (unsigned int)(sum & 0xFFFFFFFFu);
    over = sum >> 32;
  }

  return (int)over;
}

int s21_big_round_up(s21_big_decimal value, unsigned int last_removed,
                     int sticky) {
  // Определяет, нужно ли округлять число вверх. Возвращает 1, если нужно, и
  // 0, если нет.

  // Если последняя удалённая цифра больше 5, всегда округляем вверх
  if (last_removed > 5u) {
    return 1;
  }

  // Если последняя удалённая цифра меньше 5, никогда не округляем вверх
  if (last_removed < 5u) {
    return 0;
  }

  // Если последняя удалённая цифра равна 5, округляем вверх только если были
  // удалены не нулевые цифры после неё или если последняя оставшаяся цифра
  // нечётная (метод "округление до ближайшего чётного")
  if (sticky) {
    return 1;
  }

  // Если последняя удалённая цифра равна 5 и не были удалены не
  // нулевые цифры после неё, округляем вверх только если последняя
  // оставшаяся цифра нечётная (метод "округление до ближайшего
  // чётного")
  return (value.bits[0] & 1u) != 0u;
}

int s21_big_decimal_rounding(s21_big_decimal src, s21_decimal *rst) {
  // Приводит большую мантиссу к обычной, при этом выполняя округление.
  // Возвращает 0, если всё прошло успешно, 1 - если результат не может быть
  // представлен в виде s21_decimal, 2 - если результат не может быть
  // представлен в виде s21_decimal, но может быть представлен в виде
  // s21_decimal с другим знаком (например, из-за переполнения при
  // округлении).
  if (rst == NULL) {
    return src.sign ? 2 : 1;
  }

  s21_zero_decimal(rst);

  if (src.scale < 0) {
    return src.sign ? 2 : 1;
  }

  if (s21_big_is_zero(src)) {
    return 0;
  }

  s21_big_decimal temp = src;

  while (temp.scale > 28 || !s21_big_fits_decimal(temp)) {
    unsigned int last_removed = 0;
    unsigned int remainder = 0;
    int sticky = 0;

    while (temp.scale > 28 || !s21_big_fits_decimal(temp)) {
      if (temp.scale == 0 && !s21_big_fits_decimal(temp)) {
        return temp.sign ? 2 : 1;
      }

      sticky |= (last_removed != 0u);

      if (s21_big_div10_mantissa(&temp, &remainder) != 0) {
        return temp.sign ? 2 : 1;
      }

      last_removed = remainder;
      temp.scale--;
    }

    if (s21_big_round_up(temp, last_removed, sticky)) {
      if (s21_big_add_one_mantissa(&temp) != 0) {
        return temp.sign ? 2 : 1;
      }

      if (temp.scale == 0 && !s21_big_fits_decimal(temp)) {
        return temp.sign ? 2 : 1;
      }
    }
  }

  rst->bits[0] = (int)temp.bits[0];
  rst->bits[1] = (int)temp.bits[1];
  rst->bits[2] = (int)temp.bits[2];

  s21_set_scale(rst, temp.scale);

  if (!s21_is_zero(*rst)) {
    s21_set_sign(rst, temp.sign);
  }

  return 0;
}

int s21_add_sub_core(s21_decimal value_1, s21_decimal value_2,
                     s21_decimal *result, int is_sub) {
  // Основная функция для сложения и вычитания. Если is_sub == 0, то
  // выполняется сложение, иначе - вычитание. Возвращает 0, если всё прошло
  // успешно, 1 - если результат не может быть представлен в виде s21_decimal,
  // 2 - если результат не может быть представлен в виде s21_decimal, но может
  // быть представлен в виде s21_decimal с другим знаком (например, из-за
  // переполнения при округлении).
  if (result == NULL) {
    return 1;
  }

  s21_zero_decimal(result);

  s21_decimal second_dec = value_2;

  if (is_sub) {
    s21_set_sign(&second_dec, !s21_get_sign(second_dec));
  }

  s21_big_decimal a;
  s21_big_decimal b;
  s21_big_decimal res;

  s21_big_from_decimal(value_1, &a);
  s21_big_from_decimal(second_dec, &b);

  if (s21_big_normalize_scales(&a, &b) != 0) {
    return a.sign ? 2 : 1;
  }

  res.scale = a.scale;
  res.sign = 0;

  if (a.sign == b.sign) {
    if (s21_big_add_mantissa(a, b, &res) != 0) {
      return a.sign ? 2 : 1;
    }
    res.sign = a.sign;
  } else {
    int cmp = s21_big_compare_mantissa(a, b);

    if (cmp == 0) {
      return 0;
    }

    if (cmp > 0) {
      if (s21_big_sub_mantissa(a, b, &res) != 0) {
        return a.sign ? 2 : 1;
      }
      res.sign = a.sign;
    } else {
      if (s21_big_sub_mantissa(b, a, &res) != 0) {
        return b.sign ? 2 : 1;
      }
      res.sign = b.sign;
    }
  }

  res.scale = a.scale;
  return s21_big_decimal_rounding(res, result);
}