#include <stdio.h>

#define BUF_SIZE 1000 * 1000

void fill_buffer(const char *filename, char buf[]) {
  FILE *fp = fopen(filename, "r");
  size_t bytes_read = fread(buf, 1, BUF_SIZE - 1, fp);
  buf[bytes_read] = '\0';
  fclose(fp);
}

struct layer {
  int range;
  int depth;
};

struct layer pull_layer(char *p) {
  struct layer layer = {0, 0};
  while (*p != ':') {
    layer.range = layer.range * 10 + (*p - '0');
    p++;
  }
  p++;
  while (*p != ' ') {
    p++;
  }
  p++;
  while (*p != '\n') {
    layer.depth = layer.depth * 10 + (*p - '0');
    p++;
  }
  return layer;
}

void store_layer(int count, struct layer layer, struct layer layers[]) {
  layers[count] = layer;
}

int score_layer(struct layer layer) {
  if (0 == (layer.range) % ((2 * layer.depth) - 2)) {
    return layer.range * layer.depth;
  }
  return 0;
}

int delay(const char *filename) {
  char buf[BUF_SIZE];
  struct layer layers[50];
  int count = 0;
  char *runner = buf;
  struct layer layer;
  fill_buffer(filename, buf);
  while (*runner != '\0') {
    layer = pull_layer(runner);
    layers[count] = layer;
    while (*runner != '\n') {
      runner++;
    }
    runner++;
    count++;
  }
  layers[count].depth = 0;
  layers[count].range = 0;
  count = 0;
  int delay = 1;
  while (1) {
    layer = layers[count];
    int depth = layer.depth;
    int range = layer.range;
    if (depth == 0) {
      return delay;
    }
    if ((delay + range) % (2 * (depth - 1)) == 0) {
      count = 0;
      delay++;
    } else {
      count++;
    }
  }
}

int main(void) {
  int answer = delay("small_input.txt");
  printf("Computed answer is %d, correct answer is 10 \n", answer);
  answer = delay("input.txt");
  printf("Computed answer is %d, correct answer is ???? \n", answer);
  return 0;
}
