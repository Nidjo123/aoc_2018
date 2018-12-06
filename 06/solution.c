#include <stdio.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MAX_POINTS 256
#define AREA_SIZE 1000

// part 2 constant
#define MAX_DISTANCE 10000

struct Point {
  int x;
  int y;
  int is_infinite;
};

struct AABB {
  int minx, miny;
  int maxx, maxy;
};

int manhattan(const struct Point *p1,
	      const struct Point *p2) {
  return ABS(p1->x - p2->x) + ABS(p1->y - p2->y);
}

struct AABB find_AABB(const struct Point *p, int n_points) {
  struct AABB aabb = {AREA_SIZE, AREA_SIZE, 0, 0};
  for (int i = 0; i < n_points; i++) {
    aabb.minx = MIN(aabb.minx, p[i].x);
    aabb.miny = MIN(aabb.miny, p[i].y);
    aabb.maxx = MAX(aabb.maxx, p[i].x);
    aabb.maxy = MAX(aabb.maxy, p[i].y);
  }

  return aabb;
}

int closest_point(int x, int y, const struct Point *points,
		  int n_points) {
  int min_dist = 999999;
  int closest_point = -1;

  struct Point p = {x, y};

  int count = 0;
  for (int i = 0; i < n_points; i++) {
    int dist = manhattan(&p, &points[i]);
    if (dist < min_dist) {
      min_dist = dist;
      closest_point = i;
      count = 1;
    } else if (dist == min_dist) {
      count++;
    }
  }

  return (count > 1 ? -1 : closest_point);
}

void mark_infinites(const int *coordinates, struct Point *points,
		    int n_points) {
    struct AABB aabb = find_AABB(points, n_points);

    for (int i = 0; i < AREA_SIZE; i++) {
      points[coordinates[i*AREA_SIZE+aabb.minx]].is_infinite = 1;
      points[coordinates[i*AREA_SIZE+aabb.maxx]].is_infinite = 1;
      points[coordinates[aabb.miny*AREA_SIZE+i]].is_infinite = 1;
      points[coordinates[aabb.maxy*AREA_SIZE+i]].is_infinite = 1;
    }
}

int area(int *coordinates, int point_id) {
  int res = 0;
  for (int i = 0; i < AREA_SIZE; i++)
    for (int j = 0; j < AREA_SIZE; j++)
      if (coordinates[i*AREA_SIZE+j] == point_id)
	res++;
  return res;
}

void calculate_distances(int *coordinates, const struct Point *points, int n_points) {
  for (int i = 0; i < AREA_SIZE; i++)
    for (int j = 0; j < AREA_SIZE; j++) {
      struct Point p = {j, i};
      coordinates[i*AREA_SIZE+j] = 0;
      for (int k = 0; k < n_points; k++)
	coordinates[i*AREA_SIZE+j] += manhattan(&p, &points[k]);
    }
}

int main(void) {
  FILE *f = fopen("input", "r");

  struct Point points[MAX_POINTS];
  int n_points = 0;
  while (fscanf(f, "%d, %d",
		&points[n_points].x,
		&points[n_points].y) > 0) {
    points[n_points++].is_infinite = 0;
  }
  fclose(f);

  int coordinates[AREA_SIZE][AREA_SIZE];

  for (int i = 0; i < AREA_SIZE; i++)
    for (int j = 0; j < AREA_SIZE; j++) {
      coordinates[i][j] = closest_point(j, i, points, n_points);
    }

  mark_infinites(&coordinates[0][0], points, n_points);

  int max_area = -1;
  for (int i = 0; i < n_points; i++) {
    if (points[i].is_infinite)
      continue;
    max_area = MAX(max_area, area(&coordinates[0][0], i));
  }

  printf("%d\n", max_area);

  // part 2

  calculate_distances(&coordinates[0][0], &points[0], n_points);

  int sol2 = 0;
  for (int i = 0; i < AREA_SIZE; i++)
    for (int j = 0; j < AREA_SIZE; j++)
      if (coordinates[i][j] < MAX_DISTANCE)
	sol2 += 1;

  printf("%d\n", sol2);

  return 0;
}
