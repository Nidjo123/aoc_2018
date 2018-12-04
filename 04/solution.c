#include <stdio.h>
#include <stdlib.h>

#define N_MINUTES 60
#define MAX_GUARDS 256
#define MAX_ENTRIES 2048

struct Guard {
  int id;
  int minutes[N_MINUTES];
};

enum EntryType {
		BEGINS_SHIFT,
		FALLS_ASLEEP,
		WAKES_UP,
};

struct Entry {
  int year, month, day;
  int hour, minute;
  int id;
  enum EntryType type;
};

void print_entry(const struct Entry *e) {
  printf("[%d-%02d-%02d %02d:%02d] ", e->year, e->month, e->day, e->hour, e->minute);
  switch (e->type) {
  case BEGINS_SHIFT:
    printf("Guard #%d begins shift\n", e->id);
    break;
  case FALLS_ASLEEP:
    printf("falls asleep\n");
    break;
  case WAKES_UP:
    printf("wakes up\n");
    break;
  }
}

int compare_entries(const void *p, const void *q) {
  const struct Entry *e = (const struct Entry*)p;
  const struct Entry *f = (const struct Entry*)q;

  if (e->year < f->year)
    return -1;
  else if (e->year > f->year)
    return +1;
  else if (e->month < f->month)
    return -1;
  else if (e->month > f->month)
    return +1;
  else if (e->day < f->day)
    return -1;
  else if (e->day > f->day)
    return +1;
  else if (e->hour < f->hour)
    return -1;
  else if (e->hour > f->hour)
    return +1;
  else if (e->minute < f->minute)
    return -1;
  else if (e->minute > f->minute)
    return +1;
  else if (e->type < f->type)
    return -1;
  else if (e->type > f->type)
    return +1;

  return 0;
}

void add_guard(int id, struct Guard *guards, int *guard_count) {
  guards[*guard_count].id = id;
  for (int i = 0; i < N_MINUTES; i++)
    guards[*guard_count].minutes[i] = 0;
  (*guard_count)++;
}

struct Guard* get_guard(int id, struct Guard *guards, int guard_count) {
  for (int i = 0; i < guard_count; i++) {
    if (guards[i].id == id)
      return &guards[i];
  }
  return NULL;
}

int no_guard(int id, struct Guard *guards, int guard_count) {
  for (int i = 0; i < guard_count; i++) {
    if (guards[i].id == id)
      return 0;
  }
  return 1;
}

int minutes_asleep(const struct Guard *guard) {
  int minutes = 0;
  for (int i = 0; i < N_MINUTES; i++)
    minutes += guard->minutes[i];
  return minutes;
}

int sleepiest_minute(const struct Guard *guard) {
  int minute = 0;
  int asleep = guard->minutes[0];
  for (int i = 1; i < N_MINUTES; i++)
    if (guard->minutes[i] > asleep) {
      asleep = guard->minutes[i];
      minute = i;
    }
  return minute;
}

int main(void) {
  FILE *f = fopen("input", "r");

  if (f == NULL) {
    printf("Couldn't open the file!\n");
    return -1;
  }

  struct Entry entries[MAX_ENTRIES];
  int n_entries = 0;
  char type[64];
  while (fscanf(f, "[%d-%d-%d %d:%d] %s",
		&entries[n_entries].year,
		&entries[n_entries].month,
		&entries[n_entries].day,
		&entries[n_entries].hour,
		&entries[n_entries].minute,
		type) > 0) {
    if (type[0] == 'w') {
      // wakes up
      fscanf(f, " up\n");
      entries[n_entries].type = WAKES_UP;
    } else if (type[0] == 'f') {
      // falls asleep
      fscanf(f, " asleep\n");
      entries[n_entries].type = FALLS_ASLEEP;
    } else if (type[0] == 'G') {
      // Guard begins shift
      fscanf(f, " #%d begins shift\n", &entries[n_entries].id);
      entries[n_entries].type = BEGINS_SHIFT;
    }
    n_entries++;
  }

  fclose(f);

  // sort the entires in ascending order
  qsort(entries, n_entries, sizeof(struct Entry), &compare_entries);

  struct Guard guards[MAX_GUARDS];
  int n_guards = 0;

  int current_id;
  struct Guard *guard;
  int falls_asleep_mark;
  for (int i = 0; i < n_entries; i++) {
    if (entries[i].type == BEGINS_SHIFT) {
      current_id = entries[i].id;
      if (no_guard(current_id, guards, n_guards)) {
	add_guard(current_id, guards, &n_guards);
      }
      guard = get_guard(current_id, guards, n_guards);
    } else if (entries[i].type == FALLS_ASLEEP) {
      falls_asleep_mark = entries[i].minute;
    } else if (entries[i].type == WAKES_UP) {
      for (int j = falls_asleep_mark; j < entries[i].minute; j++)
	guard->minutes[j]++;
    }
  }

  // find guard with most minutes asleep
  int sol_id;
  int most_minutes = -1;
  int chosen_minute;

  for (int i = 0; i < n_guards; i++) {
    int minutes = minutes_asleep(&guards[i]);
    if (minutes > most_minutes) {
      most_minutes = minutes;
      sol_id = guards[i].id;
      chosen_minute = sleepiest_minute(&guards[i]);
    }
  }

  printf("%d\n", sol_id * chosen_minute);

  // part 2
  // find guard that was most asleep at any certain minute

  int times_asleep = -1;
  for (int i = 0; i < n_guards; i++) {
    int minute = sleepiest_minute(&guards[i]);
    int count = guards[i].minutes[minute];
    if (count > times_asleep) {
      times_asleep = count;
      sol_id = guards[i].id;
      chosen_minute = minute;
    }
  }

  printf("%d\n", sol_id * chosen_minute);

  return 0;
}
