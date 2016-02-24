#include "resources_holder.h"

#include <stdlib.h>

// keep this in sync with texture_id enum
static const char *textures[] =  {
};

// keep this in sync with font_id enum
static const char *fonts[] =  {
  "resources/Sansation.ttf"
};

void rh_init(struct resources *r)
{
  /* TODO:  add check for malloc and createFromFile */
  r->textures = malloc(sizeof(sfTexture*) * TEXTURE_COUNT);
  for (size_t i = 0; i < TEXTURE_COUNT; ++i) {
    r->textures[i] = sfTexture_createFromFile(textures[i], NULL);
  }

  r->fonts = malloc(sizeof(sfFont*) * FONT_COUNT);
  for (size_t i = 0; i < FONT_COUNT; ++i) {
    r->fonts[i] = sfFont_createFromFile(fonts[i]);
  }
}

void rh_clear(struct resources *r)
{
  for (size_t i = 0; i < TEXTURE_COUNT; ++i)
    if (r->textures[i])
      sfTexture_destroy(r->textures[i]);
  free(r->textures);
  r->textures = NULL;

  for (size_t i = 0; i < FONT_COUNT; ++i)
    if (r->fonts[i])
      sfFont_destroy(r->fonts[i]);
  free(r->fonts);
  r->fonts = NULL;
}

sfTexture *rh_get_texture(struct resources *r, enum texture_id id)
{
  return r->textures[id];
}

sfFont *rh_get_font(struct resources *r, enum font_id id)
{
  return r->fonts[id];
}
