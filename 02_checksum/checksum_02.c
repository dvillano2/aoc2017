#include <stdio.h>
#include <stdlib.h>

FILE *open_file(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "could not open file\n");
  }
  return fp;
}

int get_next_digit(FILE *fp) {
  int c = getc(fp);
  if (c == ' ') {
    return -1;
  }
  if (c == '\n') {
    return -2;
  }
  if (c == EOF) {
    return -3;
  }
  return c - '0';
}

int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

int min(int a, int b) {
  if (a < b) {
    return a;
  }
  return b;
}

int remain(int a, int b) {
  if (a < b) {
    return b % a;
  }
  return a % b;
}

struct int_node {
  int value;
  struct int_node *next_int;
};

int compare(struct int_node *base_node, int num, int num_count) {
  if (num == 0) {
    return 0;
  }
  struct int_node *node = base_node;
  int comparison = 0;
  // do the comparisons
  for (int i = 0; i < num_count; i++) {
    int value = node->value;
    if (value > 0 && remain(num, value) == 0) {
      comparison = max(num, value) / min(num, value);
    }
    if (node->next_int != NULL) {
      node = node->next_int;
    }
  }
  // make the new node if necessary, put this rows numbs in the right place
  if (node->next_int == NULL) {
    struct int_node *new_node = malloc(sizeof(struct int_node));
    new_node->next_int = NULL;
    node->next_int = new_node;
    node = new_node;
  }
  node->value = num;

  return comparison;
}

int get_checksum(const char *filename) {
  FILE *fp = open_file(filename);
  int checksum = 0;
  int number = 0;
  int num_count = 1;
  int c = get_next_digit(fp);
  struct int_node base_node = {-1, NULL};
  while (c != -3) {
    if (c >= 0) {
      number = number * 10 + c;
    } else if (number != 0) {
      checksum += compare(&base_node, number, num_count);
      number = 0;
      num_count++;
    }
    if (c == -2) {
      number = 0;
      num_count = 1;
    }
    c = get_next_digit(fp);
  }
  while (base_node.next_int != NULL) {
    struct int_node *tmp = base_node.next_int->next_int;
    free(base_node.next_int);
    base_node.next_int = tmp;
  }
  fclose(fp);
  return checksum;
}

int main(void) {
  int answer = get_checksum("checksum3.txt");
  printf("calculated %d, the answer is 9\n", answer);
  answer = get_checksum("checksum2.txt");
  printf("calculated %d, the answer is ????\n", answer);
  return 0;
}
