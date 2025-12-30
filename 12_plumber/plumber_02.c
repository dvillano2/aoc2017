#include <stdio.h>

#define BUF_SIZE 1000 * 1000

void fill_buf(const char *filename, char buf[]) {
  FILE *fp = fopen(filename, "r");
  size_t bytes_read = fread(buf, 1, BUF_SIZE - 1, fp);
  buf[bytes_read] = '\0';
  fclose(fp);
}

struct disjoint {
  int parents[2000];
  int size[2000];
};

int parent(int set, struct disjoint *data) {
  // printf("parent\n");
  int current_parent = data->parents[set];
  if (current_parent != set) {
    data->parents[set] = parent(current_parent, data);
    return data->parents[set];
  }
  return set;
}

void merge(int set_0, int set_1, struct disjoint *data) {
  int a = parent(set_0, data);
  int b = parent(set_1, data);
  if (a == b) {
    return;
  }
  if (b < a) {
    int tmp = a;
    a = b;
    b = tmp;
  }
  data->parents[b] = a;
  data->size[a] += data->size[b];
}

char **process_line(char **p, struct disjoint *structure) {
  int head = 0;
  int others[10] = {0};
  int count = 0;

  while (**p != ' ') {
    head = head * 10 + (**p - '0');
    (*p)++;
  }
  while (**p == ' ' || **p == '>' || **p == '<' || **p == '-') {
    (*p)++;
  }
  while (**p != '\n') {
    if (**p == ',') {
      count++;
      (*p)++;
    } else {
      others[count] = others[count] * 10 + (**p - '0');
    }
    (*p)++;
  }
  others[count + 1] = -1;
  count = 0;
  while (others[count] != -1) {
    int parent_head = parent(head, structure);
    int other_head = parent(others[count], structure);
    merge(parent_head, other_head, structure);
    count++;
  }
  (*p)++;
  return p;
}

int proces_buf(char buf[], struct disjoint *structure) {
  char **p = &buf;
  while (**p != '\0') {
    p = process_line(p, structure);
  }
  int groups = 0;
  // below only works for the big input
  // lots of groups that don't exist counted
  for (int i = 0; i < 2000; i++) {
    if (structure->parents[i] == i) {
      groups++;
    }
  }
  return groups;
}

int get_zero_size(const char *filename) {
  char buf[BUF_SIZE];
  fill_buf(filename, buf);
  struct disjoint structure;
  for (int i = 0; i < 2000; i++) {
    structure.parents[i] = i;
    structure.size[i] = 1;
  }

  return proces_buf(buf, &structure);
}

int main(void) {
  int answer = get_zero_size("small_input.txt");
  printf("Calcluated answer is %d, correct answer is 2\n", answer);
  answer = get_zero_size("input.txt");
  printf("Calcluated answer is %d, correct answer is ????\n", answer);
}
