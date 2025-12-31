#include <stdio.h>
#include <stdlib.h>

int move(char *state, int *position, int *tape) {
  int value = *(tape + *position);
  if (*state == 'A') {
    if (value == 0) {
      *(tape + *position) = 1;
      (*position)++;
      *state = 'B';
      return 1;
    } else if (value == 1) {
      *(tape + *position) = 0;
      (*position)--;
      *state = 'C';
      return -1;
    } else {
      printf("out of range value at position %d\n", *position);
      return 0;
    }
  } else if (*state == 'B') {
    if (value == 0) {
      *(tape + *position) = 1;
      (*position)--;
      *state = 'A';
      return 1;
    } else if (value == 1) {
      (*position)++;
      *state = 'D';
      return 0;
    } else {
      printf("out of range value at position %d\n", *position);
      return 0;
    }

  } else if (*state == 'C') {
    if (value == 0) {
      (*position)--;
      *state = 'B';
      return 0;
    } else if (value == 1) {
      *(tape + *position) = 0;
      (*position)--;
      *state = 'E';
      return -1;
    } else {
      printf("out of range value at position %d\n", *position);
      return 0;
    }

  } else if (*state == 'D') {
    if (value == 0) {
      *(tape + *position) = 1;
      (*position)++;
      *state = 'A';
      return 1;
    } else if (value == 1) {
      *(tape + *position) = 0;
      (*position)++;
      *state = 'B';
      return -1;
    } else {
      printf("out of range value at position %d\n", *position);
      return 0;
    }

  } else if (*state == 'E') {
    if (value == 0) {
      *(tape + *position) = 1;
      (*position)--;
      *state = 'F';
      return 1;
    } else if (value == 1) {
      (*position)--;
      *state = 'C';
      return 0;
    } else {
      printf("out of range value at position %d\n", *position);
      return 0;
    }

  } else if (*state == 'F') {
    if (value == 0) {
      *(tape + *position) = 1;
      (*position)++;
      *state = 'D';
      return 1;
    } else if (value == 1) {
      (*position)++;
      *state = 'A';
      return 0;
    } else {
      printf("out of range value at position %d\n", *position);
      return 0;
    }
  } else {
    printf("state out of range");
    return 0;
  }
}

int main(void) {
  char state = 'A';
  int *tape = calloc(2 * 12481997, sizeof(int));
  int position = 12481997;
  int checksum = 0;
  for (int i = 0; i < 12481997; i++) {
    checksum += move(&state, &position, tape);
  }
  printf("answer is %d\n", checksum);
  free(tape);
}
