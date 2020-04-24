#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Constance.h"
#include "Gem.h"

//The music that will be played background
Mix_Music *gMusic = NULL;

//The sound effects that will be used
Mix_Chunk *bad = NULL;
Mix_Chunk *nice = NULL;
Mix_Chunk *click = NULL;
Mix_Chunk *combo2 = NULL;
Mix_Chunk *combo3 = NULL;
Mix_Chunk *combo4 = NULL;
Mix_Chunk *combo5 = NULL;
Mix_Chunk *combo6 = NULL;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *scoreFont = NULL;
TTF_Font *timeFont = NULL;

//Mouse button sprites
SDL_Rect blueSpriteClips[ GEM_SPRITE_TOTAL ];
SDL_Rect greenSpriteClips[ GEM_SPRITE_TOTAL ];
SDL_Rect orangeSpriteClips[ GEM_SPRITE_TOTAL ];
SDL_Rect purpleSpriteClips[ GEM_SPRITE_TOTAL ];
SDL_Rect redSpriteClips[ GEM_SPRITE_TOTAL ];
SDL_Rect whiteSpriteClips[ GEM_SPRITE_TOTAL ];
SDL_Rect yellowSpriteClips[ GEM_SPRITE_TOTAL ];


