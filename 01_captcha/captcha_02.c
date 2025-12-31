#include <stdio.h>

FILE *open_file(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "could not open file \n");
  }
  return fp;
}

int get_length(FILE *fp) {
  int count = 0;
  int c = getc(fp);
  while (c != EOF && c != '\n') {
    count++;
    c = getc(fp);
  }
  rewind(fp);
  return count;
}

int get_next_int(FILE *fp) {
  int c = getc(fp);
  if (c == EOF || c == '\n') {
    return -1;
  }
  return c - '0';
}

int looped_next_int(FILE *fp) {
  int c = get_next_int(fp);
  if (c == -1) {
    rewind(fp);
    c = get_next_int(fp);
  }
  return c;
}

int solve_capatcha(const char *filename) {
  int sum = 0;
  FILE *fp0 = open_file(filename);
  int length = get_length(fp0);
  FILE *fp1 = open_file(filename);
  fseek(fp1, length / 2, SEEK_SET);
  int c = get_next_int(fp0);
  int d = looped_next_int(fp1);
  while (c != -1) {
    if (c == d) {
      sum += c;
    }
    c = get_next_int(fp0);
    d = looped_next_int(fp1);
  }
  fclose(fp0);
  fclose(fp1);
  return sum;
}

int main(void) {
  int answer = solve_capatcha("input.txt");
  printf("answer is %d\n", answer);
  return 0;
}
