#ifndef _RESOURCES_HOLDER_H_
#define _RESOURCES_HOLDER_H_

#include <SFML/Graphics.h>

enum texture_id {
  TEXTURE_COUNT = 0
};

enum font_id {
  FONT_SANSATION = 0,
  FONT_COUNT
};

struct resources {
  sfTexture **textures;
  sfFont **fonts;
};

void rh_init(struct resources *r);
void rh_clear(struct resources *r);

sfTexture *rh_get_texture(struct resources *r, enum texture_id);
sfFont *rh_get_font(struct resources *r, enum font_id);

#endif /* _RESOURCES_HOLDER_H_ */
