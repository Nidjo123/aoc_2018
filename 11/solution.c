#include <stdio.h>
#include <assert.h>

#define SERIAL_NUMBER 7989
#define MAXN 300

struct Result {
  int x, y;
  int power;
  int size;
};

int grid[MAXN][MAXN];

int power(int x, int y, int serial_number) {
  const int rack_ID = x+10;
  return (rack_ID*y+serial_number)*rack_ID/100%10-5;
}

void init_grid(int serial_number) {
  for (int i = 0; i < MAXN; i++)
    for (int j = 0; j < MAXN; j++)
      grid[i][j] = power(j, i, serial_number);
}

int power_NxN(int x, int y, int n) {
  assert(x >= 0 && y >= 0 && x <= MAXN-n && y <= MAXN-n);

  int power_sum = 0;
  for (int i = y; i < y+n; i++)
    for (int j = x; j < x+n; j++)
      power_sum += grid[i][j];

  return power_sum;
}

struct Result max_power_NxN(int n) {
  int maxi=-1, maxj, maxp;

  for (int i = 0; i < MAXN-n+1; i++)
    for (int j = 0; j < MAXN-n+1; j++) {
      int p = power_NxN(j, i, n);
      if (maxi < 0 || maxp < p) {
	maxi = i;
	maxj = j;
	maxp = p;
      }
    }

  return (struct Result){maxj, maxi, maxp, n};
}

void self_test() {
  assert(power(3, 5, 8) == 4);
  assert(power(122, 79, 57) == -5);
  assert(power(217, 196, 39) == 0);
  assert(power(101, 153, 71) == 4);

  init_grid(18);
  struct Result res = max_power_NxN(3);
  assert(res.x == 33 && res.y == 45);
}

int main(void) {
  self_test();

  init_grid(SERIAL_NUMBER);

  struct Result res = max_power_NxN(3);
  printf("%d,%d\n", res.x, res.y);

  // part 2

  for (int n = 1; n <= MAXN; n++) {
    if (n == 3) // already done
      continue;
    const struct Result new_res = max_power_NxN(n);
    if (new_res.power >= res.power)
      res = new_res;
  }

  printf("%d,%d,%d\n", res.x, res.y, res.size);

  return 0;
}
