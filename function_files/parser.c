#include "../s21_string.h"
#include "../utility.h"

int parse_prototype(char **pos, Prototype *prot, va_list args,
                    Function_type type) {
  char *next_pos = *pos;
  int result = 0;
  Prototype updated_prot = {{0}, 0,          -1, NO_LENGTH, NO_SPECIFICATOR,
                            {0}, prot->start};

  ++next_pos;  // starting with "%"

  if (!s21_strpbrk(next_pos, SPECIFICATOR_CHARS)) {
    // if no specificator is specified
    result = 1;
  }
  if (!result) {
    result = parse_flags(&next_pos, &updated_prot);
  }
  if (!result) {
    result = parse_width(&next_pos, &updated_prot, type);
  }
  if (!result && type == S21_SPRINTF) {
    result = parse_precision(&next_pos, &updated_prot);
  }
  if (!result) {
    result = parse_length(&next_pos, &updated_prot);
  }
  if (!result) {
    result = parse_specificator(&next_pos, &updated_prot);
  }

  if (!result) {
    *pos = next_pos - 1;  // loop breaks without subtracting 1
    *prot = updated_prot;
    if (type == S21_SPRINTF) {
      determine_cast_print(prot, args);
    } else if (type == S21_SSCANF) {
      determine_cast_scan(prot, args);
    }
  }
  return result;
}

int parse_flags(char **pos, Prototype *prot) {
  char *next_pos;
  Prototype updated_prot = *prot;
  int result = 0;

  next_pos = s21_strpbrk(*pos, WIDTH_PRECISION_LENGTH_SPECIFICATOR_CHARS);
  for (char *cur = *pos; cur < next_pos; ++cur) {
    if (*cur == '-') {
      updated_prot.flags.left_justify = 1;
    } else if (*cur == '+') {
      updated_prot.flags.sign = 1;
      updated_prot.flags.space = 0;
    } else if (*cur == ' ') {
      updated_prot.flags.space = updated_prot.flags.sign ? 0 : 1;
    } else if (*cur == '#') {
      updated_prot.flags.hash = 1;
    } else if (*cur == '0') {
      updated_prot.flags.zero_padding = 1;
    } else {
      result = 1;
    }
  }

  if (!result) {
    *pos = next_pos;
    *prot = updated_prot;
  }
  return result;
}

int parse_width(char **pos, Prototype *prot, Function_type type) {
  char *next_pos;
  int result = 0;
  unsigned int width = 0;
  int precision = 0;

  next_pos = s21_strpbrk(*pos, PRECISION_LENGTH_SPECIFICATOR_CHARS);
  for (char *cur = *pos; cur < next_pos; ++cur) {
    if (*cur >= '0' && *cur <= '9') {
      width *= 10;
      width += *cur - 0x30;
    } else if (*cur == '*' && type == S21_SPRINTF) {
      width = PARAMETER_WIDTH;
    } else if (*cur == '*' && type == S21_SSCANF) {
      precision = PARAMETER_PRECISION;
    } else {
      result = 1;
    }
  }

  if (!result) {
    *pos = next_pos;
    prot->width = width;
    prot->precision = precision;
  }
  return result;
}

int parse_precision(char **pos, Prototype *prot) {
  char *next_pos = *pos;
  int result = 0;
  int precision = -1;

  if (**pos == '.') {
    char *dot_pos = *pos + 1;
    precision = 0;
    next_pos = s21_strpbrk(dot_pos, LENGTH_SPECIFICATOR_CHARS);
    for (char *cur = dot_pos; cur < next_pos; ++cur) {
      if (*cur >= '0' && *cur <= '9') {
        precision *= 10;
        precision += *cur - 0x30;
      } else if (*cur == '*') {
        precision = PARAMETER_PRECISION;
      } else {
        result = 1;
      }
    }
  }

  if (!result) {
    *pos = next_pos;
    prot->precision = precision;
  }
  return result;
}

int parse_length(char **pos, Prototype *prot) {
  char *next_pos = *pos + 1;
  Prototype updated_prot = *prot;

  if (**pos == 'h') {
    updated_prot.length = SHORT_INT;
  } else if (**pos == 'l') {
    updated_prot.length = LONG_INT;
  } else if (**pos == 'L') {
    updated_prot.length = LONG_DOUBLE;
  }

  if (updated_prot.length != NO_LENGTH) {
    *pos = next_pos;
    *prot = updated_prot;
  }
  return 0;
}

int parse_specificator(char **pos, Prototype *prot) {
  char *next_pos = *pos + 1;
  Prototype updated_prot = *prot;
  int result = 0;

  if (**pos == 'c') {
    updated_prot.spec = SYM;
  } else if (**pos == 'd') {
    updated_prot.spec = DECIMAL;
  } else if (**pos == 'i') {
    updated_prot.spec = IDECIMAL;
  } else if (**pos == 'e') {
    updated_prot.spec = SCIENTIFIC;
  } else if (**pos == 'E') {
    updated_prot.spec = SCIENTIFIC_CAPITAL;
  } else if (**pos == 'f') {
    updated_prot.spec = FLOAT;
  } else if (**pos == 'g') {
    updated_prot.spec = SHORTEST;
  } else if (**pos == 'G') {
    updated_prot.spec = SHORTEST_CAPITAL;
  } else if (**pos == 'o') {
    updated_prot.spec = OCTAL;
  } else if (**pos == 's') {
    updated_prot.spec = STRING;
  } else if (**pos == 'u') {
    updated_prot.spec = UNSIGNED;
  } else if (**pos == 'x') {
    updated_prot.spec = HEXADECIMAL;
  } else if (**pos == 'X') {
    updated_prot.spec = HEXADECIMAL_CAPITAL;
  } else if (**pos == 'p') {
    updated_prot.spec = POINTER;
  } else if (**pos == 'n') {
    updated_prot.spec = N_COUNTER;
  } else if (**pos == '%') {
    updated_prot.spec = PERCENT;
  } else {
    result = 1;
  }

  if (!result) {
    *pos = next_pos;
    *prot = updated_prot;
  }
  return result;
}
