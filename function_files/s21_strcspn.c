#include "../s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t counter = 0;
  for (int i = 0; str1[i] != '\0' && !s21_strchr(str2, str1[i]); ++i) {
    ++counter;
  }
  return counter;
}