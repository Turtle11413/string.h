# string.h

| Function | Description |
| ------ | ------ |
| void *memchr(const void *str, int c, size_t n) | Searches for the first occurrence of the character c (an unsigned char) in the first n bytes of the string pointed to, by the argument str. |
| int memcmp(const void *str1, const void *str2, size_t n) | Compares the first n bytes of str1 and str2. |
| void *memcpy(void *dest, const void *src, size_t n) | Copies n characters from src to dest. |
| void *memset(void *str, int c, size_t n) | Copies the character c (an unsigned char) to the first n characters of the string pointed to, by the argument str. |
| char *strncat(char *dest, const char *src, size_t n) | Appends the string pointed to, by src to the end of the string pointed to, by dest up to n characters long. |
| char *strchr(const char *str, int c) | Searches for the first occurrence of the character c (an unsigned char) in the string pointed to, by the argument str. |
| int strncmp(const char *str1, const char *str2, size_t n) | Compares at most the first n bytes of str1 and str2. |
| char *strncpy(char *dest, const char *src, size_t n) | Copies up to n characters from the string pointed to, by src to dest. |
| size_t strcspn(const char *str1, const char *str2) | Calculates the length of the initial segment of str1 which consists entirely of characters not in str2. |
| char *strerror(int errnum) | Searches an internal array for the error number errnum and returns a pointer to an error message string. You need to declare macros containing arrays of error messages for mac and linux operating systems. Error descriptions are available in the original library. Checking the current OS is carried out using directives. |
| size_t strlen(const char *str) | Computes the length of the string str up to but not including the terminating null character. |
| char *strpbrk(const char *str1, const char *str2) | Finds the first character in the string str1 that matches any character specified in str2. |
| char *strrchr(const char *str, int c) | Searches for the last occurrence of the character c (an unsigned char) in the string pointed to by the argument str. |
| char *strstr(const char *haystack, const char *needle) | Finds the first occurrence of the entire string needle (not including the terminating null character) which appears in the string haystack. |
| char *strtok(char *str, const char *delim) | Breaks string str into a series of tokens separated by delim. |
| void *to_upper(const char *str) | Returns a copy of string (str) converted to uppercase. |
| void *to_lower(const char *str) | Returns a copy of string (str) converted to lowercase. |
| void *insert(const char *src, const char *str, size_t start_index) | Returns a new string in which a specified string (str) is inserted at a specified index position (start_index) in the given string (src). |
| void *trim(const char *src, const char *trim_chars) | Returns a new string in which all leading and trailing occurrences of a set of specified characters (trim_chars) from the given string (src) are removed. |



## install
```bash
git clone git@github.com:Turtle11413/string.h.git
cd string.h
```

## build
```bash
make s21_string.a
```
