#include <stdio.h>
#include <stdlib.h>

#define MAXN 1024

int sum(const int *fchanges, int n_fchanges) {
  int s = 0;
  for (int i = 0; i < n_fchanges; i++)
    s += fchanges[i];
  return s;
}

int main(void) {
  FILE *f = fopen("input", "r");

  if (f == NULL) {
    printf("Couldn't open the file!\n");
    return -1;
  }

  int frequency_changes[MAXN];
  int n_fchanges = 0;

  while (fscanf(f, "%d", &frequency_changes[n_fchanges]) > 0)
    n_fchanges++;

  printf("%d\n", sum(frequency_changes, n_fchanges));

  // part 2

  int found_duplicate = 0;
  int current_fc = 0;
  int frequency = 0;

  int *frequencies = (int*) malloc(MAXN * sizeof(int));
  if (frequencies == NULL) {
    printf("Allocation error!\n");
    return -1;
  }
  frequencies[0] = frequency;
  int n_frequencies = 1;
  int capacity = MAXN;

  while (!found_duplicate) {
    frequency += frequency_changes[current_fc];
    current_fc = (current_fc + 1) % n_fchanges;
    for (int i = 0; i < n_frequencies; i++) {
      if (frequency == frequencies[i]) {
	printf("%d\n", frequency);
	found_duplicate = 1;
      }
    }
    if (!found_duplicate) {
      n_frequencies++;
      if (n_frequencies >= capacity) {
	capacity <<= 1;
	frequencies = (int*)realloc(frequencies, capacity * sizeof(int));
	if (frequencies == NULL) {
	  printf("Reallocation error!\n");
	  return -1;
	}
      }
      frequencies[n_frequencies - 1] = frequency;
    }
  }

  free(frequencies);

  return 0;
}
