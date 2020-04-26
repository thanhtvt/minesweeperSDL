#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "Constances.h"
#include "Button.h"

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

//The sound effects that will be used
extern Mix_Music *winner;
extern Mix_Music *loser;
extern Mix_Chunk *click;

//Mouse button texture
extern SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
extern LTexture gButtonSpriteSheetTexture;

//Screen texture
extern LTexture gWinningTexture;
extern LTexture gBackgroundTexture;

//Buttons objects
//extern LButton gButtons[ROW_SIZE][COLUMN_SIZE];

//Gameplay variables
extern bool isRevealed;
extern int countMineLeft;
extern int countTileLeft;
extern bool gameOver;
extern bool isWinning;

//Board with mine
extern int board[ROW_SIZE][COLUMN_SIZE];

//Board for showing
extern int sBoard[ROW_SIZE][COLUMN_SIZE];





