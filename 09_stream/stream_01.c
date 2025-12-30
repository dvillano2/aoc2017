#include <stdio.h>
#define BUF_SIZE 100000

int process_string(const char *to_process) {
  int position = 0;
  int score = 0;
  int nesting_level = 0;
  int garbage_flag = 0;
  int ignore_flag = 0;
  char c;
  while ((c = to_process[position]) != '\0') {
    position++;
    if (ignore_flag == 1) {
      ignore_flag = 0;
      continue;
    }
    if (c == '!') {
      ignore_flag = 1;
    }
    if (garbage_flag == 1) {
      if (c == '>') {
        garbage_flag = 0;
      }
      continue;
    }
    if (c == '<') {
      garbage_flag = 1;
    }
    if (c == '{') {
      nesting_level++;
    }
    if (c == '}') {
      score += nesting_level;
      nesting_level--;
    }
  }
  return score;
}

int main(void) {
  char big_string[BUF_SIZE];
  FILE *fp = fopen("big_string.txt", "r");
  size_t bytes_read = fread(big_string, 1, BUF_SIZE - 1, fp);
  big_string[bytes_read] = '\0';
  fclose(fp);

  int answer = process_string("{}");
  printf("Calculated score is %d, correct score is 1\n", answer);
  answer = process_string("{{{}}}");
  printf("Calculated score is %d, correct score is 6\n", answer);
  answer = process_string("{{},{}}");
  printf("Calculated score is %d, correct score is 6\n", answer);
  answer = process_string("{{{},{},{{}}}}");
  printf("Calculated score is %d, correct score is 16\n", answer);
  answer = process_string("{<a>,<a>,<a>,<a>}");
  printf("Calculated score is %d, correct score is 1\n", answer);
  answer = process_string("{{<ab>},{<ab>},{<ab>},{<ab>}},");
  printf("Calculated score is %d, correct score is 9\n", answer);
  answer = process_string("{{<!!>},{<!!>},{<!!>},{<!!>}}");
  printf("Calculated score is %d, correct score is 9\n", answer);
  answer = process_string("{{<a!>},{<a!>},{<a!>},{<ab>}}");
  printf("Calculated score is %d, correct score is 3\n", answer);
  answer = process_string(big_string);
  printf("Calculated score is %d, correct score is ????\n", answer);
}
