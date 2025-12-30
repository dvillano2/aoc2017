#include <stdio.h>

int solve_spinlock(int step_size, int num_steps) {
  int position = 0, answer = 0, length = 1;
  for (int i = 1; i <= num_steps; i++) {
    position = (position + step_size) % length;
    length++;
    position++;
    if (position == 1) {
      answer = i;
    }
  }
  return answer;
}

int main(void) {
  int answer = solve_spinlock(348, 50000000);
  printf("Answer: %d\n", answer);
  return 0;
}
