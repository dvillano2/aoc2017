#include <stdio.h>
#include <stdlib.h>

#define NUM_PARTICLES 1000

struct particle {
  int position[3];
  int velocity[3];
  int acceleration[3];
  int current_ranking[3];
  int global_ranking[3];
  int gone;
  int tie_break;
};

int comparer(int a, int b) { return ((a > b) - (a < b)); }

int sort_index = 0;

int global_comparision(const void *a, const void *b) {
  const struct particle *p0 = (const struct particle *)a;
  const struct particle *p1 = (const struct particle *)b;

  int c;
  for (int i = 0; i < 3; i++) {
    int index = (sort_index + i) % 3;
    c = comparer(p0->acceleration[index], p1->acceleration[index]);
    if (c != 0) {
      return c;
    }
    c = comparer(p0->velocity[index], p1->velocity[index]);
    if (c != 0) {
      return c;
    }
    c = comparer(p0->position[index], p1->position[index]);
    if (c != 0) {
      return c;
    }
  }
  c = comparer(p0->tie_break, p1->tie_break);
  if (c != 0) {
    return c;
  }
  return 0;
}

int local_comparision(const void *a, const void *b) {
  const struct particle *p0 = (const struct particle *)a;
  const struct particle *p1 = (const struct particle *)b;
  int c;
  for (int i = 0; i < 3; i++) {
    int index = (sort_index + i) % 3;
    c = comparer(p0->position[index], p1->position[index]);
    if (c != 0) {
      return c;
    }
  }
  c = comparer(p0->gone, p1->gone);
  if (c != 0) {
    return c;
  }
  return global_comparision(a, b);
}

void assign_global_rankings(struct particle particles[]) {
  for (int j = 0; j < 3; j++) {
    sort_index = j;
    qsort(particles, NUM_PARTICLES, sizeof(struct particle),
          global_comparision);
    for (int i = 0; i < NUM_PARTICLES; i++) {
      (particles + i)->global_ranking[j] = i;
    }
  }
}

void assign_local_rankings(struct particle particles[]) {
  for (int j = 0; j < 3; j++) {
    sort_index = j;
    qsort(particles, NUM_PARTICLES, sizeof(struct particle), local_comparision);
    for (int i = 0; i < NUM_PARTICLES; i++) {
      (particles + i)->current_ranking[j] = i;
    }
  }
}

void update_positions(struct particle particles[]) {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    struct particle *p = particles + i;
    for (int j = 0; j < 3; j++) {
      p->velocity[j] += p->acceleration[j];
      p->position[j] += p->velocity[j];
    }
  }
  assign_local_rankings(particles);
}

int check_pos(struct particle p0, struct particle p1) {
  return p0.position[0] == p1.position[0] && p0.position[1] == p1.position[1] &&
         p0.position[2] == p1.position[2];
}

void collide(struct particle particles[]) {
  int i = 0;
  while (i < NUM_PARTICLES) {
    int j = i + 1;
    if (particles[i].gone == 0) {
      while (j < NUM_PARTICLES && particles[j].gone == 0 &&
             check_pos(particles[i], particles[j]) == 1) {
        j++;
      }
    }
    if (j == i + 1) {
      i = j;
    } else {
      for (; i < j; i++) {
        particles[i].gone = 1;
      }
    }
  }
}

int count_remaining(const struct particle particles[]) {
  int count = 0;
  for (int i = 0; i < NUM_PARTICLES; i++) {
    if (particles[i].gone == 0) {
      count++;
    }
  }
  return count;
}

int correct_monotonicity(struct particle particle, int i) {
  int a = particle.acceleration[i];
  int v = particle.velocity[i];
  if (a == 0) {
    return 1;
  }
  return ((a ^ v) > 0);
}

int compare_rankings(struct particle particles[]) {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    const struct particle *p = particles + i;
    int ranker = 1;
    for (int j = 0; j < 3; j++) {

      if (correct_monotonicity(*p, j) == 0) {
        return 1;
      }

      if (p->global_ranking[j] == p->current_ranking[j]) {
        ranker = 0;
      }
    }
    if (ranker == 1) {
      return 1;
    }
  }
  return 0;
}

int get_int(char **buf) {
  int value = 0;
  int neg_flag = 1;
  if (**buf == '-') {
    neg_flag = -1;
    (*buf)++;
  }
  while (**buf != ',' && **buf != '>') {
    value = value * 10 + (**buf - '0');
    (*buf)++;
  }
  (*buf)++;
  return neg_flag * value;
}

void parse_line(char line[], struct particle particles[], int i) {
  struct particle particle = {0};
  line += 3;
  particle.position[0] = get_int(&line);
  particle.position[1] = get_int(&line);
  particle.position[2] = get_int(&line);
  line += 5;
  particle.velocity[0] = get_int(&line);
  particle.velocity[1] = get_int(&line);
  particle.velocity[2] = get_int(&line);
  line += 5;
  particle.acceleration[0] = get_int(&line);
  particle.acceleration[1] = get_int(&line);
  particle.acceleration[2] = get_int(&line);
  particle.tie_break = i;
  particles[i] = particle;
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  if (!fp) {
    perror("input.txt");
    return 1;
  }
  struct particle particles[NUM_PARTICLES] = {0};
  int i = 0;
  for (char line[100]; fgets(line, 99, fp); i++) {
    parse_line(line, particles, i);
  }
  fclose(fp);
  assign_global_rankings(particles);
  assign_local_rankings(particles);
  while (compare_rankings(particles) == 1) {
    update_positions(particles);
    collide(particles);
  }
  printf("\nRemaining particles: %d\n\n", count_remaining(particles));
  return 0;
}
