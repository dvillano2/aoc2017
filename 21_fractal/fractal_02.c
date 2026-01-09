#include <stdio.h>

int two_parse[8][4] = {{0, 1, 2, 3}, {2, 0, 3, 1}, {3, 2, 1, 0}, {1, 3, 0, 2},
                       {1, 0, 3, 2}, {2, 3, 0, 1}, {0, 2, 1, 3}, {3, 1, 2, 0}};

int three_parse[8][9] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8}, {6, 3, 0, 7, 4, 1, 8, 5, 2},
    {8, 7, 6, 5, 4, 3, 2, 1, 0}, {2, 5, 8, 1, 4, 7, 0, 3, 6},
    {2, 1, 0, 5, 4, 3, 8, 7, 6}, {6, 7, 8, 3, 4, 5, 0, 1, 2},
    {0, 3, 6, 1, 4, 7, 2, 5, 8}, {8, 5, 2, 7, 4, 1, 6, 3, 0}};

int char_to_int(char c) { return c == '#' ? 1 : 0; }

int two_four_mapper(int index) { return index % 4 < 2 ? 0 : 1; }

int nice_exponent(int i) { return i % 8 < 4 ? i % 2 : 2 + (i % 2); }

void splitfour(int four_square, int buf[2][2]) {
  for (int i = 0; i < 16; i++) {
    buf[i / 8][two_four_mapper(i)] +=
        (1 << nice_exponent(i)) * ((four_square >> i) & 1);
  }
}

void splitthree(const int three_square[2][2], int two_square[3][3]) {
  two_square[0][0] += three_square[0][0] & 1;
  two_square[0][0] += 2 * ((three_square[0][0] >> 1) & 1);
  two_square[0][0] += 4 * ((three_square[0][0] >> 3) & 1);
  two_square[0][0] += 8 * ((three_square[0][0] >> 4) & 1);

  two_square[0][1] += 1 * ((three_square[0][0] >> 2) & 1);
  two_square[0][1] += 2 * ((three_square[0][1] >> 0) & 1);
  two_square[0][1] += 4 * ((three_square[0][0] >> 5) & 1);
  two_square[0][1] += 8 * ((three_square[0][1] >> 3) & 1);

  two_square[0][2] += 1 * ((three_square[0][1] >> 1) & 1);
  two_square[0][2] += 2 * ((three_square[0][1] >> 2) & 1);
  two_square[0][2] += 4 * ((three_square[0][1] >> 4) & 1);
  two_square[0][2] += 8 * ((three_square[0][1] >> 5) & 1);

  two_square[1][0] += 1 * ((three_square[0][0] >> 6) & 1);
  two_square[1][0] += 2 * ((three_square[0][0] >> 7) & 1);
  two_square[1][0] += 4 * ((three_square[1][0] >> 0) & 1);
  two_square[1][0] += 8 * ((three_square[1][0] >> 1) & 1);

  two_square[1][1] += 1 * ((three_square[0][0] >> 8) & 1);
  two_square[1][1] += 2 * ((three_square[0][1] >> 6) & 1);
  two_square[1][1] += 4 * ((three_square[1][0] >> 2) & 1);
  two_square[1][1] += 8 * ((three_square[1][1] >> 0) & 1);

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

int count_bits(int *grid_start, int block_size, int blocks) {
  int count = 0;
  for (int i = 0; i < blocks; i++) {
    for (int j = 0; j < block_size; j++) {
      for (int k = 0; k < blocks; k++) {
        for (int l = 0; l < block_size; l++) {
          int num = *(grid_start + i * 1000 + k);
          int shift = j * block_size + l;
          count += (num >> shift) & 1;
        }
      }
    }
  }
  return count;
}

void iterate(int *grid, int *block_size, int *blocks, int *buf,
             const int two_map[], const int three_map[]) {
  if (*block_size == 4) {
    for (int i = 0; i < *blocks; i++) {
      for (int j = 0; j < *blocks; j++) {
        int small_buf[2][2] = {{0}};
        splitfour(*(grid + 1000 * i + j), small_buf);
        *(buf + 2 * i * 1000 + 2 * j) = two_map[small_buf[0][0]];
        *(buf + 2 * i * 1000 + 2 * j + 1) = two_map[small_buf[0][1]];
        *(buf + (2 * i + 1) * 1000 + 2 * j) = two_map[small_buf[1][0]];
        *(buf + (2 * i + 1) * 1000 + 2 * j + 1) = two_map[small_buf[1][1]];
      }
    }
    *blocks *= 2;
    *block_size = 3;
  } else if (*blocks % 2 == 1) {
    for (int i = 0; i < *blocks; i++) {
      for (int j = 0; j < *blocks; j++) {
        *(buf + 1000 * i + j) = three_map[*(grid + 1000 * i + j)];
      }
    }
    *block_size = 4;
  } else {
    for (int i = 0; i < *blocks; i += 2) {
      for (int j = 0; j < *blocks; j += 2) {
        int small_buf[3][3] = {{0}};
        int three_buf[2][2] = {{0}};

        three_buf[0][0] = *(grid + 1000 * i + j);
        three_buf[0][1] = *(grid + 1000 * i + j + 1);
        three_buf[1][0] = *(grid + 1000 * (i + 1) + j);
        three_buf[1][1] = *(grid + 1000 * (i + 1) + (j + 1));
        splitthree(three_buf, small_buf);

        int bound_i = i / 2 * 3;
        int bound_j = j / 2 * 3;
        for (int k = 0; k < 3; k++) {
          for (int l = 0; l < 3; l++) {
            *(buf + (bound_i + k) * 1000 + bound_j + l) =
                two_map[small_buf[k][l]];
          }
        }
      }
    }
    *blocks /= 2;
    *blocks *= 3;
  }
}

int main(void) {
  int two_map[16] = {0};
  int three_map[512] = {0};
  FILE *fp = fopen("input.txt", "r");
  for (char line[1000]; fgets(line, sizeof(line), fp);) {
    parse_line(line, two_map, three_map);
  }
  fclose(fp);

  static int bufs[2][1000][1000] = {{{0}}};
  bufs[0][0][0] = 482;
  int blocks = 1;
  int block_size = 3;
  int *grid = &bufs[0][0][0];
  int index = 1;
  int *buf = &bufs[index][0][0];

  for (int i = 0; i < 18; i++) {
    iterate(grid, &block_size, &blocks, buf, two_map, three_map);
    grid = buf;
    index++;
    buf = &bufs[index % 2][0][0];
  }

  int final_count = count_bits(grid, block_size, blocks);
  printf("answer is %d\n", final_count);
  return 0;
}
