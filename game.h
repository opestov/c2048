#ifndef _GAME_H_
#define _GAME_H_

#include "resources_holder.h"
#include "grid.h"
#include <SFML/Graphics.h>

struct world
{
  sfRenderWindow *window;
  sfClock *wasted;
  struct resources resources;
  struct grid grid;
};

void game_init(struct world *w);
void game_clear(struct world *w);

sfBool game_is_open(struct world *w);
void game_process_events(struct world *w);
void game_render(struct world *w);

#endif /* _GAME_H_ */
