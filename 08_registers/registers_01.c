#include <limits.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1000 * 1000
#define TOTAL_REG 26 * 26 * 26

void fill_buf(const char *filename, char buf[]) {
  FILE *fp = fopen(filename, "r");
  size_t bytes_read = fread(buf, 1, BUF_SIZE, fp);
  buf[bytes_read] = '\0';
  fclose(fp);
}

enum Direction {
  INC,
  DEC,
  NUM_DIRECTIONS,
};

const char *direction_str[NUM_DIRECTIONS] = {"inc", "dec"};

enum Direction dir_compare_str(const char *s) {
  for (int i = 0; i < NUM_DIRECTIONS; i++) {
    if (strcmp(s, direction_str[i]) == 0) {
      return (enum Direction)i;
    }
  }
  return NUM_DIRECTIONS;
}

int modify(int a, int b, enum Direction mod) {
  switch (mod) {
  case INC:
    return a + b;
  case DEC:
    return a - b;
  default:
    if (b == 0) {
      return a + 1;
    } else {
      return a;
    }
  }
}
enum Comparison {
  LT,
  GT,
  LEQ,
  GEQ,
  EQ,
  NEQ,
  NUM_COMPARISONS,
};

const char *comparison_str[NUM_COMPARISONS] = {
    "<", ">", "<=", ">=", "==", "!="};

enum Comparison cmp_compare_str(const char *s) {
  for (int i = 0; i < NUM_COMPARISONS; i++) {
    if (strcmp(s, comparison_str[i]) == 0) {
      return (enum Direction)i;
    }
  }
  return NUM_COMPARISONS;
}

int compare(int a, int b, enum Comparison cmp) {
  switch (cmp) {
  case LT:
    return a < b;
  case GT:
    return a > b;
  case LEQ:
    return a <= b;
  case GEQ:
    return a >= b;
  case EQ:
    return a == b;
  case NEQ:
    return a != b;
  default:
    return 2;
  }
}

struct line_info {
  int reg0;
  enum Direction dir;
  int dir_amount;
  int reg1;
  enum Comparison cmp;
  int cmp_amount;
};

struct line_info parse_line(char *p) {
  struct line_info line;
  int index = 0;
  char small_buf[10];
  int neg_flag = 0;

  while (*p != ' ') {
    index = index * 26 + (*p - 'a');
    p++;
  }
  line.reg0 = index;

  p++;
  index = 0;

  while (*p != ' ') {
    small_buf[index] = *p;
    p++;
    index++;
  }
  small_buf[index] = '\0';
  line.dir = dir_compare_str(small_buf);

  p++;
  index = 0;

  while (*p != ' ') {
    if (*p == '-') {
      neg_flag = 1;
    } else {
      index = index * 10 + (*p - '0');
    }
    p++;
  }
  if (neg_flag == 1) {
    index = -index;
  }
  line.dir_amount = index;
  neg_flag = 0;

  p += 4;
  index = 0;

  while (*p != ' ') {
    index = index * 26 + (*p - 'a');
    p++;
  }
  line.reg1 = index;

  p++;
  index = 0;

  while (*p != ' ') {
    small_buf[index] = *p;
    p++;
    index++;
  }
  small_buf[index] = '\0';
  line.cmp = cmp_compare_str(small_buf);

  p++;
  index = 0;

  while (*p != ' ' && *p != '\n') {
    if (*p == '-') {
      neg_flag = 1;
    } else {
      index = index * 10 + (*p - '0');
    }
    p++;
  }
  if (neg_flag == 1) {
    index = -index;
  }
  line.cmp_amount = index;
  return line;
}

void move_registers(struct line_info line, int registers[]) {
  int register_value = registers[line.reg1];
  if (compare(register_value, line.cmp_amount, line.cmp) == 0) {
    return;
  }
  register_value = registers[line.reg0];
  registers[line.reg0] = modify(register_value, line.dir_amount, line.dir);
}

int max_register(const char *filename) {
  char buf[BUF_SIZE];
  fill_buf(filename, buf);
  int registers[TOTAL_REG] = {0};
  char *p = buf;
  while (*p != '\0') {
    struct line_info line = parse_line(p);
    move_registers(line, registers);
    while (*p != '\n') {
      p++;
    }
    p++;
  }
  int answer = INT_MIN;
  for (int i = 0; i < TOTAL_REG; i++) {
    if (registers[i] > answer) {
      answer = registers[i];
    }
  }
  return answer;
}

int main(void) {
  int answer = max_register("small_input.txt");
  printf("Calculated answer is %d, correct answer is 1\n", answer);
  answer = max_register("input.txt");
  printf("Calculated answer is %d, correct answer is ???\n", answer);
}
