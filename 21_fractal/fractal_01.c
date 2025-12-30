#include <stdio.h>

int two_parse[8][4] = {{0, 1, 2, 3}, {2, 0, 3, 1}, {3, 2, 1, 0}, {1, 3, 0, 2},
                       {1, 0, 3, 2}, {2, 3, 0, 1}, {0, 2, 1, 3}, {3, 1, 2, 0}};

int three_parse[8][9] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8}, {6, 3, 0, 7, 4, 1, 8, 5, 2},
    {8, 7, 6, 5, 4, 3, 2, 1, 0}, {2, 5, 8, 1, 4, 7, 0, 3, 6},
    {2, 1, 0, 5, 4, 3, 8, 7, 6}, {6, 7, 8, 3, 4, 5, 0, 1, 2},
    {0, 3, 6, 1, 4, 7, 2, 5, 8}, {8, 5, 2, 7, 4, 1, 6, 3, 0}};

int char_to_int(char c) { return c == '#' ? 1 : 0; }

void parse_line(char *line, int two_map[16], int three_map[512]) {
  int dihedral[8] = {0};
  char c = *(line + 2);
  int *parse = c == '/' ? &two_parse[0][0] : &three_parse[0][0];
  const int length = c == '/' ? 4 : 9;
  int *map = c == '/' ? two_map : three_map;

  int count = 0;
  while (*line != '>') {
    if (*line == '#' || *line == '.') {
      int val = char_to_int(*line);
      for (int i = 0; i < 8; i++) {
        dihedral[i] += val * (1 << *(parse + i * length + count));
      }
      count++;
    }
    line++;
  }

  line += 2;
  int output = 0;
  count = 0;
  while (*line != '\n') {
    if (*line == '#' || *line == '.') {
      int val = char_to_int(*line);
      output = output + val * (1 << count);
      count++;
    }
    line++;
  }

  for (int i = 0; i < 8; i++) {
    *(map + dihedral[i]) = output;
  }
}

int two_four_mapper(int index) { return index % 4 < 2 ? 0 : 1; }

int nice_exponent(int i) { return i % 8 < 4 ? i % 2 : 2 + (i % 2); }

void splitfour(int four_square, int buf[2][2]) {
  for (int i = 0; i < 16; i++) {
    buf[i / 8][two_four_mapper(i)] +=
        (1 << nice_exponent(i)) * ((four_square >> i) & 1);
  }
}

void splitthree(int three_square[2][2], int two_square[3][3]) {
  two_square[0][0] += three_square[0][0] & 1;
  two_square[0][0] += 2 * ((three_square[0][0] >> 1) & 1);
  two_square[0][0] += 4 * ((three_square[0][0] >> 3) & 1);
  two_square[0][0] += 8 * ((three_square[0][0] >> 4) & 1);

  two_square[0][1] += 1 * ((three_square[0][0] >> 2) & 1);
  two_square[0][1] += 2 * ((three_square[0][1]) & 1);
  two_square[0][1] += 4 * ((three_square[0][0] >> 5) & 1);
  two_square[0][1] += 8 * ((three_square[0][1] >> 3) & 1);

  two_square[0][2] += 1 * ((three_square[0][1] >> 1) & 1);
  two_square[0][2] += 2 * ((three_square[0][1] >> 2) & 1);
  two_square[0][2] += 4 * ((three_square[0][1] >> 4) & 1);
  two_square[0][2] += 8 * ((three_square[0][1] >> 5) & 1);

  two_square[1][0] += 1 * ((three_square[0][0] >> 6) & 1);
  two_square[1][0] += 2 * ((three_square[0][0] >> 7) & 1);
  two_square[1][0] += 4 * (three_square[1][0] & 1);
  two_square[1][0] += 8 * ((three_square[1][0] >> 1) & 1);

  two_square[1][1] += 1 * ((three_square[0][0] >> 8) & 1);
  two_square[1][1] += 2 * ((three_square[0][1] >> 6) & 1);
  two_square[1][1] += 4 * ((three_square[1][0] >> 2) & 1);
  two_square[1][1] += 8 * (three_square[1][1] & 1);

  two_square[1][2] += 1 * ((three_square[0][1] >> 7) & 1);
  two_square[1][2] += 2 * ((three_square[0][1] >> 8) & 1);
  two_square[1][2] += 4 * ((three_square[1][1] >> 1) & 1);
  two_square[1][2] += 8 * ((three_square[1][1] >> 2) & 1);

  two_square[2][0] += 1 * ((three_square[1][0] >> 3) & 1);
  two_square[2][0] += 2 * ((three_square[1][0] >> 4) & 1);
  two_square[2][0] += 4 * ((three_square[1][0] >> 6) & 1);
  two_square[2][0] += 8 * ((three_square[1][0] >> 7) & 1);

  two_square[2][1] += 1 * ((three_square[1][0] >> 5) & 1);
  two_square[2][1] += 2 * ((three_square[1][1] >> 3) & 1);
  two_square[2][1] += 4 * ((three_square[1][0] >> 8) & 1);
  two_square[2][1] += 8 * ((three_square[1][1] >> 6) & 1);

  two_square[2][2] += 1 * ((three_square[1][1] >> 4) & 1);
  two_square[2][2] += 2 * ((three_square[1][1] >> 5) & 1);
  two_square[2][2] += 4 * ((three_square[1][1] >> 7) & 1);
  two_square[2][2] += 8 * ((three_square[1][1] >> 8) & 1);
}

