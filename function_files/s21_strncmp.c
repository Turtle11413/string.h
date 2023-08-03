#include "../s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int flag = 0;
  int diff = 0;
  for (s21_size_t i = 0; i < n && !flag; ++i) {
    if (str1[i] != str2[i]) {
      flag = 1;
      diff = str1[i] - str2[i];
    }
  }
  if (!flag) diff = 0;
  return diff;
}
