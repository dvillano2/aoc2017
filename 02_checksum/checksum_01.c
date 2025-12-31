#include <limits.h>
#include <stdio.h>

FILE *open_file(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "could not open file\n");
  }
  return fp;
}

int get_next_digit(FILE *fp) {
  int c = getc(fp);
  if (c == ' ') {
    return -1;
  }
  if (c == '\n') {
    return -2;
  }
  if (c == EOF) {
    return -3;
  }
  return c - '0';
}

int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

int min(int a, int b) {
  if (a < b) {
    return a;
  }
  return b;
}

int get_checksum(const char *filename) {
  FILE *fp = open_file(filename);
  int checksum = 0;
  int number = 0;
  int max_number = INT_MIN;
  int min_number = INT_MAX;
  int high = INT_MIN;
  int low = INT_MAX;
  int c = get_next_digit(fp);
  while (c != -3) {
    if (c >= 0) {
      number = number * 10 + c;
      max_number = number;
      min_number = number;
    } else {
      low = min(min_number, low);
      high = max(max_number, high);
      if (c == -2) {
        checksum += (high - low);
        high = INT_MIN;
        low = INT_MAX;
      }
      number = 0;
      min_number = INT_MAX;
      max_number = INT_MIN;
    }
    c = get_next_digit(fp);
  }
  fclose(fp);
  return checksum;
}
int main(void) {
  int answer = get_checksum("input.txt");
  printf("answer is %d\n", answer);
  return 0;
}
