#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack {
  int capacity;
  int size;
  char *c;
};

void init_stack(struct Stack *stack) {
  stack->capacity = 256;
  stack->size = 0;
  stack->c = (char*)malloc(stack->capacity * sizeof(char));
}

void cleanup_stack(struct Stack *stack) {
  free(stack->c);
}

int is_empty(const struct Stack *stack) {
  return stack->size == 0;
}

char peek(const struct Stack *stack) {
  return stack->c[stack->size - 1];
}

char pop(struct Stack *stack) {
  return stack->c[stack->size--];
}

void push(struct Stack *stack, char c) {
  stack->size++;
  if (stack->size >= stack->capacity) {
    stack->capacity <<= 1;
    stack->c = (char*) realloc(stack->c, stack->capacity);
  }
  stack->c[stack->size - 1] = c;
}

int react(char c1, char c2) {
  return tolower(c1) == tolower(c2)
    && (isupper(c1) && islower(c2) || islower(c1) && isupper(c2));
}

int length(const struct Stack *container, char ignored) {
  struct Stack stack;
  init_stack(&stack);

  ignored = tolower(ignored);

  for (int i = 0; i < container->size; i++) {
    char c = container->c[i];
    if (tolower(c) == ignored)
      continue;
    if (is_empty(&stack) || !react(peek(&stack), c)) {
      push(&stack, c);
    } else {
      pop(&stack);
    }
  }

  int size = stack.size;
  cleanup_stack(&stack);

  return size;
}

int main(void) {
  FILE *f = fopen("input", "r");

  if (f == NULL) {
    printf("Couldn't open the file.\n");
    return -1;
  }

  struct Stack container;
  init_stack(&container);

  char c;
  while (fscanf(f, "%c", &c) > 0 && isalpha(c)) {
    push(&container, c);
  }
  fclose(f);

  printf("%d\n", length(&container, 0));

  // part 2

  int min_length = container.size;

  for (char c = 'a'; c <= 'z'; c++) {
    int len = length(&container, c);
    if (len < min_length)
      min_length = len;
  }

  printf("%d\n", min_length);

  cleanup_stack(&container);

  return 0;
}
