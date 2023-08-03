#include "../s21_string.h"

char* s21_strstr(const char* haystack, const char* needle) {
  s21_size_t len_needle = s21_strlen(needle);
  s21_size_t len_haystack = s21_strlen(haystack);
  char* result = S21_NULL;

  for (s21_size_t i = 0; (i < len_haystack) && (result == S21_NULL); ++i) {
    if (!s21_strncmp(haystack + i, needle, len_needle)) {
      result = (char*)(haystack + i);
    }
  }
  return result;
}
