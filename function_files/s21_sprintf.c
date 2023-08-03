#include "../s21_string.h"
#include "../utility.h"

int s21_sprintf(char* str, const char* format, ...) {
  int result = 0;
  char* cur = str;

  va_list args;
  va_start(args, format);

  if (str) {
    for (char* pos = (char*)format; *pos; ++pos) {
      Prototype prot;
      prot.start = str;
      if (*pos == '%' && !parse_prototype(&pos, &prot, args, S21_SPRINTF)) {
        print_prototype(&cur, prot);
      } else {
        print_character(&cur, *pos);
      }
    }
    print_character(&cur, 0);
    result = cur - str - 1;
  } else {
    result = -1;
  }

  va_end(args);
  return result;
}

// N

void get_n(char** cur, Prototype prot) { *(prot.value.n) = *cur - prot.start; }

// PERCENT

void print_percent(char** cur, Prototype prot) {
  print_character(cur, prot.value.c);
}

// Shortest Representation

void print_shortest(char** cur, Prototype prot) {
  // according to c11 standart
  int p = prot.precision;
  p = p == -1 ? FLOAT_DEFAULT_PRECISION : p;
  p = p == 0 ? 1 : p;
  long double estyle = prot.value.Lf;
  int x = normalize(&estyle);

  if (p > x && x >= -4) {
    prot.precision = p - (x + 1);
    prot.precision = remove_trailing_zeroes(prot);
    print_float(cur, prot);
  } else {
    prot.spec = prot.spec == SHORTEST ? SCIENTIFIC : SCIENTIFIC_CAPITAL;
    prot.precision = p - 1;
    prot.precision = remove_trailing_zeroes(prot);
    print_scientific(cur, prot);
  }
}

int get_shortest_length(Prototype prot) {
  int p = prot.precision;
  p = p == -1 ? FLOAT_DEFAULT_PRECISION : p;
  p = p == 0 ? 1 : p;
  long double estyle = prot.value.Lf;
  int x = normalize(&estyle);
  int len = 0;

  if (p > x && x >= -4) {
    prot.precision = p - (x + 1);
    prot.precision = remove_trailing_zeroes(prot);
    len = get_float_length(prot);
  } else {
    prot.spec = prot.spec == SHORTEST ? SCIENTIFIC : SCIENTIFIC_CAPITAL;
    prot.precision = p - 1;
    prot.precision = remove_trailing_zeroes(prot);
    len = get_scientific_length(prot);
  }

  return len;
}

// returns new precision
int remove_trailing_zeroes(Prototype prot) {
  int current_prescision = prot.precision;
  int new_precision = current_prescision;

  if (!prot.flags.hash && current_prescision != 0) {
    if (prot.spec == SCIENTIFIC || prot.spec == SCIENTIFIC_CAPITAL) {
      normalize(&prot.value.Lf);
    }
    Split_float split = {0, 0, 0};
    split_float(prot.value.Lf, &split, current_prescision);
    int trailing_zeroes = 0;
    for (int digit = (int)fmodl(split.q, 10);
         digit == 0 && trailing_zeroes < current_prescision;
         ++trailing_zeroes) {
      split.q /= 10;
      digit = (int)fmodl(split.q, 10);
    }
    new_precision = current_prescision - trailing_zeroes;
  }

  return new_precision;
}

// Scientific Notation

void print_scientific(char** cur, Prototype prot) {
  int exp = normalize(&prot.value.Lf);
  Prototype exp_prot = {{0, 1, 0, 0, 0}, 0,     2,         NO_LENGTH,
                        DECIMAL,         {exp}, prot.start};
  Prototype mantissa_prot = {
      prot.flags,     prot.width - 1 - get_decimal_length(exp_prot),
      prot.precision, prot.length,
      prot.spec,      prot.value,
      prot.start};
  print_float(cur, mantissa_prot);
  print_character(cur, prot.spec == SCIENTIFIC ? 'e' : 'E');
  print_decimal(cur, exp_prot);
}

