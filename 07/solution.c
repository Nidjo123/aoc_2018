#include <stdio.h>

#define MAXN ('Z' - 'A' + 1)
#define N_WORKERS 5

#define INDEX(c) ((c) - 'A')
#define CHAR(i) ('A' + (i))

struct Node {
  char name;
  int n_next;
  char constraints[MAXN];
};

struct Worker {
  char c;
  int progress;
};

enum Status {
	     PRESENT = 1,
	     AFTER_CONSTRAINT,
	     PRINTED,
	     AVAILABLE,
};

void add_next(struct Node *nodes, char before, char after) {
  int to = INDEX(after);
  int n_next = nodes[to].n_next;

  // find appropriate place (alphabetically sorted)
  int i;
  for (i = 0; i < n_next; i++)
    if (nodes[to].constraints[i] > after)
      break;

  // make room for new member in next
  for (int j = n_next; j > i; j--)
    nodes[to].constraints[j] = nodes[to].constraints[j - 1];

  nodes[to].constraints[i] = before;
  nodes[to].n_next++;
}

char can_use(int *printed, const struct Node *node) {
  for (int i = 0; i < node->n_next; i++)
    if (!printed[INDEX(node->constraints[i])])
      return 0;
  return 1;
}

void print(struct Node *nodes) {
  static int printed[MAXN] = {0};

  int flag = 0;
  for (int i = 0; i < MAXN; i++) {
    if (printed[i] || !nodes[i].name)
      continue;

    if (can_use(printed, &nodes[i])) {
      putchar(CHAR(i));
      printed[i] = 1;
      flag = 1;
      break;
    }
  }

  if (flag)
    print(nodes);
}

int is_done(const struct Worker *worker) {
  return worker->progress > 60 + INDEX(worker->c) + 1;
}

int duration(struct Node *nodes) {
  struct Worker workers[N_WORKERS] = {0};
  int printed[MAXN] = {0}, scheduled[MAXN] = {0};

  int done = 0;
  int second;
  for (second = 0; !done; second++) {
    for (int i = 0; i < N_WORKERS; i++) {
      workers[i].progress++;
      if (workers[i].c && is_done(&workers[i])) {
	printed[INDEX(workers[i].c)] = 1;
	workers[i].c = 0;
	scheduled[i] = 0;
      }
    }

    done = 1;
    for (int i = 0; i < MAXN; i++) {
      if (printed[i] || !nodes[i].name)
	continue;

      if (can_use(printed, &nodes[i]) && !scheduled[i]) {
	// assign it to a worker
	for (int j = 0; j < N_WORKERS; j++)
	  if (!workers[j].c || is_done(&workers[j])) {
	    workers[j].c = CHAR(i);
	    workers[j].progress = 1;
	    scheduled[i] = 1;
	    break;
	  }
      }
      done = 0;
    }
  }

  return second - 1;
}

int main(void) {
  FILE *f = fopen("input", "r");

  if (f == NULL) {
    printf("Couldn't open the file!\n");
    return -1;
  }

  struct Node nodes[MAXN] = {0};

  int flags[MAXN] = {0};

  char c_before, c_after;
  char c_start;
  while (fscanf(f,
		"Step %c must be finished before step %c can begin.\n",
		&c_before, &c_after) > 0) {
    add_next(nodes, c_before, c_after);

    nodes[INDEX(c_before)].name = CHAR(c_before);
    nodes[INDEX(c_after)].name = CHAR(c_after);
  }
  fclose(f);

  print(nodes);
  putchar('\n');

  // part 2
  printf("%d\n", duration(nodes));

  return 0;
}
