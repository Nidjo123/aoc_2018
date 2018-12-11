#include <stdio.h>
#include <stdlib.h>

struct Node {
  int n;
  struct Node *prev;
  struct Node *next;
};

struct Node* init_list() {
  struct Node *head = (struct Node*) malloc(sizeof(struct Node));
  if (head == NULL) {
    printf("Couldn't initalize list!\n");
    exit(1);
  }

  head->n = 0;
  head->next = head;
  head->prev = head;

  return head;
}

struct Node* insert(struct Node *head, int value) {
  for (int i = 0; i < 2; i++)
    head = head->next;

  struct Node *node = (struct Node*) malloc(sizeof(struct Node));
  if (node == NULL) {
    printf("Couldn't allocate node!\n");
    exit(1);
  }

  node->n = value;
  node->prev = head->prev;
  node->next = head;

  head->prev = node;
  node->prev->next = node;

  // printf("Adding node %d <- %d -> %d\n", node->prev->n, node->n, node->next->n);

  return node;
}

int removed_marble;
struct Node* remove_marble7(struct Node *head) {
  for (int i = 0; i < 7; i++)
    head = head->prev;

  head->prev->next = head->next;
  head->next->prev = head->prev;

  struct Node *new_current = head->next;
  removed_marble = head->n;

  free(head);

  return new_current;
}

void cleanup_list(struct Node *head) {
  struct Node *current = head->next;
  struct Node *next;

  do {
    next = current->next;
    free(current);
    current = next;
  } while (current != head);

  free(head);
}

void print_list(const struct Node *head) {
  const struct Node *current = head;

  do {
    printf("%d ", current->n);
    current = current->next;
  } while (current != head);

  printf("\n");
}

unsigned max(unsigned *a, int n) {
  unsigned x = a[0];
  for (int i = 1; i < n; i++)
    if (a[i] > x)
      x = a[i];

  return x;
}

unsigned high_score(int n_players, int last_marble) {
  struct Node *current = init_list();
  unsigned *scores = (unsigned*) calloc(n_players, sizeof(unsigned));

  for (int i = 1; i <= last_marble; i++) {
    if (i % 23 == 0) {
      int player = (i-1)%n_players;
      scores[player] += i;
      current = remove_marble7(current);
      scores[player] += removed_marble;
    } else {
      current = insert(current, i);
    }
    // print_list(current);
  }

  unsigned best_score = max(scores, n_players);

  free(scores);
  cleanup_list(current);

  return best_score;
}

int main(void) {
  FILE *f = fopen("input", "r");

  if (f == NULL) {
    printf("Couldn't open the input file!");
    return 1;
  }

  int n_players, last_marble;
  fscanf(f, "%d players; last marble is worth %d points", &n_players, &last_marble);
  fclose(f);

  unsigned score = high_score(n_players, last_marble);

  printf("%u\n", score);

  // part 2
  score = high_score(n_players, 100 * last_marble);

  printf("%u\n", score);

  return 0;
}
