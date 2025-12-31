#include <stdio.h>
#define BUF_SIZE 100000

void spin(int jumps, char dancers[], int positions[], int length) {
  jumps %= length;
  // willing to eat the extra steps
  for (int i = 0; i < jumps; i++) {
    char tmp = dancers[length - 1];
    for (int j = length - 1; j > 0; j--) {
      dancers[j] = dancers[j - 1];
    }
    dancers[0] = tmp;
    for (int j = 0; j < length; j++) {
      positions[j] = (positions[j] + 1) % length;
    }
  }
}

void exchange(int position_0, int position_1, char dancers[], int positions[]) {
  char dancer_0 = dancers[position_0];
  char dancer_1 = dancers[position_1];

  positions[dancer_0 - 'a'] = position_1;
  positions[dancer_1 - 'a'] = position_0;

  dancers[position_0] = dancer_1;
  dancers[position_1] = dancer_0;
}

void partner(char char_0, char char_1, char dancers[], int positions[]) {
  exchange(positions[char_0 - 'a'], positions[char_1 - 'a'], dancers,
           positions);
}

char *perform_move(char *code, char dancers[], int positions[]) {
  if (*code == '\0') {
    return code;
  }
  if (*code == '\n') {
    return code + 1;
  }
  if (*code == ',') {
    code++;
  }
  if (*code == 'p') {
    partner(*(code + 1), *(code + 3), dancers, positions);
    return code + 4;
  }

  char move = *code;
  code++;

  int input_0 = 0;
  while (*code != '/' && *code != ',') {
    input_0 = input_0 * 10 + (*code - '0');
    code++;
  }

  int input_1 = 0;
  if (*code == '/') {
    code++;
    while (*code != ',' && *code != '\n') {
      input_1 = input_1 * 10 + (*code - '0');
      code++;
    }
  }

  if (move == 's') {
    spin(input_0, dancers, positions, 16);
  } else if (move == 'x') {
    exchange(input_0, input_1, dancers, positions);
  }
  return code;
}

//
int main(void) {
  FILE *fp = fopen("input.txt", "r");
  char buffer[BUF_SIZE];
  size_t bytes_read = fread(buffer, 1, BUF_SIZE - 1, fp);
  buffer[bytes_read] = '\0';
  fclose(fp);

  char dancers[16];
  int positions[16];

  for (int i = 0; i < 16; i++) {
    dancers[i] = (char)(i + 'a');
    positions[i] = i;
  }

  int loop = 1000000000 % 60;
  char *bp;
  for (int i = 0; i < loop; i++) {
    bp = buffer;
    while (*bp != '\0') {
      bp = perform_move(bp, dancers, positions);
    }
  }

  printf("answer is ");
  for (int i = 0; i < 16; i++) {
    printf("%c", dancers[i]);
  }
  printf("\n");
  return 0;
}
