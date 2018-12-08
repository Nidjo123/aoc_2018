#include <stdio.h>
#include <stdlib.h>

#define INIT_CAPACITY 256

struct Node {
  // header
  int n_children;
  int n_metadata;
  // children
  struct Node **children;
  int *metadata;
};

struct Node* make_node(int n_children, int n_metadata) {
  struct Node *node = (struct Node*) malloc(sizeof(struct Node));
  node->n_children = n_children;
  node->n_metadata = n_metadata;

  if (n_children > 0) {
    node->children = (struct Node**) malloc(n_children * sizeof(struct Node));
    if (node->children == NULL) {
      printf("Couldn't allocate memory for node children!\n");
      exit(1);
    }
  }

  if (n_metadata > 0) {
    node->metadata = (int*) malloc(n_metadata * sizeof(int));
    if (node->metadata == NULL) {
      printf("Couldn't allocate memory for node metadata!\n");
      exit(1);
    }
  }

  return node;
}

void cleanup_node(struct Node *node) {
  for (int i = 0; i < node->n_children; i++)
    cleanup_node(node->children[i]);

  if (node->n_children > 0)
    free(node->children);
  if (node->n_metadata > 0)
    free(node->metadata);
}

struct Vector {
  int capacity;
  int size;
  int *data;
};

void init_vector(struct Vector *vector) {
  vector->capacity = INIT_CAPACITY;
  vector->size = 0;
  vector->data = (int*) malloc(INIT_CAPACITY * sizeof(int));
  if (vector->data == NULL) {
    printf("Couldn't allocate vector memory!");
    exit(1);
  }
}

void cleanup_vector(struct Vector *vector) {
  free(vector->data);
}

void push_back(struct Vector *vector, int value) {
  if (vector->size == vector->capacity) {
    // resize
    vector->capacity <<= 1;
    vector->data = (int*) realloc(vector->data, vector->capacity * sizeof(int));
  }
  vector->data[vector->size++] = value;
}

struct Node* build_tree(const struct Vector *vec) {
  static int pos = 0;
  int n_children = vec->data[pos++];
  int n_metadata = vec->data[pos++];
  struct Node *node = make_node(n_children, n_metadata);

  for (int i = 0; i < n_children; i++)
    node->children[i] = build_tree(vec);

  for (int i = 0; i < n_metadata; i++)
    node->metadata[i] = vec->data[pos++];

  return node;
}

int metadata_sum(const struct Node *node) {
  int sum = 0;

  for (int i = 0; i < node->n_children; i++)
    sum += metadata_sum(node->children[i]);

  for (int i = 0; i < node->n_metadata; i++)
    sum += node->metadata[i];

  return sum;
}

void print_tree(const struct Node *node, int lvl) {
  for (int i = 0; i < lvl; i++) putchar(' ');
  printf("%d %d ", node->n_children, node->n_metadata);
  for (int i = 0; i < node->n_metadata; i++)
    printf("%d ", node->metadata[i]);
  putchar('\n');
  for (int i = 0; i < node->n_children; i++)
    print_tree(node->children[i], lvl+1);
}

int main(void) {
  FILE *f = fopen("input", "r");

  if (f == NULL) {
    printf("Couldn't open the input file.\n");
    return -1;
  }

  struct Vector vec;
  init_vector(&vec);

  int x;
  while (fscanf(f, "%d", &x) == 1) {
    push_back(&vec, x);
  }
  fclose(f);

  struct Node *tree = build_tree(&vec);

  print_tree(tree, 0);

  printf("%d\n", metadata_sum(tree));

  cleanup_node(tree);
  cleanup_vector(&vec);

  return 0;
}
