#include "Variables.h"
#include "Constances.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//The sound effects that will be used
Mix_Music *winner = NULL;
Mix_Music *loser = NULL;
Mix_Chunk *click = NULL;

//Mouse button texture
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
LTexture gButtonSpriteSheetTexture;

//Screen texture
LTexture gWinningTexture;
LTexture gBackgroundTexture;

//Buttons objects
//LButton gButtons[ROW_SIZE][COLUMN_SIZE];

//Gameplay variables
bool isRevealed;
int countMineLeft = MINE_COUNT;
int countTileLeft = ROW_SIZE * COLUMN_SIZE;
bool gameOver = false;
bool isWinning = false;

//Board with mine
int board[ROW_SIZE][COLUMN_SIZE];

//Board for showing
int sBoard[ROW_SIZE][COLUMN_SIZE];
