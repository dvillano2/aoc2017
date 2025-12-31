#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Direction {
  RIGHT,
  UP,
  LEFT,
  DOWN,
  NUM_DIRECTIONS,
};

struct spiral_node {
  int value;
  enum Direction next_direction;
  struct spiral_node *neighbors[NUM_DIRECTIONS];
};

void connect_nodes(enum Direction direction, struct spiral_node *node_0,
                   struct spiral_node *node_1) {
  node_0->neighbors[direction] = node_1;
  node_1->neighbors[(direction + 2) % NUM_DIRECTIONS] = node_0;
}

struct spiral_node *add_node(struct spiral_node *current_node) {
  // make the new node
  struct spiral_node *new_node = malloc(sizeof(struct spiral_node));
  memset(new_node, 0, sizeof(struct spiral_node));
  // connect to the last node and add value
  connect_nodes(current_node->next_direction, current_node, new_node);
  new_node->value = current_node->value;
  // check if other nodes exist, if so, add value and connect if apppropriate
  enum Direction present_direction = current_node->next_direction;
  enum Direction next_direction = ((present_direction) + 1) % NUM_DIRECTIONS;
  struct spiral_node *inner_node = current_node->neighbors[next_direction];

  new_node->next_direction = next_direction;
  if (inner_node != NULL) {
    new_node->value += inner_node->value;
    inner_node = inner_node->neighbors[present_direction];
    if (inner_node != NULL) {
      new_node->value += inner_node->value;
      connect_nodes(next_direction, new_node, inner_node);
      inner_node = inner_node->neighbors[present_direction];
      new_node->next_direction = present_direction;
      if (inner_node != NULL) {
        new_node->value += inner_node->value;
      }
    }
  }
  return new_node;
}

int build_spiral(int threshold) {
  struct spiral_node base_node = {1, RIGHT, {NULL, NULL, NULL, NULL}};
  struct spiral_node *node = &base_node;
  int answer;
  while (node->value <= threshold) {
    node = add_node(node);
  }
  answer = node->value;

  struct spiral_node *to_be_freed = base_node.neighbors[RIGHT];
  while (to_be_freed != NULL) {
    struct spiral_node *tmp =
        to_be_freed->neighbors[to_be_freed->next_direction];
    free(to_be_freed);
    to_be_freed = tmp;
  }
  return answer;
}

int main(void) {
  int answer = build_spiral(325489);
  printf("answer is %d\n", answer);
}
