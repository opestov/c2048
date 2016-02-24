#include "grid.h"
#include "common.h"

#include <string.h>


void grid_init(struct grid *g)
{
  for (int i = 0; i < SIZE; ++i)
    for (int j = 0; j < SIZE; ++j)
      g->tiles[i][j] = 0;
  g->free = SIZE * SIZE;
  g->score = 0;
}

bool grid_place_random(struct grid *g)
{
  if (g->free == 0)
    return false;

  int x = (9 + rng_next_ab(0, 9)) / 9; // 0..8 -> 1, 9 -> 2
  int z = rng_next_ab(0, g->free - 1);

  for (int i = 0; i < SIZE; ++i)
    for (int j = 0; j < SIZE; ++j)
      if (g->tiles[i][j] == 0)
        if ((z--) == 0) {
          g->tiles[i][j] = x;
          g->free--;
          return true;
        }

  return false;
}

void grid_shift(struct grid *g, int dr, int dc)
{
  int (*a)[SIZE];
  a = g->tiles;

  bool flag = false;
  for (int step = 0; step < SIZE - 1; ++step)
    for (int i = 0; i < SIZE; ++i) {
      // find the farthest cell in the given direction
      int r, c;
      for (r = i, c = i; r >= 0 && r < SIZE && c >= 0 && c < SIZE; r += dr, c += dc)
        ;
      r -= dr;
      c -= dc;

      for (int j = 0; j < SIZE - 1; ++j) {
        r -= dr;
        c -= dc;
        if (a[r][c] <= 0 || a[r + dr][c + dc] < 0) continue;
        if (a[r + dr][c + dc] == 0) {
          flag = true;
          a[r + dr][c + dc] = a[r][c];
          a[r][c] = 0;
        }
        else if (a[r + dr][c + dc] == a[r][c]) {
          flag = true;
          g->score += 1 << (a[r][c] + 1);
          g->free++;
          a[r + dr][c + dc] = -(a[r][c] + 1);
          a[r][c] = 0;
        }
      }
    }
  for (int i = 0; i < SIZE; ++i)
    for (int j = 0; j < SIZE; ++j)
      if (a[i][j] < 0)
        a[i][j] = - a[i][j];

  if (flag)
    grid_place_random(g);
}

bool grid_is_over(struct grid *g)
{
  if (g->free > 0)
    return false;

  struct grid b;
  memcpy(&b, g, sizeof(struct grid));

  static int dc[4] = {1, 0, -1, 0};
  static int dr[4] = {0, 1, 0, -1};

  for (int i = 0; i < 4; ++i) {
    grid_shift(&b, dr[i], dc[i]);
    if (b.score > g->score)
      return false;
    else
      memcpy(&b, g, sizeof(struct grid));
  }

  return true;
}
