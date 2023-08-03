#include "../s21_string.h"

int s21_memcmp(const void* str1, const void* str2, s21_size_t n) {
  int result_value = 0;
  const char* tmp_str1 = str1;
  const char* tmp_str2 = str2;

  for (s21_size_t i = 0; i < n; ++i) {
    result_value = tmp_str1[i] - tmp_str2[i];
    if (result_value != 0) {
      break;
    }
  }

  return result_value;
}