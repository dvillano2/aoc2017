#include <stdio.h>

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

int hash_zeros(char *s_input) {
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

  int dense[16] = {0};
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      dense[i] ^= thread[16 * i + j];
    }
  }

  int ones = 0;
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 8; j++) {
      ones += dense[i] % 2;
      dense[i] /= 2;
    }
  }
  return ones;
}

void make_input(int input, char base[20]) {
  const char *prefix = "amgozmfv-";
  sprintf(base, "%s", prefix);
  sprintf(base + 9, "%d", input);
}

int main(void) {
  int total_ones = 0;
  for (int i = 0; i < 128; i++) {
    char base[20] = {0};
    make_input(i, base);
    total_ones += hash_zeros(base);
  }
  printf("\ncaluclated answer is %d", total_ones);
  printf("\n\n");
  return 0;
}
