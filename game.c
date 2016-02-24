#include "game.h"
#include "icon.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CELL_SIZE 80
#define SPACE 14

enum game_state {
  GAME_ACTION,
  GAME_OVER
};

static enum game_state state;

static void init_grid(struct grid *g)
{
  grid_init(g);
  grid_place_random(g);
  grid_place_random(g);
}

void game_init(struct world *w)
{
  srand(time(NULL));
  static const int px = SIZE * (CELL_SIZE + SPACE);

  rh_init(&w->resources);
  init_grid(&w->grid);

  state = GAME_ACTION;

  sfText *text = sfText_create();
  sfText_setFont(text, rh_get_font(&w->resources, FONT_SANSATION));
  sfText_setCharacterSize(text, 1 * CELL_SIZE / 2);
  sfText_setString(text, "0123456789");
  sfFloatRect b = sfText_getLocalBounds(text);
  int footer = SPACE / 2 + b.height;
  sfText_destroy(text);

  w->window = sfRenderWindow_create
    ((sfVideoMode){px, px + footer, 32}, "2048", sfClose, NULL);
  sfRenderWindow_setIcon(w->window, sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);

  sfView *view = sfView_copy(sfRenderWindow_getView(w->window));
  sfView_move(view, (sfVector2f) {-SPACE / 2, -SPACE / 2});
  sfRenderWindow_setView(w->window, view);
  sfView_destroy(view);

  w->wasted = sfClock_create();
}

void game_clear(struct world *w)
{
  rh_clear(&w->resources);
  sfRenderWindow_destroy(w->window);
  sfClock_destroy(w->wasted);
}

void game_process_events(struct world *w)
{
  sfEvent event;
  while (sfRenderWindow_pollEvent(w->window, &event))
    if (event.type == sfEvtClosed)
      sfRenderWindow_close(w->window);
    else if (event.type == sfEvtKeyPressed) {
      if (event.key.code == sfKeyF4 && event.key.alt) {
        sfRenderWindow_close(w->window);
        return;
      }
      if (state == GAME_ACTION) {
        if (event.key.code == sfKeyRight)
          grid_shift(&w->grid, 0, 1);
        else if (event.key.code == sfKeyLeft)
          grid_shift(&w->grid, 0, -1);
        else if (event.key.code == sfKeyUp)
          grid_shift(&w->grid, -1, 0);
        else if (event.key.code == sfKeyDown)
          grid_shift(&w->grid, 1, 0);

        if (grid_is_over(&w->grid))
          state = GAME_OVER;
      }
      else if (state == GAME_OVER) {
        init_grid(&w->grid);
        state = GAME_ACTION;
      }
    }
}

sfBool game_is_open(struct world *w)
{
  return sfRenderWindow_isOpen(w->window);
}

static void draw_base_layer(struct world *w)
{
  sfRectangleShape *s = sfRectangleShape_create();
  sfRectangleShape_setSize(s, (sfVector2f) {CELL_SIZE, CELL_SIZE});
  sfRectangleShape_setFillColor(s, sfColor_fromRGB(255, 190, 150));
  sfRectangleShape_setOrigin(s, (sfVector2f) {0, 0});

  for (int r = 0; r < SIZE; ++r)
    for (int c = 0; c < SIZE; ++c) {
      int x = c * (CELL_SIZE + SPACE);
      int y = r * (CELL_SIZE + SPACE);
      sfRectangleShape_setPosition(s, (sfVector2f) {x, y});
      sfRenderWindow_drawRectangleShape(w->window, s, NULL);
    }
  sfRectangleShape_destroy(s);
}

