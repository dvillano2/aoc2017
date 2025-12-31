#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1000 * 1000

struct node {
  struct node *children[27];
  int weight;
  int total_weight;
  int end;
  struct node *tree_children[20];
  int num_children;
  struct node *tree_parent;
};

int get_index(const char *s) { return (*s != '\0') ? (*s - 'a') : 26; }

void add_node(char *s, int weight, struct node *head) {
  struct node *node = head;
  while (1) {
    int index = get_index(s);
    if (node->children[index] == NULL) {
      node->children[index] = calloc(1, sizeof(struct node));
    }
    node = node->children[index];
    node->weight = weight;
    node->total_weight = weight;
    if (*s == '\0') {
      node->end = 1;
      return;
    }
    s++;
  }
}

struct node *get_node(char *s, struct node *node) {
  while (node->end == 0) {
    int index = get_index(s);
    node = node->children[index];
    s++;
  }
  return node;
}

void free_tree(struct node *head) {
  if (head == NULL) {
    return;
  }
  for (int i = 0; i < 27; i++) {
    free_tree(head->children[i]);
  }
  free(head);
}

void weight_process(char *line, struct node *head) {
  char *name = strtok(line, " ");
  char *s = strtok(NULL, " ");
  s++;
  int weight = 0;
  for (; *s != ')'; s++) {
    weight = weight * 10 + (*s - '0');
  }
  add_node(name, weight, head);
}

void assign_children(char *line, struct node *head) {
  char *name = strtok(line, " ,\n");
  struct node *parent = get_node(name, head);
  char *s = strtok(NULL, " ,\n");
  while (s != NULL) {
    if (*s >= 'a' && *s <= 'z') {
      get_node(s, head)->tree_parent = parent;
      parent->tree_children[parent->num_children] = get_node(s, head);
      parent->num_children++;
    }
    s = strtok(NULL, " ,\n");
  }
}

void update_weights(char *line, struct node *head) {
  char *name = strtok(line, " ,\n");
  int base_weight = get_node(name, head)->weight;
  int extra_weight = 0;
  char *s = strtok(NULL, " ,\n");
  while (s != NULL) {
    if (*s >= 'a' && *s <= 'z') {
      extra_weight += get_node(s, head)->total_weight;
    }
    s = strtok(NULL, " ,\n");
  }
  get_node(name, head)->total_weight = base_weight + extra_weight;
}

struct node *identify(struct node *base) {
  if (base->num_children == 0) {
    printf("reached a dead end\n");
    return NULL;
  }
  if (base->num_children == 1) {
    return base->tree_children[0];
  }
  if (base->num_children == 2) {
    return NULL;
  }
  struct node *first_contender = base->tree_children[0];
  int first_weight = first_contender->total_weight;

  struct node *second_contender = base->tree_children[1];
  int second_weight = second_contender->total_weight;

  struct node *third_contender = base->tree_children[2];
  int third_weight = third_contender->total_weight;

  int majority;
  if (first_weight == second_weight && first_weight == third_weight) {
    majority = first_weight;
  } else if (first_weight == second_weight) {
    return third_contender;
  } else if (first_weight == third_weight) {
    return second_contender;
  } else {
    return first_contender;
  }

  for (int i = 3; i < base->num_children; i++) {
    struct node *child = base->tree_children[i];
    if (child->total_weight != majority) {
      return child;
    }
  }
  return NULL;
}

int diff(struct node *base) {
  const struct node *node = identify(base);
  int odd_weight = node->total_weight;
  for (int i = 0; i < base->num_children; i++) {
    if (base->tree_children[i] != node) {
      return base->tree_children[i]->total_weight - odd_weight;
    }
  }
  printf("problem\n");
  return -1;
}

int main(void) {
  struct node *head = calloc(1, sizeof(struct node));
  FILE *fp = fopen("input.txt", "r");
  for (char line[100]; fgets(line, 99, fp);) {
    weight_process(line, head);
  }
  rewind(fp);
  for (char line[100]; fgets(line, 99, fp);) {
    assign_children(line, head);
  }
  for (int i = 0; i < 2000; i++) {
    rewind(fp);
    for (char line[100]; fgets(line, 99, fp);) {
      update_weights(line, head);
    }
  }
  fclose(fp);
  struct node *walker = get_node("gmcrj", head);
  int to_add = diff(walker);
  while (1) {
    struct node *tmp = identify(walker);
    if (tmp == NULL) {
      break;
    }
    walker = tmp;
  }
  printf("answer is %d\n", walker->weight + to_add);

  free_tree(head);
  return 0;
}
