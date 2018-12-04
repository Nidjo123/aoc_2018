#include <stdio.h>

#define MAX_DIM 1000
#define N_CLAIMS 1500

struct Claim {
  int id;
  int x, y;
  int w, h;
};

int area[MAX_DIM][MAX_DIM];

int has_overlaps(const struct Claim *claim) {
  for (int i = claim->y; i < claim->y + claim->h; i++)
    for (int j = claim->x; j < claim->x + claim->w; j++) {
      if (area[i][j] > 1)
	return 1;
    }

  return 0;
}

int main(void) {
  FILE *f = fopen("input", "r");

  if (f == NULL) {
    printf("Couldn't open the file!\n");
    return -1;
  }

  struct Claim claims[N_CLAIMS];
  int n_claims = 0;

  while (fscanf(f, "#%d @ %d,%d: %dx%d\n",
		&claims[n_claims].id,
		&claims[n_claims].x,
		&claims[n_claims].y,
		&claims[n_claims].w,
		&claims[n_claims].h) > 0) {
    const struct Claim *claim = &claims[n_claims];
    for (int i = claim->y; i < claim->y + claim->h; i++)
      for (int j = claim->x; j < claim->x + claim->w; j++) {
	area[i][j]++;
      }
    n_claims++;
  }

  fclose(f);

  int sol = 0;
  for (int i = 0; i < MAX_DIM; i++) {
    for (int j = 0; j < MAX_DIM; j++) {
      if (area[i][j] > 1)
	sol++;
    }
  }

  printf("The solution is %d.\n", sol);

  // part 2

  for (int i = 0; i < n_claims; i++) {
    if (!has_overlaps(&claims[i])) {
      printf("Claim with ID %d has no overlaps! Use it, quickly!\n", claims[i].id);
    }
  }

  return 0;
}
