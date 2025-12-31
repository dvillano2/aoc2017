#include <stdio.h>
#include <stdlib.h>

FILE *open_file(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "Could not open file\n");
    exit(EXIT_FAILURE);
  }
  return fp;
}

int get_length(FILE *fp) {
  int c = getc(fp);
  int counter = 0;
  while (c != EOF) {
    if (c == '\n') {
      counter++;
    }
    c = getc(fp);
  }
  rewind(fp);
  return counter;
}

int str_to_int(const char *input) {
  int num = 0;
  int position = 0;
  int neg = 0;
  if (input[0] == '-') {
    neg = 1;
    position = 1;
  }
  char c = input[position];
  while (c != '\0') {
    int c_int = ((int)c) - ((int)'0');
    num = num * 10 + c_int;
    position++;
    c = input[position];
  }
  if (neg == 1) {
    num = -num;
  }
  return num;
}

int *make_array(FILE *fp, int length) {
  int *array = malloc(length * sizeof(int));
  char buffer[10];
  int buf_tracker = 0;
  int count = 0;
  int c = getc(fp);
  while (c != EOF) {
    if (c == '\n') {
      buffer[buf_tracker] = '\0';
      array[count] = str_to_int(buffer);
      buf_tracker = 0;
      count++;
    } else {
      buffer[buf_tracker] = (char)c;
      buf_tracker++;
      // printf("buff_tracker is %d\n", buf_tracker);
    }
    c = getc(fp);
  }
  return array;
}

int traverse_array(int *array, int length) {
  int jumps = 0;
  int position = 0;
  while (0 <= position && position < length) {
    int jump_size = array[position];
    if (jump_size >= 3) {
      array[position]--;
    } else {
      array[position]++;
    }
    position += jump_size;
    jumps++;
  }
  return jumps;
}

int solve_trampoline(const char *filename) {
  FILE *fp = open_file(filename);
  int length = get_length(fp);
  int *array = make_array(fp, length);
  int jumps = traverse_array(array, length);
  fclose(fp);
  free(array);
  return jumps;
}

int main(void) {
  int answer = solve_trampoline("input.txt");
  printf("answer is %d\n", answer);
  return 0;
}
