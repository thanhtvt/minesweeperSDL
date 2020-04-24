#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <string>
#include "Texture.h"
#include "Constances.h"
#include "Variables.h"

using namespace std;

LButton gButtons[ROW_SIZE][COLUMN_SIZE];

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

int main( int argc, char* args[] )
{
    srand(time(NULL));

	//Start up SDL and create window
	if( !init() )
	{
		cout << "Failed to initialize!\n";
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			cout << "Failed to load media!\n";
		}
		else
		{
			//Main loop flag
			bool quit = false;
			bool gameOver = false;

			//Winning flag
			bool isWinning = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
			    //While game is not over yet
			    while ( !gameOver && !quit )
                {
                    //Handle events on queue
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                            quit = true;
                        }
                        //Handle key press
                        else if( e.type == SDL_KEYDOWN )
                        {

                        }
                    }

                    //Clear screen
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    SDL_RenderClear( gRenderer );

                    //Render buttons
                    for( int i = 0; i < ROW_SIZE; i++ )
                    {
                        for ( int j = 0; j < COLUMN_SIZE; j++ )
                        {
                            gButtons[i][j].render(i, j);
                        }
                    }

                    //Update screen
                    SDL_RenderPresent( gRenderer );
                }
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
	    cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{

			cout << "Warning: Linear texture filtering not enabled!";
		}

		//Create window
		gWindow = SDL_CreateWindow( "Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
			    cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
				success = false;
			}
			else
			{
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
				    cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
					success = false;
				}

				 //Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
				    cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if( !gButtonSpriteSheetTexture.loadFromFile( "Image/Tiles.png" ) )
	{
		cout << "Failed to load sprites texture!\n";
		success = false;
	}
	else
    {
        //Set sprites
		for( int i = 0; i < BUTTON_SPRITE_TOTAL; i++ )
		{
			gSpriteClips[ i ].x = i * 32;
			gSpriteClips[ i ].y = 0;
			gSpriteClips[ i ].w = TILE_SIZE;
			gSpriteClips[ i ].h = TILE_SIZE;
		}
		//Set buttons position
		for (int i = 0; i < ROW_SIZE; i++)
        {
            for (int j = 0; j < COLUMN_SIZE; j++)
            {
                gButtons[i][j].setPosition(i * TILE_SIZE, j * TILE_SIZE);
            }
        }
    }

	//Load sound effects
	winner = Mix_LoadWAV( "Sounds/winner.wav" );
	if( winner == NULL )
	{
		cout << "Failed to load winner sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	loser = Mix_LoadWAV( "Sounds/loser.wav" );
	if( loser == NULL )
	{
		cout << "Failed to load loser sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	click = Mix_LoadWAV( "Sounds/click.wav" );
	if( click == NULL )
	{
		cout << "Failed to load click sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gButtonSpriteSheetTexture.free();

	//Free the sound effects
	Mix_FreeChunk( winner );
	Mix_FreeChunk( loser );
	Mix_FreeChunk( click );
	winner = NULL;
	loser = NULL;
	click = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