int get_scientific_length(Prototype prot) {
  int exp = normalize(&prot.value.Lf);
  Prototype exp_prot = {{0, 1, 0, 0, 0}, 0,     2,         NO_LENGTH,
                        DECIMAL,         {exp}, prot.start};
  Prototype mantissa_prot = {
      prot.flags,     prot.width - 1 - get_decimal_length(exp_prot),
      prot.precision, prot.length,
      prot.spec,      prot.value,
      prot.start};
  return get_float_length(mantissa_prot) + 1 + get_decimal_length(exp_prot);
}

int normalize(long double* num) {
  int exp = 0;
  int sign = *num < 0 ? 1 : 0;
  *num = sign ? -(*num) : *num;

  while ((*num < 1 || *num >= 10) && *num != 0) {
    if (*num < 1) {
      --exp;
      *num *= 10;
    } else {
      ++exp;
      *num /= 10;
    }
  }

  *num = sign ? -(*num) : *num;

  return exp;
}

// FLOAT

void print_float(char** cur, Prototype prot) {
  long double f = prot.value.Lf;
  int sign = f < 0 ? 1 : 0;
  f = sign ? -f : f;
  int precision =
      prot.precision == -1 ? FLOAT_DEFAULT_PRECISION : prot.precision;
  Split_float split = {0, 0, 0};
  int sign_lenth = (sign || prot.flags.sign || prot.flags.space);
  int dot_length = (prot.precision != 0 || prot.flags.hash);

  split_float(f, &split, precision);

  if (sign)
    print_character(cur, '-');
  else if (prot.flags.sign || prot.flags.space) {
    print_character(cur, prot.flags.sign ? '+' : ' ');
  }

  if (prot.flags.zero_padding) {
    int leading_zeroes = prot.width - sign_lenth -
                         get_pure_float_length(split.z) - dot_length -
                         precision;
    for (int i = 0; i < leading_zeroes; ++i) {
      print_character(cur, '0');
    }
  }
  print_Lf(cur, split.z);

  if (dot_length) print_character(cur, '.');

  if (split.q != 0) {
    for (int i = 0; i < split.leading_zeroes; ++i) print_character(cur, '0');
    print_Lf(cur, split.q);
  } else {
    for (int i = 0; i < precision; ++i) print_character(cur, '0');
  }
}

void split_float(long double f, Split_float* split, int precision) {
  long double fq = modfl(f, &split->z);
  split->leading_zeroes = 0;
  int count_zeroes = 1;

  for (int i = 0; i < precision; ++i) {
    fq *= 10;
    split->q *= 10;
    int digit = (int)fmodl(fq, 10);
    if (count_zeroes && digit == 0)
      split->leading_zeroes++;
    else
      count_zeroes = 0;
    split->q += digit;
  }
  fq *= 10;
  int round_digit = (int)fmodl(fq, 10);
  if (round_digit >= 5) {
    if (get_pure_float_length(split->q + 1) + split->leading_zeroes >
        precision) {
      split->z += 1;  // overflow happened ig
      split->q = 0;
    } else {
      if (get_pure_float_length(split->q) <
              get_pure_float_length(split->q + 1) ||
          split->q < 1)
        split->leading_zeroes--;  // overflow case too
      split->q += 1;
    }
  }
}

int get_pure_float_length(long double num) {
  int i = 0;

  while (num >= 1) {
    num /= 10;
    ++i;
  }

  return i == 0 ? 1 : i;
}

