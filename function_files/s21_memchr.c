#include "../s21_string.h"

void* s21_memchr(const void* str, int c, s21_size_t n) {
  void* result_ptr = S21_NULL;
  const char* tmp_ptr = str;

  for (s21_size_t i = 0; i < n; ++i) {
    if (tmp_ptr[i] == c) {
      result_ptr = (void*)(tmp_ptr + i);
      break;
    }
  }

  return result_ptr;
}
