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
  const int input[16] = {102, 255, 99,  252, 200, 24, 219, 57,
                         103, 2,   226, 254, 1,   0,  69,  216};
  int thread[256];
  for (int i = 0; i < 256; i++) {
    thread[i] = i;
  }
  int index = 0;
  for (int i = 0; i < 16; i++) {
    int length = input[i];
    knot(index, length, thread);
    index += length;
    index += i;
    index %= 256;
  }
  printf("\ncaluclated answer is %d\n\n", thread[0] * thread[1]);
  return 0;
}