int get_float_length(Prototype prot) {
  long double f = prot.value.Lf;
  int sign = f < 0 ? 1 : 0;
  f = sign ? -f : f;
  Split_float split = {0, 0, 0};
  int precision =
      prot.precision == -1 ? FLOAT_DEFAULT_PRECISION : prot.precision;
  int dot = prot.precision != 0 || prot.flags.hash;
  int leading_zeroes = 0;

  split_float(f, &split, precision);
  int pure_len = get_pure_float_length(split.z);

  if (prot.flags.sign || prot.flags.space) sign = 1;
  if (prot.flags.zero_padding) {
    leading_zeroes = prot.width - sign - pure_len - dot - precision;
    leading_zeroes = leading_zeroes < 0 ? 0 : leading_zeroes;
  }

  return sign + leading_zeroes + pure_len + dot + precision;
}

void print_Lf(char** cur, long double num) {
  if (num >= 1) {
    char reverse[DOUBLE_MAX_LENGTH];
    int i = DOUBLE_MAX_LENGTH - 1;

    while (num >= 1) {
      int rem = (fmodl(num, 10));
      reverse[i--] = (int)rem + 0x30;
      num -= rem;
      num /= 10;
    }

    for (i++; i < DOUBLE_MAX_LENGTH; ++i) {  // это ROFLS
      print_character(cur, reverse[i]);
    }
  } else {
    print_character(cur, '0');
  }
}

// POINTER

void print_pointer(char** cur, Prototype prot) {
  prot.spec = HEXADECIMAL;
  prot.length = LONG_INT;
  print_prefix(cur, prot);
  print_decimal(cur, prot);
}

int get_pointer_length(Prototype prot) {
  prot.spec = HEXADECIMAL;
  prot.length = LONG_INT;
  return 2 + get_decimal_length(prot);
}

// DECIMAL TYPES

void print_lu(char** cur, Prototype prot) {
  unsigned long num = prot.value.lu;
  int base = get_base(prot);
  char* alph = prot.spec == HEXADECIMAL ? HEXADECIMAL_ALPHABET
                                        : HEXADECIMAL_ALPHABET_CAPITAL;

  if (num != 0) {
    char reverse[UNSIGNED_LONG_MAX_LENGTH];
    int i = UNSIGNED_LONG_MAX_LENGTH - 1;

    while (num) {
      reverse[i--] = alph[num % base];
      num /= base;
    }

    for (i++; i < UNSIGNED_LONG_MAX_LENGTH; ++i) {  // это ROFLS
      print_character(cur, reverse[i]);
    }
  } else if (prot.precision != 0) {
    print_character(cur, '0');
  }
}

void convert_ld_to_lu(Prototype* prot, int* sign) {
  long val = prot->value.ld;
  if (prot->spec == DECIMAL || prot->spec == IDECIMAL) {
    *sign = val < 0 ? 1 : 0;
    prot->value.ld = *sign ? -val : val;
  }
}

void print_decimal(char** cur, Prototype prot) {
  int sign = 0;
  int pure_len = 0;

  convert_ld_to_lu(&prot, &sign);
  pure_len = get_pure_decimal_length(prot.value.ld, get_base(prot));

  if (sign)
    print_character(cur, '-');
  else if (prot.flags.sign || prot.flags.space) {
    print_character(cur, prot.flags.sign ? '+' : ' ');
  }

  if (prot.flags.hash && prot.value.lu != 0) print_prefix(cur, prot);

  int leading_zeroes = prot.precision;
  if (prot.flags.zero_padding && prot.precision == -1) {
    leading_zeroes = prot.width - (sign || prot.flags.sign || prot.flags.space);
  }
  if (leading_zeroes > pure_len) {
    for (int i = 0; i < leading_zeroes - pure_len; ++i) {
      print_character(cur, '0');
    }
  }
  print_lu(cur, prot);
}

void print_prefix(char** cur, Prototype prot) {
  int base = get_base(prot);

  if (base == 8) {
    print_character(cur, '0');
  } else if (base == 16 && prot.spec == HEXADECIMAL) {
    print_character(cur, '0');
    print_character(cur, 'x');
  } else if (base == 16 && prot.spec == HEXADECIMAL_CAPITAL) {
    print_character(cur, '0');
    print_character(cur, 'X');
  }
}