static void draw_tiles(struct world *w)
{
  static char s[10];
  static int size[SIZE * SIZE + 2];

  sfText *text = sfText_create();
  sfText_setFont(text, rh_get_font(&w->resources, FONT_SANSATION));

  for (int r = 0; r < SIZE; ++r)
    for (int c = 0; c < SIZE; ++c)
      if (w->grid.tiles[r][c]) {
        int i = w->grid.tiles[r][c];
        sprintf(s, "%d", 1 << i);
        sfText_setString(text, s);

        // find font size
        sfFloatRect b;
        if (size[i] == 0) {
          int lt = 0;
          int rt = 1000;
          while (lt + 1 < rt) {
            int me = (lt + rt) / 2;
            sfText_setCharacterSize(text, me);
            b = sfText_getLocalBounds(text);
            if (4 * b.width > 3 * CELL_SIZE || 4 * b.height > 3 * CELL_SIZE)
              rt = me;
            else
              lt = me;
          }
          size[i] = lt;
        }

        sfText_setCharacterSize(text, size[i]);
        b = sfText_getLocalBounds(text);
        sfVector2f o = {b.left +  b.width / 2.f, b.top + b.height / 2.f};
        sfText_setOrigin(text, o);

        sfVector2f p = {c * (CELL_SIZE + SPACE) + CELL_SIZE / 2, r * (CELL_SIZE + SPACE) + CELL_SIZE / 2};
        sfText_setPosition(text, p);

        sfRenderWindow_drawText(w->window, text, NULL);
      }

  sfText_destroy(text);
}

static void draw_statistics(struct world *w)
{
  static char s[40];

  sfText *text = sfText_create();
  sfText_setFont(text, rh_get_font(&w->resources, FONT_SANSATION));
  sfText_setColor(text, sfBlack);
  sfText_setCharacterSize(text, 1 * CELL_SIZE / 2);

  sprintf(s, "%d", w->grid.score);
  sfText_setString(text, s);
  sfFloatRect b = sfText_getLocalBounds(text);
  sfVector2f o = {b.left + b.width, b.top + b.height};
  sfText_setOrigin(text, o);

  sfVector2u v = sfRenderWindow_getSize(w->window);
  sfText_setPosition(text, (sfVector2f) {v.x - SPACE, v.y - SPACE});
  sfRenderWindow_drawText(w->window, text, NULL);

  sprintf(s, "%d", (int)sfTime_asSeconds(sfClock_getElapsedTime(w->wasted)));
  sfText_setColor(text, sfRed);
  sfText_setString(text, s);
  b = sfText_getLocalBounds(text);
  o = (sfVector2f) {b.left, b.top + b.height};
  sfText_setOrigin(text, o);

  v = sfRenderWindow_getSize(w->window);
  sfText_setPosition(text, (sfVector2f) {0, v.y - SPACE});
  sfRenderWindow_drawText(w->window, text, NULL);


  sfText_destroy(text);
}

static void draw_game_over(struct world *w)
{
  static char s[40];

  sfVector2u ws = sfRenderWindow_getSize(w->window);
  sfRectangleShape *fade = sfRectangleShape_create();
  sfRectangleShape_setSize(fade, (sfVector2f) {ws.x, ws.y});
  sfRectangleShape_setFillColor(fade, sfColor_fromRGBA(150, 200, 200, 200));
  sfRectangleShape_setPosition(fade, (sfVector2f) {-SPACE/2, -SPACE/2});

  sfRenderWindow_drawRectangleShape(w->window, fade, NULL);
  sfRectangleShape_destroy(fade);

  sfText *text = sfText_create();
  sfText_setFont(text, rh_get_font(&w->resources, FONT_SANSATION));
  sfText_setColor(text, sfBlack);
  sfText_setCharacterSize(text, CELL_SIZE/2);

  sprintf(s, "game over");
  sfText_setColor(text, sfBlack);
  sfText_setString(text, s);
  sfFloatRect b = sfText_getLocalBounds(text);
  sfVector2f o = {b.left +  b.width / 2.f, b.top + b.height / 2.f};
  sfText_setOrigin(text, o);

  static int px = (CELL_SIZE + SPACE) * SIZE - SPACE / 2;

  sfText_setPosition(text, (sfVector2f) {px / 2, px / 2});

  sfRenderWindow_drawText(w->window, text, NULL);

  sfText_destroy(text);
}

void game_render(struct world *w)
{
  sfRenderWindow_clear(w->window, sfColor_fromRGB(255, 255, 255));

  draw_base_layer(w);
  draw_tiles(w);
  draw_statistics(w);
  if (state == GAME_OVER)
    draw_game_over(w);

  sfRenderWindow_display(w->window);
}
