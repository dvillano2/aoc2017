#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 1000
#define MAX_BLOCKS 20

FILE *open_file(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "Could not open file\n");
    exit(EXIT_FAILURE);
  }
  return fp;
}

int next_index(int index, int *blocks) {
  if (blocks[index] < 0) {
    return 0;
  }
  return index + 1;
}

void fill_blocks(FILE *fp, int *blocks) {
  int c = getc(fp);
  bool last_space = false;
  int index = 0;
  int runner = 0;
  while (c != EOF) {
    if (c == ' ' && !last_space) {
      last_space = true;
      blocks[index] = runner;
      runner = 0;
      index++;
    } else {
      last_space = false;
      runner = runner * 10 + (c - '0');
    }
    c = getc(fp);
  }
}

int get_max_location(int *blocks) {
  int index = 0;
  int max = 0
  int max_location = 0;
  while (blocks[index] >= 0) {
    if (blocks[index] > max) {
      max = blocks[index];
      max_location = index;
    }
    index++;
  }
  return max_location;
}

void redistribute(int *blocks) {
  printf("00\n");
  int max_location = get_max_location(blocks);
  printf("01\n");
  int steps = blocks[max_location];
  printf("steps are %d\n", steps);
  blocks[max_location] = 0;
  int position = max_location;
  for (int step = 0; step < steps; step++) {
    position = next_index(position, blocks);
    blocks[position]++;
  }
}

struct tree_node {
  struct tree_node *branches[200];
};

bool match_state(int *blocks, struct tree_node *head, int *node_count,
                 struct tree_node *node_array) {
  int index = 0;
  printf("0\n");
  struct tree_node *node = head;
  printf("1\n");
  struct tree_node *parent = NULL;
  printf("2\n");
  bool match = true;
  printf("4\n");
  while (blocks[index] > -1) {
    parent = node;
    printf("5\n");
    node = node->branches[blocks[index]];
    printf("6\n");
    if (node == NULL) {
      parent->branches[blocks[index]] = node_array + *node_count;
      printf("7\n");
      (*node_count)++;
      printf("8\n");
      match = false;
      printf("9\n");
      if (*node_count == MAX_NODES) {
        printf("answer too big\n");
        return true;
      }
    }
    index++;
  }
  return match;
}

int seen_state(const char *filename) {
  int blocks[20] = {-1};
  FILE *fp = open_file(filename);
  fill_blocks(fp, blocks);
  fclose(fp);
  struct tree_node head = {{NULL}};
  struct tree_node node_array[MAX_NODES] = {{{NULL}}};
  int count = 0;
  bool match = false;
  printf("HERE\n");
  while (!match) {
    redistribute(blocks);
    match = match_state(blocks, &head, &count, node_array);
  }
  return count;
}

int main(void) {
  int answer = seen_state("reallocataion1.txt");
  printf("Calculated answer is %d, correct answer is 5\n", answer);
  answer = seen_state("rellocataion2txt");
  printf("Calculated answer is %d, correct answer is ????\n", answer);
  return 0;
}
