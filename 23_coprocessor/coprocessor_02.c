#include <stdio.h>

int main(void) {
  int b = 81 * 100 + 100000;
  int c = b + 17000;
  int count = 0;
  for (int i = b; i <= c; i += 17) {
    for (int j = 2; j < 360; j++) {
      if (i % j == 0) {
        count++;
        break;
      }
    }
  }
  printf("answer is %d\n", count);
  return 0;
}
