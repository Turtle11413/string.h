#include <string.h>

#include "../s21_string.h"
#include "../utility.h"

// floats

int scan_float(char** cur, Prototype prot) {
  int result = 1;
  int sign = 1;
  char* next_pos = *cur;
  long double integral = 0, quotient = 0, value = 0;
  int exp = 0;

  if (!s21_strpbrk(next_pos, DECIMAL_ALPHABET)) {
    result = 0;
  }
  if (result) {
    result = scan_sign(&next_pos, &sign);
  }
  if (result) {
    result = scan_integral(&next_pos, &integral);
  }
  if (result) {
    result = scan_quotient(&next_pos, &quotient);
  }
  if (result) {
    result = scan_exponent(&next_pos, &exp);
  }
  if (result) {
    value = sign * (integral + quotient) * powl(10, exp);
  }

  if (prot.value.p == S21_NULL) result = 0;
  if (result) {
    cast_float(prot, value);
  }
  *cur = next_pos;

  return result;
}

int scan_integral(char** pos, long double* num) {
  int result = 1;

  for (; !s21_strchr(".eE", **pos) && **pos && result; ++(*pos)) {
    int digit;
    result = get_digit_from_char(**pos, 10, &digit);
    *num *= 10;
    *num += digit;
  }

  return result;
}

int scan_quotient(char** pos, long double* num) {
  int result = 1;
  long double q = 0;

  if (**pos == '.') {
    ++(*pos);
    for (; !s21_strchr("eE", **pos) && **pos && result; ++(*pos)) {
      int digit;
      result = get_digit_from_char(**pos, 10, &digit);
      q *= 10;
      q += digit;
    }
    *num = q / powl(10, get_pure_float_length(q));
  }

  return result;
}

int scan_exponent(char** pos, int* exp) {
  int result = 1;
  int sign = 1;
  char* next_pos = *pos;
  unsigned long lexp = 0;

  if (s21_strchr("eE", *next_pos)) {
    ++next_pos;
    scan_sign(&next_pos, &sign);
    if (next_pos) {
      result = scan_decimal(&next_pos, &lexp, 10);
    }
  }

  if (result && next_pos) {
    *pos = next_pos;
    *exp = ((int)lexp) * sign;
  }

  return result;
}

void cast_float(Prototype prot, long double value) {
  if (prot.length == NO_LENGTH) {
    *(float*)(prot.value.p) = (float)value;
  } else if (prot.length == LONG_INT) {
    *(double*)(prot.value.p) = (double)value;
  } else if (prot.length == LONG_DOUBLE) {
    *(long double*)(prot.value.p) = value;
  }
}

// integers

int scan_int(char** cur, Prototype prot) {
  int result = 1;
  union Value value = {0};
  int sign = 1;
  char* alph = DECIMAL_ALPHABET;
  alph = prot.spec == OCTAL ? OCTAL_ALPHABET : alph;
  alph = prot.spec == HEXADECIMAL || prot.spec == HEXADECIMAL_CAPITAL ||
                 prot.spec == IDECIMAL || prot.spec == POINTER
             ? HEXADECIMAL_ALPHABET_ALL
             : alph;
  char* next_pos = *cur;
  int base;
  if (prot.spec != IDECIMAL) base = get_base(prot);

  if (!s21_strpbrk(next_pos, alph)) {
    result = 0;
  }
  if (result) {
    result = scan_sign(&next_pos, &sign);
  }
  if (result) {
    result = scan_prefix(&next_pos, &base, prot.spec == IDECIMAL);
  }
  if (result) {
    result = scan_decimal(&next_pos, &value.lu, base);
  }
  if (result && sign == -1) {
    value.ld = -((long)value.lu);
  }

  if (prot.value.p == S21_NULL) result = 0;
  if (result) {
    cast_int(prot, value);
  }
  *cur = next_pos;

  return result;
}

