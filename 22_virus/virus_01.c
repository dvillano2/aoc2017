#include <stdio.h>
#define GRID_DIM 10000

enum DIRECTION { UP, RIGHT, DOWN, LEFT, NUM_DIRECTIONS };

struct virus {
  enum DIRECTION direction;
  char *position;
  int new_currently_infected;
};

void turn_left(struct virus *virus) {
  virus->direction = (virus->direction - 1) % NUM_DIRECTIONS;
}

void turn_right(struct virus *virus) {
  virus->direction = (virus->direction + 1) % NUM_DIRECTIONS;
}

void move_forward(struct virus *virus) {
  switch (virus->direction) {
  case (UP):
    virus->position -= GRID_DIM;
    break;
  case (DOWN):
    virus->position += GRID_DIM;
    break;
  case (LEFT):
    virus->position--;
    break;
  case (RIGHT):
    virus->position++;
    break;
  case (NUM_DIRECTIONS):
    printf("funny business\n");
    break;
  }
}

void move(struct virus *virus) {
  switch (*(virus->position)) {
  case ('#'):
    turn_right(virus);
    *(virus->position) = '.';
    break;
  case ('.'):
  case ('\0'):
  case ('\n'):
    turn_left(virus);
    *(virus->position) = '#';
    virus->new_currently_infected++;
    break;
  }
  move_forward(virus);
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  static char buf[GRID_DIM][GRID_DIM] = {{0}};
  char *start = &buf[0][0];
  for (int i = 0; i < 25; i++) {
    if (fgets(buf[GRID_DIM / 2 - 12 + i] + (GRID_DIM / 2 - 12), 100, fp) ==
        NULL) {
      break;
    };
  }
  start += GRID_DIM * GRID_DIM / 2 + GRID_DIM / 2;
  fclose(fp);

  struct virus virus = {UP, start, 0};

  for (int i = 0; i < 10000; i++) {
    move(&virus);
  }

  printf("answer is %d\n", virus.new_currently_infected);
  return 0;
}
