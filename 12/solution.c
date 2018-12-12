#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_STATE_SIZE 128
#define N_TRANSITIONS 32
#define N_GENERATIONS 20
#define PADDING 4
#define N_GENERATIONS2 50000000000ull

int last, diff;

struct Transition {
  char from[5+1];
  char to;
} transitions[N_TRANSITIONS];

struct State {
  int size;
  char *data;
  int first;
};

void init_state(struct State *state) {
  state->size = INIT_STATE_SIZE;
  state->data = (char*) malloc(state->size * sizeof(char));
  if (state->data == NULL) {
    printf("Couldn't allocate memory for state data!\n");
    exit(1);
  }

  state->first = 0;

  for (int i = 0; i < state->size; i++)
    state->data[i] = '.';
}

void cleanup_state(struct State *state) {
  if (state->data)
    free(state->data);
}

void print_state(const struct State *state) {
  for (int i = 0; i < state->size; i++)
    putchar(state->data[i]);
  putchar('\n');
}

int pot_sum(const struct State *state) {
  int sum = 0;
  for (int i = 0; i < state->size; i++)
    if (state->data[i] == '#')
      sum += state->first + i;

  return sum;
}

void next_state(struct State *state) {
  static int generation = 0;
  // find how much padding there is on each side
  int left_pad = 0;
  int right_pad = 0;

  while (state->data[left_pad] == '.')
    left_pad++;

  while (state->data[state->size-right_pad-1] == '.')
    right_pad++;

  if (left_pad + right_pad < 2 * PADDING) {
    // we have to resize
    state->size <<= 1;
    state->data = (char*) realloc(state->data, state->size * sizeof(char));
    for (int i = state->size >> 1; i < state->size; i++)
      state->data[i] = '.';
    for (int i = (state->size>>1)-1+PADDING-left_pad; i >= PADDING-left_pad; i--)
      state->data[i] = state->data[i-PADDING+left_pad];
    state->first -= PADDING-left_pad;
  } else if (left_pad+right_pad-PADDING >= PADDING && right_pad < PADDING) {
    // move to the left
    for (int i = 0; i <= state->size-PADDING; i++)
      state->data[i] = state->data[i+PADDING-right_pad];
    state->first += PADDING-right_pad;
  } else if (left_pad < PADDING && right_pad+left_pad-PADDING >= PADDING) {
    // move to the right
    for (int i = state->size-1; i >= PADDING-left_pad; i--)
      state->data[i] = state->data[i-PADDING+left_pad];
    state->first -= PADDING-left_pad;
  }

  for (int i = 0; i < PADDING; i++) {
    state->data[i] = '.';
    state->data[state->size-i-1] = '.';
  }

  left_pad = 0;
  while (state->data[left_pad] == '.')
    left_pad++;

  if (left_pad > PADDING) {
    for (int i = PADDING; i < state->size-PADDING; i++)
      state->data[i] = state->data[i+left_pad-PADDING];
    state->first += left_pad-PADDING;
  }

  char *tmp_state = (char*) malloc(state->size * sizeof(char));
  for (int i = 0; i < state->size; i++)
    tmp_state[i] = '.';

  for (int i = 0; i <= state->size - 5; i++)
    for (int j = 0; j < N_TRANSITIONS; j++)
      if (!strncmp(transitions[j].from, state->data+i, 5)) {
	tmp_state[i+2] = transitions[j].to;
	break;
      }

  for (int i = 0; i < state->size; i++)
    state->data[i] = tmp_state[i];

  generation++;
  int psum = pot_sum(state);

  if (!last) {
    last = psum;
  } else {
    diff = psum - last;
    last = psum;
  }

  free(tmp_state);
}

int main(void) {
  FILE *f = fopen("input", "r");

  if (f == NULL) {
    printf("Couldn't open the input file.\n");
    return 1;
  }

  struct State state;

  init_state(&state);

  fscanf(f, "initial state: %s\n\n", state.data);

  int i = 0;
  while (fscanf(f, "%s => %c\n",
		&transitions[i].from[0],
		&transitions[i].to) > 0)
    i++;

  fclose(f);

  for (int i = state.size-1; state.data[i] != '#'; i--)
    state.data[i] = '.';

  for (int i = 0; i < N_GENERATIONS; i++)
    next_state(&state);

  printf("%d\n", pot_sum(&state));

  // part 2
  // after the pattern stabilizes, the pot sum raises by
  // constant difference

  const int gens = 200;
  for (int i = 0; i < gens - N_GENERATIONS; i++)
    next_state(&state);

  unsigned long long res = last+(N_GENERATIONS2-gens)*diff;
  printf("%llu\n", res);

  cleanup_state(&state);

  return 0;
}
