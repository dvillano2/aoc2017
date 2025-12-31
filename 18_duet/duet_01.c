#include <stdio.h>

int main(void) {
  long long last_played = -1;
  long long int a = 0;
  long long b = 0;
  long long p = 0;
  int i = 0;

  i = 31;
  a = 1;
  p *= 17;
  // p jump
  while (i != 0) {
    a *= 2;
    i--;
  }
  a--;
  i = 127;
  p = 464;
  while (i != 0) {
    p *= 8505;
    p %= a;
    p *= 129749;
    p += 12345;
    p %= a;
    b = p;
    b %= 10000;
    last_played = b;
    i--;
  }
  printf("answer is %lld\n", last_played);
  return 0;
}
