#include <stdio.h>

#define GRID_SIZE 128 * 128

int find(int point, int parents[GRID_SIZE]) {
  if (parents[point] != point) {
    parents[point] = find(parents[point], parents);
    return parents[point];
  }
  return point;
}

void merge(int point_0, int point_1, int parents[GRID_SIZE]) {
  point_0 = find(point_0, parents);
  point_1 = find(point_1, parents);
  parents[point_1] = point_0;
}

void swap(int index_0, int index_1, int thread[256]) {
  int tmp = thread[index_0];
  thread[index_0] = thread[index_1];
  thread[index_1] = tmp;
}

void knot(int index, int length, int thread[256]) {
  int end = (index + length - 1) % 256;
  int mid = length / 2;
  for (int i = 0; i < mid; i++) {
    swap(index, end, thread);
    index = (index + 1) % 256;
    end = end == 0 ? 255 : (end - 1);
  }
}

void hash(char *s_input, int dense[16]) {
  int input[200] = {0};
  int input_length = 0;
  for (; *s_input != '\0'; s_input++, input_length++) {
    input[input_length] = (int)*s_input;
  }
  input[input_length] = 17;
  input[input_length + 1] = 31;
  input[input_length + 2] = 73;
  input[input_length + 3] = 47;
  input[input_length + 4] = 23;
  input_length = input_length + 5;

  int thread[256];
  for (int i = 0; i < 256; i++) {
    thread[i] = i;
  }

  int index = 0;
  int skip_size = 0;
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < input_length; j++) {
      int length = input[j];
      knot(index, length, thread);
      index += length;
      index += skip_size;
      index %= 256;
      skip_size++;
    }
  }

  for (int i = 0; i < 16; i++) {
    dense[i] = 0;
    for (int j = 0; j < 16; j++) {
      dense[i] ^= thread[16 * i + j];
    }
  }
}

void make_input(int input, char base[20]) {
  const char *prefix = "amgozmfv-";
  sprintf(base, "%s", prefix);
  sprintf(base + 9, "%d", input);
}

int main(void) {
  int parents[GRID_SIZE];
  for (int i = 0; i < GRID_SIZE; i++) {
    parents[i] = -1;
  }

  for (int i = 0; i < 128; i++) {
    char base[20] = {0};
    make_input(i, base);
    int dense[16] = {0};
    hash(base, dense);

    for (int j = 0; j < 16; j++) {
      for (int k = 0; k < 8; k++) {
        int present_bit = (dense[j] >> (7 - k)) & 1;
        int spot = 128 * i + 8 * j + k;

        if (present_bit == 1) {
          parents[spot] = spot;
          if (j + k > 0 && parents[spot - 1] > -1) {
            merge(spot, spot - 1, parents);
          }
          if (i > 0 && parents[spot - 128] > -1) {
            merge(spot, spot - 128, parents);
          }
        }
      }
    }
  }
  int region_count = 0;
  for (int i = 0; i < 128; i++) {
    for (int j = 0; j < 128; j++) {
      int spot = 128 * i + j;
      if (parents[spot] == spot) {
        region_count++;
      }
    }
  }
  printf("answer is %d\n", region_count);
  return 0;
}