void print_grid(int *grid_start, int block_size, int blocks) {
  printf("\n");
  for (int i = 0; i < blocks; i++) {
    for (int j = 0; j < block_size; j++) {
      for (int k = 0; k < blocks; k++) {
        for (int l = 0; l < block_size; l++) {
          int num = *(grid_start + i * blocks + k);
          int shift = j * block_size + l;
          int flag = (num >> shift) & 1;
          char bit = flag == 1 ? '#' : '.';
          printf("%c", bit);
        }
      }
      printf("\n");
    }
  }
}
int count_bits(int *grid_start, int block_size, int blocks) {
  int count = 0;
  for (int i = 0; i < blocks; i++) {
    for (int j = 0; j < block_size; j++) {
      for (int k = 0; k < blocks; k++) {
        for (int l = 0; l < block_size; l++) {
          int num = *(grid_start + i * blocks + k);
          int shift = j * block_size + l;
          count += (num >> shift) & 1;
        }
      }
    }
  }
  return count;
}

int main(void) {
  int two_map[16] = {0};
  int three_map[512] = {0};
  FILE *fp = fopen("input.txt", "r");
  for (char line[100]; fgets(line, sizeof(line), fp);) {
    parse_line(line, two_map, three_map);
  }
  fclose(fp);

  int start = 482;

  int one_pass = three_map[start];
  int buf_1[2][2] = {{0}};
  splitfour(one_pass, buf_1);

  int next[2][2];
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      next[i][j] = two_map[buf_1[i][j]];
    }
  }

  int buf_2[3][3] = {{0}};
  splitthree(next, buf_2);

  int again[3][3] = {{0}};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      again[i][j] = two_map[buf_2[i][j]];
    }
  }

  int continued[3][3];
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      continued[i][j] = three_map[again[i][j]];
    }
  }

  int split[6][6];
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      int buf_3[2][2] = {{0}};
      splitfour(continued[i][j], buf_3);
      split[2 * i][2 * j] = buf_3[0][0];
      split[2 * i][2 * j + 1] = buf_3[0][1];
      split[2 * i + 1][2 * j] = buf_3[1][0];
      split[2 * i + 1][2 * j + 1] = buf_3[1][1];
    }
  }

  int final[6][6];
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      final[i][j] = two_map[split[i][j]];
    }
  }
  int final_count = count_bits(&final[0][0], 3, 6);
  printf("answer is %d\n", final_count);

  return 0;
}
