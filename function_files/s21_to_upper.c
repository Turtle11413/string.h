#include <stdlib.h>

#include "../s21_string.h"

void *s21_to_upper(const char *str) {
  char *result = NULL;
  if (str) {
    s21_size_t len = s21_strlen(str);
    result = malloc((len + 1) * sizeof(char));
    if (result) {
      for (s21_size_t i = 0; i <= len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
          result[i] = (str[i] - 'a') + 'A';
        } else {
          result[i] = str[i];
        }
      }
      result[len] = '\0';
    }
  }
  return result;
}