int scan_decimal(char** pos, unsigned long* num, int base) {
  int result = 1;

  for (; **pos && result; ++(*pos)) {
    int digit;
    result = get_digit_from_char(**pos, base, &digit);
    *num *= base;
    *num += digit;
  }

  return result;
}

int get_digit_from_char(char c, int base, int* digit) {
  int result = 1;
  char* alph = DECIMAL_ALPHABET;
  alph = base == 8 ? OCTAL_ALPHABET : alph;
  alph = base == 16 ? HEXADECIMAL_ALPHABET_ALL : alph;

  if (strchr(alph, c)) {
    if (c >= '0' && c <= '9') {
      *digit = c - 0x30;
    } else if (c >= 'A' && c <= 'F') {
      *digit = c - 0x37;
    } else if (c >= 'a' && c <= 'f') {
      *digit = c - 0x57;
    }
  } else {
    result = 0;
  }

  return result;
}

int scan_sign(char** pos, int* sign) {
  char* next_pos;
  int result = 1;

  next_pos = s21_strpbrk(*pos, HEXADECIMAL_ALPHABET_ALL);
  if (next_pos - *pos > 1 && (**pos != '+' && **pos != '-')) {
    result = 0;
  } else if (**pos == '-') {
    *sign = -1;
  } else {
    *sign = 1;
  }

  if (next_pos == S21_NULL) next_pos = *pos + 1;
  if (result) *pos = next_pos;

  return result;
}

int scan_prefix(char** pos, int* base, int change_base) {
  char* next_pos = *pos;
  int result = 1;
  int new_base = 10;

  if (*next_pos == '0') {
    if (*(next_pos + 1) == 'x' || *(next_pos + 1) == 'X') {
      new_base = 16;
      *pos = next_pos + 2;
    } else {
      new_base = 8;
      *pos = next_pos + 1;
    }
  }

  if (change_base) *base = new_base;
  return result;
}

void cast_int(Prototype prot, union Value value) {
  if (prot.spec == DECIMAL || prot.spec == IDECIMAL) {
    if (prot.length == NO_LENGTH) {
      *(int*)(prot.value.p) = (int)value.ld;
    } else if (prot.length == SHORT_INT) {
      *(short int*)(prot.value.p) = (short int)value.ld;
    } else if (prot.length == LONG_INT) {
      *(long*)(prot.value.p) = (long)value.ld;
    }
  } else if (prot.spec == UNSIGNED || prot.spec == OCTAL ||
             prot.spec == HEXADECIMAL || prot.spec == HEXADECIMAL_CAPITAL) {
    if (prot.length == NO_LENGTH) {
      *(unsigned int*)(prot.value.p) = (unsigned int)value.lu;
    } else if (prot.length == SHORT_INT) {
      *(unsigned short int*)(prot.value.p) = (unsigned short int)value.lu;
    } else if (prot.length == LONG_INT) {
      *(unsigned long*)(prot.value.p) = (unsigned long)value.lu;
    }
  } else if (prot.spec == POINTER) {
    *(unsigned long*)(prot.value.p) =
        (unsigned long)value.lu;  // void is not assignable lmao
  }
}

// s

int scan_s(char** cur, Prototype prot) {
  int result = 1;

  int length = prot.width == 0 ? s21_strlen(*cur) : prot.width;
  *cur += length;
  if (prot.value.p != S21_NULL) {
    char* dest = (char*)(prot.value.p);
    s21_strncpy(dest, *cur, length);
    dest[length] = 0;
  } else {
    result = 0;
  }

  return result;
}

// c

int scan_c(char** cur, Prototype prot) {
  int result = 1;
  char c = 0;
  wchar_t wc = 0;

  if (prot.length == NO_LENGTH) {
    c = scan_character(cur);
    result = c == 0 ? 0 : 1;
    *cur = c == 0 ? *cur - 1 : *cur;
  } else if (prot.length == LONG_INT) {
    c = scan_wc(cur);
    result = c == 0 ? 0 : 1;
    *cur = c == 0 ? *cur - 1 : *cur;
  }

  if (prot.value.p != S21_NULL && result) {
    if (prot.length == NO_LENGTH) {
      *(char*)(prot.value.p) = c;
    } else if (prot.length == LONG_INT) {
      *(wchar_t*)(prot.value.p) = wc;
    }
  } else {
    result = 0;
  }

  return result;
}

