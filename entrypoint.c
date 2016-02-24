#include <windows.h>
#include <stdio.h>

#include <SFML/Graphics.h>
#include <SFML/System.h>

#include "game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  struct world w;
  game_init(&w);

  while (game_is_open(&w)) {
    game_process_events(&w);
    game_render(&w);
    sfSleep(sfMilliseconds(1000.f / 40.f));
  }

  game_clear(&w);
}

/*
int main()
{
  struct world w;
  game_init(&w);

  while (game_is_open(&w)) {
    game_process_events(&w);
    game_render(&w);
    sfSleep(sfMilliseconds(1000.f / 40.f));
  }

  game_clear(&w);
  return 0;
}
*/
