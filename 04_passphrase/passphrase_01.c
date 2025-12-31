#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct word_node {
  char word[20];
  struct word_node *next_word;
};

FILE *open_file(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    exit(EXIT_FAILURE);
  }
  return fp;
}

FILE *examine_line(FILE *fp, int *score, struct word_node *base_node) {
  int c = getc(fp);
  int good_phrase = 1;
  struct word_node *node = base_node;
  struct word_node *last_node = NULL;
  int first_space = 1;
  char current_word[20];
  int char_count = 0;
  int word_count = 1;
  while (1) {
    if (c == EOF) {
      fclose(fp);
      return NULL;
    }

    if (c == ' ' || c == '\n') {
      if (first_space == 1) {
        // complete the string and set the counter back to 0
        current_word[char_count] = '\0';
        char_count = 0;

        // compare to all other words and increment word count
        for (int i = 0; i < word_count; i++) {
          if (strcmp(node->word, current_word) == 0) {
            good_phrase = 0;
          }
          last_node = node;
          node = node->next_word;
        }
        word_count++;

        // make a new node if necessary
        if (node == NULL) {
          node = malloc(sizeof(struct word_node));
          node->next_word = NULL;
          node->word[0] = '\0';
          last_node->next_word = node;
        }
        strcpy(node->word, current_word);
        node = base_node;
        last_node = NULL;
      }
      first_space = 0;

      if (c == '\n') {
        *score += good_phrase;
        return fp;
      }
    }

    else {
      current_word[char_count] = c;
      char_count++;
      first_space = 1;
    }
    c = getc(fp);
  }
}

int score_file(const char *filename) {
  FILE *fp = open_file(filename);
  int score = 0;
  struct word_node base_node = {"\0", NULL};
  while (fp != NULL) {
    fp = examine_line(fp, &score, &base_node);
  }
  struct word_node *tmp = base_node.next_word;
  while (tmp != NULL) {
    tmp = tmp->next_word;
    // free(base_node.next_word->word);
    free(base_node.next_word);
    base_node.next_word = tmp;
  }
  return score;
}

int main(void) {
  int answer = score_file("input.txt");
  printf("answer is %d\n", answer);
  return 0;
}
