#include "../s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t lenSrc = s21_strlen(src);
  s21_size_t lenDest = s21_strlen(dest);
  if (lenSrc < n) n = lenSrc;
  for (s21_size_t i = lenDest; i < lenDest + n; ++i) {
    dest[i] = src[i - lenDest];
  }
  return dest;
}