int get_decimal_length(Prototype prot) {
  int sign = 0;
  convert_ld_to_lu(&prot, &sign);
  int len = 0;
  int base = get_base(prot);
  int prefix = 0;
  if (prot.flags.hash && base != 10 && prot.value.lu != 0) {
    prefix = base == 8 ? 1 : prefix;
    prefix = base == 16 ? 2 : prefix;
  }

  len = get_pure_decimal_length(prot.value.lu, get_base(prot));
  int leading_zeroes = prot.precision;

  if (prot.flags.sign || prot.flags.space) sign = 1;
  if (prot.flags.zero_padding && prot.precision == -1) {
    leading_zeroes = prot.width - sign;
    if (prot.flags.hash) {
      leading_zeroes -= prefix;
    }
  }
  if (leading_zeroes > len) len += leading_zeroes - len;

  return sign + prefix + len;
}

int get_pure_decimal_length(unsigned long num, int base) {
  int i = 0;

  while (num) {
    num /= base;
    ++i;
  }

  return i == 0 ? 1 : i;
}

int get_base(Prototype prot) {
  int base = 10;
  base = prot.spec == OCTAL ? 8 : base;
  base = prot.spec == HEXADECIMAL || prot.spec == HEXADECIMAL_CAPITAL ||
                 prot.spec == POINTER
             ? 16
             : base;

  return base;
}

// STRING

void print_string(char** cur, Prototype prot) {
  for (char* p = prot.value.s;
       *p && (p - prot.value.s < prot.precision || prot.precision == -1); ++p) {
    print_character(cur, *p);
  }
}

void print_wstring(char** cur, Prototype prot) {
  for (wchar_t* p = prot.value.ls;
       *p && (p - prot.value.ls < prot.precision || prot.precision == -1);
       ++p) {
    print_wcharacter(cur, *p);
  }
}

int get_string_length(Prototype prot) {
  int str_l = s21_strlen(prot.value.s);
  int result = 0;
  if (prot.precision != -1 && str_l > prot.precision) {
    result = prot.precision;
  } else {
    result = str_l;
  }

  return result;
}

// CHARACTERS TYPES

void print_c(char** cur, Prototype prot) { print_character(cur, prot.value.c); }

void print_wc(char** cur, Prototype prot) {
  print_wcharacter(cur, prot.value.lc);
}

void print_character(char** cur, char c) {
  **cur = c;
  ++(*cur);
}

void print_wcharacter(char** cur, wchar_t c) {
  wchar_t** wcur = (wchar_t**)cur;
  **wcur = c;
  *cur += 1;
}

// PROTOTYPE PRINTING/PARSING

void print_prototype(char** cur, Prototype prot) {
  void (*choice)(char**, Prototype);
  int printed = 0;

  if (prot.length == NO_LENGTH && prot.spec == SYM) {
    choice = &print_c;
    printed = 1;
  } else if (prot.length == LONG_INT && prot.spec == SYM) {
    choice = &print_wc;
    printed = 1;
  } else if (prot.spec == DECIMAL || prot.spec == IDECIMAL ||
             prot.spec == UNSIGNED || prot.spec == OCTAL ||
             prot.spec == HEXADECIMAL || prot.spec == HEXADECIMAL_CAPITAL) {
    choice = &print_decimal;
    printed = get_decimal_length(prot);
  } else if (prot.spec == FLOAT) {
    choice = &print_float;
    printed = get_float_length(prot);
  } else if (prot.spec == SCIENTIFIC || prot.spec == SCIENTIFIC_CAPITAL) {
    choice = &print_scientific;
    printed = get_scientific_length(prot);
  } else if (prot.spec == SHORTEST || prot.spec == SHORTEST_CAPITAL) {
    choice = &print_shortest;
    printed = get_shortest_length(prot);
  } else if (prot.spec == STRING && prot.length == NO_LENGTH) {
    choice = &print_string;
    printed = get_string_length(prot);
  } else if (prot.spec == STRING && prot.length == LONG_INT) {
    choice = &print_wstring;
    printed = get_string_length(prot);
  } else if (prot.spec == POINTER) {
    choice = &print_pointer;
    printed = get_pointer_length(prot);
  } else if (prot.spec == PERCENT) {
    choice = &print_percent;
    printed = 1;
  } else if (prot.spec == N_COUNTER) {
    choice = &get_n;
    printed = 0;
  }
  justify_modifier(cur, prot, printed, choice);
}

