#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define INIT_CAPACITY 64
#define HEIGHT_LIMIT 30

struct Particle {
  int x, y;
  int dx, dy;
};

struct AABB {
  int minx, miny;
  int maxx, maxy;
};

struct Vector {
  int capacity;
  int size;
  struct Particle *data;
};

void init_vector(struct Vector *vector) {
  vector->capacity = INIT_CAPACITY;
  vector->size = 0;
  vector->data = (struct Particle*) malloc(INIT_CAPACITY * sizeof(struct Particle));
  if (vector->data == NULL) {
    printf("Couldn't allocate vector memory!");
    exit(1);
  }
}

void cleanup_vector(struct Vector *vector) {
  free(vector->data);
}

void push_back(struct Vector *vector, struct Particle value) {
  if (vector->size == vector->capacity) {
    // resize
    vector->capacity <<= 1;
    vector->data = (struct Particle*) realloc(vector->data,
					      vector->capacity * sizeof(struct Particle));
  }
  vector->data[vector->size++] = value;
}

void update_AABB(struct AABB *aabb, int x, int y) {
  aabb->minx = MIN(aabb->minx, x);
  aabb->miny = MIN(aabb->miny, y);
  aabb->maxx = MAX(aabb->maxx, x);
  aabb->maxy = MAX(aabb->maxy, y);
}

void assure_positive_quadrant(struct Vector *particles, struct AABB *aabb) {
  for (int i = 0; i < particles->size; i++) {
    particles->data[i].x -= aabb->minx;
    particles->data[i].y -= aabb->miny;
  }
  aabb->maxx -= aabb->minx - 1;
  aabb->maxy -= aabb->miny - 1;
  aabb->minx = 0;
  aabb->miny = 0;
}

void update_particles(struct Vector *particles, struct AABB *aabb) {
  *aabb = (struct AABB){999999, 999999, -999999, -999999};
  for (int i = 0; i < particles->size; i++) {
    particles->data[i].x += particles->data[i].dx;
    particles->data[i].y += particles->data[i].dy;
    update_AABB(aabb, particles->data[i].x, particles->data[i].y);
  }

  assure_positive_quadrant(particles, aabb);
}

void print_particles(const struct Vector *particles, const struct AABB *aabb) {
  for (int i = 0; i < aabb->maxy; i++) {
    for (int j = 0; j < aabb->maxx; j++) {
      int has_particle = 0;
      for (int k = 0; k < particles->size; k++)
	if (particles->data[k].x == j && particles->data[k].y == i)
	  has_particle = 1;

      printf("%c", has_particle ? '#' : '.');
    }
    printf("\n");
  }
  printf("\n");
}

int main(void) {
  FILE *f = fopen("input", "r");

  if (f == NULL) {
    printf("Couldn't open the input file!\n");
    return 1;
  }

  struct Vector particles;
  init_vector(&particles);

  struct AABB aabb = {999999, 999999, -999999, -999999};

  int x, y, dx, dy;
  while (fscanf(f, "position=<%d, %d> velocity=<%d, %d>\n", &x, &y, &dx, &dy) > 0) {
    push_back(&particles, (struct Particle){x, y, dx, dy});
    update_AABB(&aabb, x, y);
  }
  fclose(f);

  assure_positive_quadrant(&particles, &aabb);

  int second = 0;
  while (aabb.maxy - aabb.miny > HEIGHT_LIMIT) {
    // update particle positions
    update_particles(&particles, &aabb);
    second++;
  }

  while (aabb.maxy - aabb.miny <= HEIGHT_LIMIT) {
    printf("Second %d\n", second++);
    print_particles(&particles, &aabb);
    update_particles(&particles, &aabb);
  }

  cleanup_vector(&particles);

  return 0;
}
