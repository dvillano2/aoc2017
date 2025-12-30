#include <stdio.h>

int absv(int a, int b) { return a > b ? a - b : b - a; }

int total_steps(int num) {
  if (num == 1) {
    return 0;
  }
  int last = 1;
  int current = 3;
  while (num > current * current) {
    last = current;
    current += 2;
  }
  num = (num - (last * last + 1)) % (current - 1);
  return (current - 1) / 2 + absv(num, (current - 3) / 2);
}

int main(void) {
  int answer = total_steps(1);
  printf("caculated is %d, correct answer is 0\n", answer);
  answer = total_steps(12);
  printf("caculated is %d, correct answer is 3\n", answer);
  answer = total_steps(23);
  printf("caculated is %d, correct answer is 2\n", answer);
  answer = total_steps(1024);
  printf("caculated is %d, correct answer is 31\n", answer);
  answer = total_steps(325489);
  printf("caculated is %d, correct answer is ?????\n", answer);
}
