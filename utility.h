#ifndef S_21_STRING_UTILITY_H
#define S_21_STRING_UTILITY_H

#include <math.h>
#include <stdarg.h>
#include <wchar.h>

typedef enum {
  NO_TYPE,
  S21_SPRINTF,
  S21_SSCANF,
} Function_type;

enum Specificator {
  NO_SPECIFICATOR,
  SYM,
  DECIMAL,
  IDECIMAL,
  SCIENTIFIC,
  SCIENTIFIC_CAPITAL,
  FLOAT,
  SHORTEST,
  SHORTEST_CAPITAL,
  OCTAL,
  STRING,
  UNSIGNED,
  HEXADECIMAL,
  HEXADECIMAL_CAPITAL,
  POINTER,
  N_COUNTER,
  PERCENT,
};

union Value {
  long ld;
  unsigned long lu;
  long double Lf;
  char c;
  char* s;
  wchar_t lc;
  wchar_t* ls;
  void* p;
  int* n;
};

enum Length {
  NO_LENGTH,
  SHORT_INT,
  LONG_INT,
  LONG_DOUBLE,
};

struct Flags {
  unsigned int left_justify : 1;
  unsigned int sign : 1;
  unsigned int space : 1;
  unsigned int hash : 1;
  unsigned int zero_padding : 1;
};

typedef struct {
  struct Flags flags;
  unsigned int width;
  int precision;
  enum Length length;
  enum Specificator spec;
  union Value value;
  char* start;
} Prototype;

typedef struct {
  long double z;
  long double q;
  int leading_zeroes;
} Split_float;

#define WIDTH_PRECISION_LENGTH_SPECIFICATOR_CHARS \
  "123456789*.hlLcdieEfgGosuxXpn%"
#define PRECISION_LENGTH_SPECIFICATOR_CHARS ".hlLcdieEfgGosuxXpn%"
#define LENGTH_SPECIFICATOR_CHARS "hlLcdieEfgGosuxXpn%"
#define SPECIFICATOR_CHARS "cdieEfgGosuxXpn%"

#define UNSIGNED_LONG_MAX_LENGTH 22
#define DOUBLE_MAX_LENGTH 310
#define FLOAT_DEFAULT_PRECISION 6

#define HEXADECIMAL_ALPHABET "0123456789abcdef"
#define HEXADECIMAL_ALPHABET_CAPITAL "0123456789ABCDEF"

#define PARAMETER_WIDTH ((unsigned int)-1)
#define PARAMETER_PRECISION (-2)

// S21_SPRINTF
// c
void print_c(char** cur, Prototype prot);
void print_wc(char** cur, Prototype prot);
void print_wcharacter(char** cur, wchar_t c);

// d, u, o, x, X
void print_decimal(char** cur, Prototype prot);
void convert_ld_to_lu(Prototype* prot, int* sign);
void print_lu(char** cur, Prototype prot);
int get_decimal_length(Prototype prot);
int get_pure_decimal_length(unsigned long num, int base);
int get_base(Prototype prot);
void print_prefix(char** cur, Prototype prot);

// f
void print_float(char** cur, Prototype prot);
void split_float(long double f, Split_float* split, int precision);
int get_float_length(Prototype prot);
void print_Lf(char** cur, long double num);
int get_pure_float_length(long double num);

// e, E
void print_scientific(char** cur, Prototype prot);
int normalize(long double* num);
int get_scientific_length(Prototype prot);

// g, G
void print_shortest(char** cur, Prototype prot);
int get_shortest_length(Prototype prot);
int remove_trailing_zeroes(Prototype prot);

// s
void print_string(char** cur, Prototype prot);
void print_wstring(char** cur, Prototype prot);
int get_string_length(Prototype prot);

// p
void print_pointer(char** cur, Prototype prot);
int get_pointer_length(Prototype prot);

// %
void print_precent(char** cur, Prototype prot);

// n
void get_n(char** cur, Prototype prot);

// sprintf essentials
void justify_modifier(char** cur, Prototype prot, int printed,
                      void (*fun)(char**, Prototype));
void print_prototype(char** cur, Prototype prot);
void print_character(char** cur, char c);
void determine_cast_print(Prototype* prot, va_list args);

// S21_SSCANF

#define DELIMITERS " \n\t"

#define OCTAL_ALPHABET "01234567"
#define DECIMAL_ALPHABET "0123456789"
#define HEXADECIMAL_ALPHABET_ALL "0123456789abcdefABCDEF"

// s
int scan_s(char** cur, Prototype prot);

// c
int scan_c(char** cur, Prototype prot);
wchar_t scan_wc(char** cur);
char scan_character(char** cur);

// integers
int scan_int(char** cur, Prototype prot);
void cast_int(Prototype prot, union Value value);
int scan_sign(char** pos, int* sign);
int scan_prefix(char** pos, int* base, int change_base);
int scan_decimal(char** pos, unsigned long* num, int base);
int get_digit_from_char(char c, int base, int* digit);

// floats
int scan_float(char** cur, Prototype prot);
int scan_integral(char** pos, long double* num);
int scan_quotient(char** pos, long double* num);
int scan_exponent(char** pos, int* exp);
void cast_float(Prototype prot, long double value);

// n
void backtrace_zero(char* start, char** cur);

// sscanf essentials
int scan_prototype(char** cur, Prototype prot);
void determine_cast_scan(Prototype* prot, va_list args);

// PARSER

int parse_prototype(char** pos, Prototype* prot, va_list args,
                    Function_type type);
int parse_flags(char** pos, Prototype* prot);
int parse_width(char** pos, Prototype* prot, Function_type type);
int parse_precision(char** pos, Prototype* prot);
int parse_length(char** pos, Prototype* prot);
int parse_specificator(char** pos, Prototype* prot);

// ADDONS

size_t s21_strspn(const char *str, const char *delim);


#endif
