#include <stdio.h>

enum Instructions { SET, SUB, MUL, JNZ, NUM_INSTRUCTIONS };

int make_int(char *s) {
  int num = 0;
  int neg_flag = 1;
  if (*s == '-') {
    neg_flag = -1;
    s++;
  }
  while (*s != '\0') {
    num = num * 10 + (*s - '0');
    s++;
  }
  return neg_flag * num;
}

int get_int(char *s, const int regs[]) {
  if (*s < 'a') {
    return make_int(s);
  }
  return regs[*s - 'a'];
}

struct node {
  enum Instructions instruction;
  char input_0[10];
  char input_1[10];
  struct node *next;
  struct node *prev;
};

struct node line_to_node(char line[], struct node *last_node) {
  struct node node;
  int index = 0;

  if (*line == 'j') {
    node.instruction = JNZ;
  } else if (*line == 'm') {
    node.instruction = MUL;
  } else if (*(line + 1) == 'e') {
    node.instruction = SET;
  } else if (*(line + 1) == 'u') {
    node.instruction = SUB;
  } else {
    node.instruction = NUM_INSTRUCTIONS;
  }

  line += 4;

  while (*line != ' ') {
    node.input_0[index] = *line;
    line++;
    index++;
  }
  node.input_0[index] = '\0';

  line++;
  index = 0;
  while (*line != '\n') {
    node.input_1[index] = *line;
    line++;
    index++;
  }
  node.input_1[index] = '\0';
  node.next = NULL;
  node.prev = last_node;
  return node;
}

void make_linked_list(const char *filename, struct node nodes[]) {
  FILE *fp = fopen(filename, "r");
  struct node *last_node = NULL;
  for (char line[20]; fgets(line, 19, fp);) {
    *nodes = line_to_node(line, last_node);
    if (last_node != NULL) {
      last_node->next = nodes;
    }
    last_node = nodes;
    nodes++;
  }
  fclose(fp);
}

int walk_nodes(struct node nodes[]) {
  int regs[8] = {0};
  int mul_count = 0;
  struct node *node = nodes;
  while (node != NULL) {
    switch (node->instruction) {
    case SET: {
      regs[node->input_0[0] - 'a'] = get_int(node->input_1, regs);
      node = node->next;
      break;
    }
    case SUB: {
      regs[node->input_0[0] - 'a'] -= get_int(node->input_1, regs);
      node = node->next;
      break;
    }
    case MUL: {
      mul_count++;
      regs[node->input_0[0] - 'a'] *= get_int(node->input_1, regs);
      node = node->next;
      break;
    }
    case JNZ: {
      int first_arg = get_int(node->input_0, regs);
      int second_arg = get_int(node->input_1, regs);
      if (first_arg == 0) {
        node = node->next;
      } else if (second_arg == 0) {
        return mul_count;
      } else if (second_arg > 0) {
        for (int i = 0; i < second_arg; i++) {
          node = node->next;
          if (node == NULL) {
            return mul_count;
          }
        }
      } else {
        for (int i = 0; i > second_arg; i--) {
          node = node->prev;
          if (node == NULL) {
            return mul_count;
          }
        }
      }
      break;
    }
    default: {
      fprintf(stderr, "non standard instruction\n");
      break;
    }
    }
  }
  return mul_count;
}

int count_muls(const char *filename) {
  struct node nodes[32] = {{0}};
  make_linked_list(filename, nodes);
  return walk_nodes(nodes);
}

int main(void) {
  int answer = count_muls("input.txt");
  printf("calculated answer is %d\n", answer);
}
