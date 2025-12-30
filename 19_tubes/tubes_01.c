#include <stdio.h>

#define BUF_LEN 1000 * 1000

enum Direction { UP, RIGHT, DOWN, LEFT, NUM_DIRECTIONS };

int move_len(const enum Direction *d, int length) {
  switch (*d) {
  case (UP):
    return -length;
    break;
  case (RIGHT):
    return 1;
    break;
  case (DOWN):
    return length;
    break;
  case (LEFT):
    return -1;
    break;
  default:
    printf("Bad enum\n");
    return 0;
    break;
  }
}

void straight(char **buf, const enum Direction *d, int length) {
  *buf += move_len(d, length);
}

void turn(char **buf, enum Direction *d, int length) {
  enum Direction first_turn = (*d + 1) % NUM_DIRECTIONS;
  char next = *(*buf + move_len(&first_turn, length));
  if (next != ' ') {
    *d = first_turn;
  } else {
    *d = (first_turn + 2) % NUM_DIRECTIONS;
  }
  straight(buf, d, length);
}

void collect(char **buf, char **letters) {
  if (**buf >= 'A' && **buf <= 'Z') {
    **letters = **buf;
    (*letters)++;
  }
}

int get_len(const char *buf) {
  int len = 1;
  while (*buf != '\n') {
    buf++;
    len++;
  }
  return len;
}

void find_start(char **buf) {
  while (**buf == ' ' || **buf == '\n') {
    (*buf)++;
  }
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  char buffer[BUF_LEN] = {0};
  size_t bytes_read = fread(buffer, 1, BUF_LEN - 1, fp);
  buffer[bytes_read] = '\0';
  fclose(fp);

  int length = get_len(buffer);
  char letters[20] = {0};
  enum Direction d = DOWN;

  char *b = buffer;
  char *l = letters;

  find_start(&b);
  while (*b != ' ') {
    collect(&b, &l);
    if (*b == '+') {
      turn(&b, &d, length);
    } else {
      straight(&b, &d, length);
    }
  }

  printf("\n");
  printf("\nCollected letters are %s\n\n", letters);
  return 0;
}
