#include <stdio.h>
#include <string.h>

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

void two_balance(int a, int b, int d[6]) {
  int balancer = min(d[a], d[b]);
  d[a] -= balancer;
  d[b] -= balancer;
}

void three_balance(int a, int b, int c, int d[6]) {
  int balancer = min(d[a], d[b]);
  d[a] -= balancer;
  d[b] -= balancer;
  d[c] += balancer;
}
int simplify(int d[6]) {
  two_balance(1, 4, d);
  two_balance(2, 5, d);
  three_balance(1, 5, 0, d);
  three_balance(2, 4, 3, d);
  two_balance(2, 5, d);
  two_balance(0, 3, d);
  three_balance(0, 2, 1, d);
  three_balance(1, 3, 2, d);
  three_balance(3, 5, 4, d);
  three_balance(0, 4, 5, d);
  return d[0] + d[1] + d[2] + d[3] + d[4] + d[5];
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  int steps[6] = {0};
  int answer = 0;

  for (char line[30000]; fgets(line, 29999, fp);) {
    const char *dir = strtok(line, ",");
    while (dir != NULL) {
      if (strcmp(dir, "n") == 0) {
        steps[0]++;
      } else if (strcmp(dir, "ne") == 0) {
        steps[1]++;
      } else if (strcmp(dir, "se") == 0) {
        steps[2]++;
      } else if (strcmp(dir, "s") == 0) {
        steps[3]++;
      } else if (strcmp(dir, "sw") == 0) {
        steps[4]++;
      } else if (strcmp(dir, "nw") == 0) {
        steps[5]++;
      } else {
        printf("\n\nWEIRD TOKEN: %s\n\n", dir);
      }
      dir = strtok(NULL, ",");
      int distance = simplify(steps);
      answer = max(answer, distance);
    }
  }
  fclose(fp);
  printf("\n\nCalculated answer is %d\n\n\n", answer);
  return 0;
}
