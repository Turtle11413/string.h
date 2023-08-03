#include "../s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  int flag = 0;
  for (s21_size_t i = 0; i < n; ++i) {
    if (flag || src[i] == '\0') {
      dest[i] = '\0';
      flag = 1;
    } else {
      dest[i] = src[i];
    }
  }

  return dest;
}
