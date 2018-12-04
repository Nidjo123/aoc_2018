#include <stdio.h>
#include <string.h>

#define MAX_LEN 256
#define N_CHARS ('z' - 'a' + 1)

int difference_count(const char* const s1, const char* const s2) {
  int diffs = 0;
  for (int i = 0; s1[i] && s2[i]; i++) {
    if (s1[i] != s2[i]) diffs++;
  }
  return diffs;
}

int main(void) {
  FILE* f = fopen("input", "r");

  if (f == NULL) {
    printf("Couldn't open file!\n");
    return -1;
  }

  char lines[MAX_LEN][MAX_LEN + 1];

  int counter[N_CHARS];

  int line_count = 0;
  int n2 = 0;
  int n3 = 0;
  while (fgets(lines[line_count], MAX_LEN, f)) {
    for (int i = 0; i < N_CHARS; i++)
      counter[i] = 0;

    for (int i = 0; lines[line_count][i]; i++)
      counter[lines[line_count][i] - 'a']++;

    int found2 = 0;
    int found3 = 0;
    for (int i = 0; i < N_CHARS; i++) {
      if (counter[i] == 2)
	found2 = 1;
      else if (counter[i] == 3)
	found3 = 1;
    }

    if (found2)
      n2++;
    if (found3)
      n3++;

    line_count++;
  }

  printf("Checksum is %d.\n", n2 * n3);

  // part 2
  for (int i = 0; i < line_count - 1; i++) {
    for (int j = i + 1; j < line_count; j++) {
      if (difference_count(lines[i], lines[j]) == 1) {
	// found the solution!
	const char *c1 = lines[i];
	const char *c2 = lines[j];
	while (*c1 && *c2) {
	  if (*c1 == *c2)
	    printf("%c", *c1);
	  c1++;
	  c2++;
	}
      }
    }
  }

  printf("\n");

  return 0;
}
