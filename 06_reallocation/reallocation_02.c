#include <stdio.h>
#include <stdlib.h>

#define LEN 16
#define SMALL_LEN 4

int get_max_index(const int *blocks, int len) {
  int max_seen = -1;
  int answer = -1;
  for (int i = 0; i < len; i++) {
    if (blocks[i] > max_seen) {
      max_seen = blocks[i];
      answer = i;
    }
  }
  return answer;
}

int next_index(int index, int len) { return (index + 1) % len; }

void redistribute(int *blocks, int len) {
  int index = get_max_index(blocks, len);
  int steps = blocks[index];
  blocks[index] = 0;
  for (int i = 0; i < steps; i++) {
    index = next_index(index, len);
    blocks[index]++;
  }
}

struct node {
  struct node *children[18];
  int count;
};

int add_node(struct node *head, int *count, const int blocks[]) {
  struct node *node = head;
  int new_flag = 0;
  (*count)++;
  int cycler = -1;
  for (int i = 0; i < LEN; i++) {
    if (node->children[blocks[i]] == NULL) {
      node->children[blocks[i]] = calloc(1, sizeof(struct node));
      node->count = *count;
      new_flag = 1;
    } else {
      cycler = node->count;
    }
    node = node->children[blocks[i]];
  }
  return new_flag == 1 ? -1 : cycler;
}

void free_tree(struct node *head) {
  if (head == NULL) {
    return;
  }
  for (int i = 0; i < 18; i++) {
    free_tree(head->children[i]);
  }
  free(head);
  return;
}

int main(void) {
  int blocks[LEN] = {14, 0, 15, 12, 11, 11, 3, 5, 1, 6, 8, 4, 9, 1, 8, 4};
  int count = -1;
  int repeat = -1;
  struct node *head = calloc(1, sizeof(struct node));
  while (repeat == -1) {
    repeat = add_node(head, &count, blocks);
    redistribute(blocks, LEN);
  }
  free_tree(head);
  printf("\n\nanswer is %d\n\n", count - repeat);
  return 0;
}
