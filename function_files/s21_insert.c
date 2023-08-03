#include <stdio.h>
#include <stdlib.h>

#include "../s21_string.h"
void* s21_insert(const char* src, const char* str, s21_size_t start_index) {
  char* result = S21_NULL;
  s21_size_t len_src = s21_strlen(src);
  s21_size_t len_str = (str != S21_NULL) ? s21_strlen(str) : 0;

  if ((start_index <= len_src) && (len_str != 0) && (str != S21_NULL)) {
    result = (char*)malloc((len_src + len_str + 1) * sizeof(char));
  }

  if (result) {
    for (s21_size_t i = 0, index_src = 0, index_str = 0; i < len_str + len_src;
         i++) {
      if (i < start_index || i >= start_index + len_str) {
        result[i] = src[index_src];
        index_src++;
      } else {
        result[i] = str[index_str];
        index_str++;
      }
    }

    result[len_str + len_src] = '\0';
  }

  else if ((len_str == 0) && (start_index <= len_src)) {
    result = (char*)malloc((len_src + 1) * sizeof(char));
    s21_strncpy(result, src, len_src);
    result[len_src] = '\0';
  }

  return result;
}
