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
  // printf("range: %d, depth %d \n", layer.range, layer.depth);
  return layer;
}

int score_layer(struct layer layer) {
  if (0 == (layer.range) % ((2 * layer.depth) - 2)) {
    return layer.range * layer.depth;
  }
  return 0;
}

int severity(const char *filename) {
  char buf[BUF_SIZE];
  int score = 0;
  char *runner = buf;
  fill_buffer(filename, buf);
  while (*runner != '\0') {
    struct layer layer = pull_layer(runner);
    score += score_layer(layer);
    while (*runner != '\n') {
      runner++;
    }
    runner++;
  }
  return score;
}

int main(void) {
  int answer = severity("input.txt");
  printf("answer is %d\n", answer);
  return 0;
}
