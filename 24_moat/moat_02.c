#include <stdio.h>

#define BUF_SIZE 1000 * 1000

void fill_buffer(const char *filename, char buf[]) {
  FILE *fp = fopen(filename, "r");
  size_t bytes_read = fread(buf, 1, BUF_SIZE - 1, fp);
  buf[bytes_read] = '\0';
  fclose(fp);
}

struct port {
  int left;
  int right;
  int used;
};

char **buf_to_port(char **p, struct port *port) {
  int left = 0;
  int right = 0;
  while (**p != '/') {
    left = left * 10 + (**p - '0');
    (*p)++;
  }
  (*p)++;
  while (**p != '\n') {
    right = right * 10 + (**p - '0');
    (*p)++;
  }
  (*p)++;
  port->left = left;
  port->right = right;
  port->used = 0;
  return p;
}

int match_port(int value, struct port port) {
  if (port.left == value) {
    return port.right;
  }
  if (port.right == value) {
    return port.left;
  }
  return -1;
}

int max(int a, int b) { return a > b ? a : b; }

struct bridge {
  int length;
  int strength;
};

struct bridge max_match(int value, struct port ports[]) {
  int count = 0;
  struct bridge max_bridge = {0, 0};
  do {
    struct port port = ports[count];
    int match = match_port(value, port);

    if (match == -1 || port.used == 1) {
      count++;
      continue;
    }

    ports[count].used = 1;
    struct bridge sub_bridge = max_match(match, ports);

    int sub_length = 1 + sub_bridge.length;
    if (sub_length >= max_bridge.length) {
      int sub_total = sub_bridge.strength + port.left + port.right;
      if (sub_length == max_bridge.length) {
        max_bridge.strength = max(sub_total, max_bridge.strength);
      } else {
        max_bridge.length = sub_length;
        max_bridge.strength = sub_total;
      }
    }

    ports[count].used = 0;
    count++;
  } while (ports[count].left > -1);
  return max_bridge;
}

int main(void) {
  char buf[BUF_SIZE];
  fill_buffer("input.txt", buf);
  char *b = buf;
  char **bp = &b;
  struct port ports[70];
  struct port *p = ports;
  while (**bp != '\0') {
    bp = buf_to_port(bp, p);
    p++;
  }
  p->left = -1;
  p->right = -1;
  p->used = -1;

  struct bridge max_len_bridge = max_match(0, ports);
  printf("answer is %d\n", max_len_bridge.strength);
  return 0;
}