void justify_modifier(char** cur, Prototype prot, int printed,
                      void (*fun)(char**, Prototype)) {
  if (prot.width > (unsigned int)printed) {
    if (prot.flags.left_justify) {
      fun(cur, prot);
      for (int i = 0; i < (int)prot.width - printed; ++i)
        print_character(cur, ' ');
    } else {
      for (int i = 0; i < (int)prot.width - printed; ++i) {
        print_character(cur, ' ');
      }
      fun(cur, prot);
    }
  } else {
    fun(cur, prot);
  }
}

void determine_cast_print(Prototype* prot, va_list args) {
  if (prot->width == PARAMETER_WIDTH) {
    prot->width = va_arg(args, unsigned int);
  }
  if (prot->precision == PARAMETER_PRECISION) {
    prot->precision = va_arg(args, int);
  }

  if (prot->spec == SYM && prot->length == NO_LENGTH) {
    prot->value.c = va_arg(args, int);
  } else if (prot->spec == SYM && prot->length == LONG_INT) {
    prot->value.lc = va_arg(args, wchar_t);
  } else if ((prot->spec == DECIMAL || prot->spec == IDECIMAL) &&
             (prot->length == SHORT_INT || prot->length == NO_LENGTH)) {
    prot->value.ld = va_arg(args, int);
  } else if ((prot->spec == DECIMAL || prot->spec == IDECIMAL) &&
             prot->length == LONG_INT) {
    prot->value.ld = va_arg(args, long int);
  } else if ((prot->spec == UNSIGNED || prot->spec == OCTAL ||
              prot->spec == HEXADECIMAL || prot->spec == HEXADECIMAL_CAPITAL) &&
             (prot->length == SHORT_INT || prot->length == NO_LENGTH)) {
    prot->value.lu = va_arg(args, unsigned int);
  } else if ((prot->spec == UNSIGNED || prot->spec == OCTAL ||
              prot->spec == HEXADECIMAL || prot->spec == HEXADECIMAL_CAPITAL) &&
             prot->length == LONG_INT) {
    prot->value.lu = va_arg(args, unsigned long int);
  } else if ((prot->spec == FLOAT || prot->spec == SCIENTIFIC ||
              prot->spec == SCIENTIFIC_CAPITAL || prot->spec == SHORTEST ||
              prot->spec == SHORTEST_CAPITAL) &&
             (prot->length == LONG_INT || prot->length == NO_LENGTH)) {
    prot->value.Lf = va_arg(args, double);
  } else if ((prot->spec == FLOAT || prot->spec == SCIENTIFIC ||
              prot->spec == SCIENTIFIC_CAPITAL || prot->spec == SHORTEST ||
              prot->spec == SHORTEST_CAPITAL) &&
             prot->length == LONG_DOUBLE) {
    prot->value.Lf = va_arg(args, long double);
  } else if (prot->spec == STRING && prot->length == NO_LENGTH) {
    prot->value.s = va_arg(args, char*);
  } else if (prot->spec == STRING && prot->length == LONG_INT) {
    prot->value.ls = va_arg(args, wchar_t*);
  } else if (prot->spec == POINTER) {
    prot->value.p = va_arg(args, void*);
  } else if (prot->spec == PERCENT) {
    prot->value.c = '%';
  } else if (prot->spec == N_COUNTER) {
    prot->value.n = va_arg(args, int*);
  }
}
