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

int main(void) {
  const char *s_input = "102,255,99,252,200,24,219,57,103,2,226,254,1,0,69,216";
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

  printf("answer is ");

  for (int i = 0; i < 16; i++) {
    printf("%02x", dense[i]);
  }
  printf("\n");
  return 0;
}
