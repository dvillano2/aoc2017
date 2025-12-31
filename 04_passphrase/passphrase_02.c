#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct word_node {
  char letters[26];
  struct word_node *next_word;
};

FILE *open_file(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "Could not open file\n");
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
  int current_word[26] = {0};
  int word_count = 1;
  while (1) {
    if (c == EOF) {
      fclose(fp);
      return NULL;
    }

    if (c == ' ' || c == '\n') {
      if (first_space == 1) {
        // compare to all other words and increment word count
        for (int i = 0; i < word_count; i++) {
          int tracker = 26;
          for (int j = 0; j < 26; j++) {
            if (current_word[j] == node->letters[j]) {
              tracker -= 1;
            }
          }
          if (tracker == 0) {
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
          for (int j = 0; j < 26; j++) {
            node->letters[j] = 0;
          }
          last_node->next_word = node;
        }
        for (int j = 0; j < 26; j++) {
          node->letters[j] = current_word[j];
        }
        // if (node->word != NULL) {
        //   strcpy(node->word, current_word);
        // } else {
        //   node->word = strdup(current_word);
        //   printf("mallocd for word %s\n", current_word);
        // }
        node = base_node;
        last_node = NULL;
        for (int j = 0; j < 26; j++) {
          current_word[j] = 0;
        }
      }
      first_space = 0;

      if (c == '\n') {
        *score += good_phrase;
        return fp;
      }
    }

    else {
      current_word[c - 'a']++;
      first_space = 1;
    }
    c = getc(fp);
  }
}

int score_file(const char *filename) {
  FILE *fp = open_file(filename);
  int score = 0;
  struct word_node base_node;
  base_node.next_word = NULL;
  for (int j = 0; j < 26; j++) {
    base_node.letters[j] = -1;
  }
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
