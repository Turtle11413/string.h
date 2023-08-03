#include "../s21_string.h"

char *s21_strchr(const char *str, int c) {
  s21_size_t index = 0;
  char *result = S21_NULL;
  while (str[index] != '\0' && str[index] != c) {
    ++index;
  }
  if (str[index] != '\0') {
    result = (char *)str + index;
  } else if (c == '\0') {
    result = (char *)str + index;
  }
  return result;
}
