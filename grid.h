#ifndef _GRID_H_
#define _GRID_H_

#include <stdbool.h>

#define SIZE 4

struct grid {
  int tiles[SIZE][SIZE];
  int free;
  int score;
};

void grid_init(struct grid *g);
bool grid_place_random(struct grid *g);
void grid_shift(struct grid *g, int dr, int dc);
bool grid_is_over(struct grid *g);



#endif /* _GRID_H_ */
