#include <stdint.h>
#include <stdio.h>

#define FACTOR_A 16807
#define FACTOR_B 48271
#define BIG_CONST 2147483647
#define TWO_16 65536
#define FIVE_MIL 5000000

uint_fast64_t next_numA(uint_fast64_t last) {
  uint_fast64_t next;
  do {
    next = (last * FACTOR_A) % BIG_CONST;
    last = next;
  } while ((next % 4) != 0);
  return next;
}

uint_fast64_t next_numB(uint_fast64_t last) {
  uint_fast64_t next;
  do {
    next = (last * FACTOR_B) % BIG_CONST;
    last = next;
  } while ((next % 8) != 0);
  return next;
}

int compare(uint_fast64_t a_num, uint_fast64_t b_num) {
  if (a_num % TWO_16 == b_num % TWO_16) {
    return 1;
  }
  return 0;
}

int count_matches(uint_fast64_t a_num, uint_fast64_t b_num) {
  int count = 0;
  for (unsigned int i = 0; i < FIVE_MIL; i++) {
    a_num = next_numA(a_num);
    b_num = next_numB(b_num);
    count += compare(a_num, b_num);
  }
  return count;
}

int main(void) {
  int answer = count_matches(65, 8921);
  printf("Calculated %d, correct answer is 309\n", answer);
  answer = count_matches(289, 629);
  printf("Calculated %d, correct answer is ????\n", answer);
}