wchar_t scan_wc(char** cur) {
  wchar_t c = **(wchar_t**)cur;
  ++(*cur);

  return c;
}

char scan_character(char** cur) {
  char c = **cur;
  ++(*cur);

  return c;
}

// n

void backtrace_zero(char* start, char** cur) {
  if (*cur != start) {
    while ((!(*(*cur - 1)) || s21_strchr(DELIMITERS, *(*cur - 1))) &&
           *cur != start)
      --(*cur);
  }
}

// sscanf

int s21_sscanf(const char* str, const char* format, ...) {
  int result = 0;
  char* cur;

  va_list args;
  va_start(args, format);

  if (str) {
    int full_length = s21_strlen(str);
    char str_cpy[full_length];
    s21_strncpy(str_cpy, str, full_length + 1);
    str_cpy[full_length] = '\0';
    cur = strtok(str_cpy, DELIMITERS);
    for (char* pos = (char*)format; *pos; ++pos) {
      Prototype prot = {{0}, 0, -1, NO_LENGTH, NO_SPECIFICATOR, {0}, str_cpy};
      if (*pos == '%' && !parse_prototype(&pos, &prot, args, S21_SSCANF) &&
          cur != S21_NULL && prot.spec != N_COUNTER) {
        unsigned int cur_len = s21_strlen(cur);  // length of current word
        char cur_str[cur_len + 1];               // a string to parse
        char* cur_cur_str = cur_str;             // cursor to that string
        unsigned int change =
            cur_len < prot.width ? cur_len : prot.width;  // the smallest
        change =
            prot.width == 0 ? cur_len : change;  // prot.width = 0 => no width
        s21_strncpy(cur_str, cur, change);
        cur_str[change] = '\0';  // strncpy doesn't null-terminate
        result += scan_prototype(&cur_cur_str, prot);
        change = cur_cur_str - cur_str;  // how much we've read from that
        cur = change == cur_len
                  ? strtok(S21_NULL, DELIMITERS)
                  : cur + change;  // don't get new word if this one is still
                                   // not fully processed
      } else if (prot.spec == N_COUNTER) {
        if (cur == S21_NULL) {
          cur = str_cpy + full_length;
        }
        char* backtrace = cur;
        backtrace_zero(str_cpy, &backtrace);
        result += scan_prototype(&backtrace, prot);
      }
    }
  } else {
    result = -1;
  }

  va_end(args);
  return result;
}

void determine_cast_scan(Prototype* prot, va_list args) {
  if (prot->precision != PARAMETER_PRECISION && prot->spec != PERCENT &&
      prot->spec != N_COUNTER) {
    prot->value.p = va_arg(args, void*);
  } else if (prot->spec == N_COUNTER) {
    prot->value.n = va_arg(args, int*);
  }
}

int scan_prototype(char** cur, Prototype prot) {
  int result = 0;
  if (prot.spec == SYM) {
    result = scan_c(cur, prot);
  } else if (prot.spec == DECIMAL || prot.spec == IDECIMAL ||
             prot.spec == OCTAL || prot.spec == HEXADECIMAL ||
             prot.spec == HEXADECIMAL_CAPITAL || prot.spec == POINTER ||
             prot.spec == UNSIGNED) {
    result = scan_int(cur, prot);
  } else if (prot.spec == FLOAT || prot.spec == SCIENTIFIC ||
             prot.spec == SCIENTIFIC_CAPITAL || prot.spec == SHORTEST ||
             prot.spec == SHORTEST_CAPITAL) {
    result = scan_float(cur, prot);
  } else if (prot.spec == STRING) {
    result = scan_s(cur, prot);
  } else if (prot.spec == N_COUNTER) {
    get_n(cur, prot);
  }

  return result;
}
