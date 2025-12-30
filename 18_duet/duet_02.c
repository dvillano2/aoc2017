#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct queue_node {
  int value;
  struct queue_node *next;
};

struct queue_head {
  struct queue_node *first;
  struct queue_node *last;
};

void queue_push(struct queue_head *head, int value) {
  struct queue_node *new = malloc(sizeof(struct queue_node));
  new->value = value;
  new->next = NULL;
  if (head->last != NULL) {
    head->last->next = new;
  }
  head->last = new;
  if (head->first == NULL) {
    head->first = new;
  }
}

int queue_pop(struct queue_head *head) {
  struct queue_node *to_free = head->first;
  int to_use = to_free->value;
  head->first = to_free->next;
  if (head->first == NULL) {
    head->last = NULL;
  }
  free(to_free);
  return to_use;
}

void free_queue(struct queue_head *head) {
  while (head->first != NULL) {
    (void)queue_pop(head);
  }
}

enum Instructions { SET, SND, ADD, MUL, MOD, JGZ, RCV, NUM_INSTRUCTIONS };

struct instruction_node {
  enum Instructions instruction;
  char input_0[30];
  char input_1[30];
};

int make_instuction(char line[], struct instruction_node buf[], int spot) {
  const char *instruction = strtok(line, " \n");
  if (*instruction == 'a') {
    buf[spot].instruction = ADD;
  } else if (*instruction == 'r') {
    buf[spot].instruction = RCV;
  } else if (*instruction == 'j') {
    buf[spot].instruction = JGZ;
  } else if (*(instruction + 1) == 'e') {
    buf[spot].instruction = SET;
  } else if (*(instruction + 1) == 'n') {
    buf[spot].instruction = SND;
  } else if (*(instruction + 1) == 'u') {
    buf[spot].instruction = MUL;
  } else if (*(instruction + 1) == 'o') {
    buf[spot].instruction = MOD;
  } else {
    printf("bad instruction on read\n");
    return -1;
  }
  const char *tok = strtok(NULL, " \n");
  strcpy(buf[spot].input_0, tok);
  tok = strtok(NULL, " \n");
  if (tok != NULL) {
    strcpy(buf[spot].input_1, tok);
  }
  return spot + 1;
}

int get_int(const char *input, long reg[], int reg_flag) {
  if (input == NULL) {
    return 0;
  }
  if (*input >= 'a' && *input <= 'z') {
    return reg_flag == 1 ? reg[*input - 'a'] : *input - 'a';
  }
  int neg_flag = 1;
  int value = 0;
  if (*input == '-') {
    neg_flag = -1;
    input++;
  }
  while (*input != '\0') {
    value = value * 10 + *input - '0';
    input++;
  }
  return neg_flag * value;
}

int execute_instruction(int spot, int length, int *snd_count,
                        struct instruction_node buf[], long reg[],
                        struct queue_head *give_queue,
                        struct queue_head *rcv_queue,
                        int *other_program_state) {
  enum Instructions instruction = buf[spot].instruction;
  int input_0 = get_int(buf[spot].input_0, reg, 0);
  int input_1 = get_int(buf[spot].input_1, reg, 1);
  if (spot < 0 || spot >= length) {
    printf("Out of bounds");
    return -2;
  }
  switch (instruction) {
  case ADD:
    reg[input_0] += input_1;
    return spot + 1;
  case SET:
    reg[input_0] = input_1;
    return spot + 1;
  case MUL:
    reg[input_0] *= input_1;
    return spot + 1;
  case MOD:
    reg[input_0] %= input_1;
    return spot + 1;
  case SND:
    queue_push(give_queue, reg[input_0]);
    (*snd_count)++;
    *other_program_state = 0;
    return spot + 1;
  case RCV:
    if (rcv_queue->first == NULL) {
      return -100;
    }
    reg[input_0] = queue_pop(rcv_queue);
    return spot + 1;
  case JGZ:
    input_0 = get_int(buf[spot].input_0, reg, 1);
    if (input_0 > 0) {
      return spot + input_1;
    }
    return spot + 1;
  case NUM_INSTRUCTIONS:
    printf("bad instruction !\n");
    return -100000;
  default:
    printf("even weirder\n");
    return -100000;
  }
}

int main(void) {
  struct instruction_node buf[50] = {0};
  struct queue_head p0_queue = {NULL, NULL};
  struct queue_head p1_queue = {NULL, NULL};
  long p0_reg[26] = {0};
  long p1_reg[26] = {0};
  p1_reg['p' - 'a'] = 1;
  int snd_counter_0 = 0;
  int snd_counter_1 = 0;
  int length = 0;
  int p0_pos = 0;
  int p1_pos = 0;
  int p0_state = 0;
  int p1_state = 0;
  FILE *fp = fopen("input.txt", "r");
  for (char line[50]; fgets(line, 49, fp); length++) {
    make_instuction(line, buf, length);
  }
  fclose(fp);
  while (p0_state == 0 || p1_state == 0) {
    if (p0_state == 0) {
      int tmp = execute_instruction(p0_pos, length, &snd_counter_0, buf, p0_reg,
                                    &p1_queue, &p0_queue, &p1_state);
      if (tmp < 0) {
        if (tmp == -2) {
          break;
        }

        p0_state = tmp;
      } else {
        p0_pos = tmp;
      }
    } else {
      int tmp = execute_instruction(p1_pos, length, &snd_counter_1, buf, p1_reg,
                                    &p0_queue, &p1_queue, &p0_state);
      if (tmp < 0) {
        if (tmp == -2) {
          break;
        }
        p1_state = tmp;
      } else {
        p1_pos = tmp;
      }
    }
  }
  free_queue(&p0_queue);
  free_queue(&p1_queue);
  printf("answer is %d\n", snd_counter_1);
  return 0;
}
