#include "common.h"

unsigned long rng_next()
{
  static int prepared;
  static unsigned long x, y, z, w, v;
  if (!prepared) {
    srand(time(NULL));
    x = rand(), y = rand(), z = rand(), w = rand(), v = rand();
    prepared = 0;
  }

  unsigned long t = (x ^ (x >> 7));
  x = y; y = z; z = w; w = v;
  v = (v ^ (v << 6)) ^ (t ^ (t << 13));
  return (y + y + 1) * v;
}

int rng_next_ab(int a, int b)
{
/*
  unsigned long diff, range, i;
  diff = b - a + 1;
  range = (0x7FFFFFFF / diff) * diff;
  for (i = rng_next(); i >= range; i = rng_next())
    ;
  return a + (i % diff);
*/
  return a + rand() % (b - a + 1);
}
