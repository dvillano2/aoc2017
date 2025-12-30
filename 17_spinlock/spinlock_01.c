#include <stdio.h>
#include <stdlib.h>

struct node {
  struct node *next_node;
  int value;
};

struct node *make_node(struct node *next_node, int value) {
  struct node *tmp = malloc(sizeof(struct node));
  if (tmp == NULL) {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }
  tmp->next_node = next_node;
  tmp->value = value;
  return tmp;
}

int solve_spinlock(int step_size, int num_steps) {
  struct node base_node;
  base_node.next_node = &base_node;
  base_node.value = 0;
  int i, j, answer;
  struct node *running_node = &base_node;
  for (i = 1; i < num_steps + 1; i++) {
    for (j = 0; j < step_size; j++) {
      running_node = running_node->next_node;
    }
    struct node *new_node = make_node(running_node->next_node, i);
    running_node->next_node = new_node;
    running_node = new_node;
  }
  answer = running_node->next_node->value;
  while (base_node.next_node != &base_node) {
    if (running_node->next_node == &base_node) {
      running_node = &base_node;
    }
    struct node *tmp = running_node->next_node->next_node;
    free(running_node->next_node);
    running_node->next_node = tmp;
  }
  return answer;
}

int main(void) {
  int answer = solve_spinlock(348, 2017);
  printf("Answer: %d\n", answer);
  return 0;
}
