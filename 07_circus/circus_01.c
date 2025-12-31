#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1000 * 1000

struct node {
  struct node *children[27];
};

int add_node(char **s, struct node *head) {
  if (**s == '\0') {
    return -1;
  }
  struct node *node = head;
  int new_flag = 0;
  while (1) {
    int index = (**s != '_') ? (**s - 'a') : 26;
    if (node->children[index] == NULL) {
      node->children[index] = calloc(1, sizeof(struct node));
      new_flag = 1;
    }
    (*s)++;
    if (**s == '_') {
      (*s)++;
      return new_flag;
    }
    node = node->children[index];
  }
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

void process_line(char *line, char **high_buf, char **low_buf) {
  while (*line != ' ') {
    **high_buf = *line;
    (*high_buf)++;
    line++;
  }
  **high_buf = '_';
  (*high_buf)++;

  while (*line != ')') {
    line++;
  }
  if (*(line + 1) == '\n') {
    return;
  }
  while (*line != '>') {
    line++;
  }
  line += 2;

  for (; *line != '\n'; line++) {
    switch (*line) {
    case ' ':
      break;
    case ',':
      **low_buf = '_';
      (*low_buf)++;
      break;
    default:
      **low_buf = *line;
      (*low_buf)++;
      break;
    }
  }
  **low_buf = '_';
  (*low_buf)++;
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  char high_buf[BUF_SIZE] = {0};
  char low_buf[BUF_SIZE] = {0};
  char *hb = high_buf;
  char *lb = low_buf;
  for (char line[100]; fgets(line, 99, fp);) {
    process_line(line, &hb, &lb);
  }

  *hb = '\0';
  *lb = '\0';
  hb = high_buf;
  lb = low_buf;

  fclose(fp);
  struct node *head = calloc(1, sizeof(struct node));
  int done;
  do {
    done = add_node(&lb, head);
  } while (done > -1);

  do {
    done = add_node(&hb, head);
  } while (done < 1);

  hb -= 2;
  while (*hb != '_' && high_buf <= hb) {
    hb--;
  }
  hb++;
  printf("answer is ");
  while (*hb != '_') {
    printf("%c", *hb);
    hb++;
  }
  printf("\n");
  free_tree(head);
  return 0;
}
