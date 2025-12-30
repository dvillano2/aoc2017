#include <stdio.h>

FILE *open_file(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "could not open file \n");
  }
  return fp;
}

int get_next_int(FILE *fp) {
  int c = getc(fp);
  if (c == '\n' || c == EOF) {
    return -1;
  }
  return c - 48;
}

int solve_capatcha(const char *filename) {
  int sum = 0;
  int first_digit, last_digit, current_digit;
  FILE *fp = open_file(filename);
  first_digit = get_next_int(fp);
  last_digit = first_digit;
  current_digit = get_next_int(fp);
  while (current_digit != -1) {
    if (current_digit == last_digit) {
      sum += last_digit;
    }
    last_digit = current_digit;
    current_digit = get_next_int(fp);
  }
  if (last_digit == first_digit) {
    sum += last_digit;
  }
  fclose(fp);
  return sum;
}


int main(void){
	int answer = solve_capatcha("cap_0.txt");
	printf("Calculated %d, correct answer is 3\n", answer);
	answer = solve_capatcha("cap_1.txt");
	printf("Calculated %d, correct answer is 4\n", answer);
	answer = solve_capatcha("cap_2.txt");
	printf("Calculated %d, correct answer is 0\n", answer);
	answer = solve_capatcha("cap_3.txt");
	printf("Calculated %d, correct answer is 9\n", answer);
	answer = solve_capatcha("cap_4.txt");
	printf("Calculated %d, correct answer is ?????\n", answer);
}
