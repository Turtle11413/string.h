#include "../s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  s21_size_t counter = 0;
  char *result = S21_NULL;
  for (int i = 0; str1[i] != '\0' && !s21_strchr(str2, str1[i]); ++i) {
    ++counter;
  }
  if (str1[counter] != '\0') {
    result = (char *)str1 + counter;
  }
  return result;
}
