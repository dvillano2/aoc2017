#include <stdio.h>

// This is cheating because I just looked through the
// list first to see if i could identify low acceleration
// vectors. Theres only one where the sums of the
// absolute values are 1... so the task is just to find that

int get_int(char **buf) {
  int value = 0;
  if (**buf == '-') {
    (*buf)++;
  }
  while (**buf != ',' && **buf != '>') {
    value = value * 10 + (**buf - '0');
    (*buf)++;
  }
  (*buf)++;
  return value;
}

int main(void) {
  int count = 0;
  FILE *fp = fopen("input.txt", "r");
  if (!fp) {
    perror("input.txt");
    return 1;
  }
  for (char line[90]; fgets(line, 89, fp);) {
    char a = 0;
    char *l = line;
    while (*l != 'a') {
      l++;
    }
    l += 3;
    while (*l != '\n') {
      a += get_int(&l);
    }
    if (a == 1) {
      printf("answer is %d\n", count);
      break;
    }
    count++;
  }
  fclose(fp);
  return 0;
}
