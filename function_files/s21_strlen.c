#include "../s21_string.h"

s21_size_t s21_strlen(const char *str) {
  s21_size_t index = 0;
  while (str[index] != '\0') {
    ++index;
  }
  return index;
}