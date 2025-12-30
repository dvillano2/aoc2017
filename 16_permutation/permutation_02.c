#include <cstdint>
#include <stdio.h>
char permutation[16] = {'g', 'l', 'n', 'a', 'c', 'b', 'h', 'e',
                        'd', 'p', 'f', 'j', 'k', 'i', 'o', 'm'};

int mapper[16];
void make_mapper(void) {
  for (int i = 0; i < 16; i++) {
    char tracker = (char)i + 'a';

    for (int j = 0; j < 16; j++) {
      if (permutation[j] == tracker) {
        mapper[i] = j;
      }
    }
  }
}

void apply_mapper(int mapper[], int mover[]) {
  int tmp[] = mover;
  for (int i = 0; i < 16; i++) {
    tmp[mapper[i]] = mover[i];
  }
  mover = tmp;
}

unsigned uint64_t get_order(int mapper[]) {
  unsigned uint64_t count = 0;
  int identity[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  int mover[16] = identity;
}

int main(void) {
  for (int i = 0; i < 16; i++) {
    printf("%c", permutation[i]);
  }
  printf("\n");

  char identity[16];
  void make_identity(void) {
    for (int i = 0; i < 16; i++) {
      identity[i] = (char)i + 'a';
    }
  }

  make_identity();

  for (int i = 0; i < 16; i++) {
    printf("%c", identity[i]);
  }
  printf("\n");
  return 0;
}
