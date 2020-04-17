#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cstring>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <sstream>
#include "Constance.h"
#include "Variables.h"
#include "Gem.h"
#include "LTexture.h"

using namespace std;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
	    cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << endl;
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText( string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( timeFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
	}

	//Return success
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

Gem::Gem()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = GEM_SPRITE_MOUSE_OUT;
}

void Gem::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

SDL_Point Gem::getPosition()
{
    return mPosition;
}

void Gem::setPressed(bool isPressed)
{
    pressed = isPressed;
}

void Gem::handleEvent( SDL_Event* e, Gem pressedGem[2] )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in gem
		bool inside = true;

		//Mouse is left of the gem
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the gem
		else if( x > mPosition.x + GEM_WIDTH )
		{
			inside = false;
		}
		//Mouse above the gem
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the gem
		else if( y > mPosition.y + GEM_HEIGHT )
		{
			inside = false;
		}

		//Mouse is outside gem
		if( !inside )
		{
			mCurrentSprite = GEM_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside gem
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = GEM_SPRITE_MOUSE_OVER_MOTION;
				break;

				case SDL_MOUSEBUTTONDOWN:
                {
                    //Plays "pressed" sound effect
                    Mix_PlayChannel( -1, click, 0 );
                    mCurrentSprite = GEM_SPRITE_MOUSE_DOWN;

                    //If gem is pressed before
                    if (pressed)
                    {
                        //Removes pressed flag
                        pressedGem[0].setPressed(false);
                        pressedCount--;
                        break;
                    }
                    else
                    {
                        //Sets pressed flag
                        pressedGem[pressedCount].setPressed(true);
                        pressedCount++;
                        break;
                    }
                }
			}
		}
	}
}

void Gem::setType(int typeOfGem)
{
	type = typeOfGem;
}

int Gem::getType()
{
	return type;
}

void Gem::setRemoved (bool isRemoved)
{
    removed = isRemoved;
}

bool Gem::getRemoved()
{
    return removed;
}

void Gem::render()
{
	//Show current button sprite
	switch (type)
	{
    case BLUE_GEM:
        blueSpriteSheetTexture.render(mPosition.x, mPosition.y, &blueSpriteClips[ mCurrentSprite ] );
        break;
    case GREEN_GEM:
        greenSpriteSheetTexture.render(mPosition.x, mPosition.y, &greenSpriteClips[ mCurrentSprite ] );
        break;
    case ORANGE_GEM:
        orangeSpriteSheetTexture.render(mPosition.x, mPosition.y, &orangeSpriteClips[ mCurrentSprite ] );
        break;
    case PURPLE_GEM:
        purpleSpriteSheetTexture.render(mPosition.x, mPosition.y, &purpleSpriteClips[ mCurrentSprite ] );
        break;
    case RED_GEM:
        redSpriteSheetTexture.render(mPosition.x, mPosition.y, &redSpriteClips[ mCurrentSprite ] );
        break;
    case WHITE_GEM:
        whiteSpriteSheetTexture.render(mPosition.x, mPosition.y, &whiteSpriteClips[ mCurrentSprite ] );
        break;
    case YELLOW_GEM:
        yellowSpriteSheetTexture.render(mPosition.x, mPosition.y, &yellowSpriteClips[ mCurrentSprite ] );
        break;
	}
}

bool Gem::checkAdjacent(Gem gem0, Gem gem1)
{
    if (((gem1.getPosition().x == gem0.getPosition().x - DISTANCE_BETWEEN) && (gem1.getPosition().y == gem0.getPosition().y)) ||
		((gem1.getPosition().x == gem0.getPosition().x + DISTANCE_BETWEEN) && (gem1.getPosition().y == gem0.getPosition().y)) ||
		((gem1.getPosition().y == gem0.getPosition().y - DISTANCE_BETWEEN) && (gem1.getPosition().x == gem0.getPosition().x)) ||
		((gem1.getPosition().y == gem0.getPosition().y + DISTANCE_BETWEEN) && (gem1.getPosition().x == gem0.getPosition().x)))
    {
		return true;
	}
	else
    {
		return false;
	}
}

bool Gem::checkSequence(Gem gem[TOTAL_GEMS])
{
    bool isSequence = false;
    int countSequence = 0;
    int typeOfGem;
    int position;

    //Checks in column
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COLUMN_SIZE; j++)
        {
            int a = i * 8 + j;
            for (int k = 0; k <= 56; k += 8)    //56 is the minus of bottom to top point (calculate in position)
            {
                typeOfGem = gem[a].getType();
                if (gem[a].getType() == typeOfGem)
                {
                    countSequence++;
                    position = j;
                }
                else
                {
                    if (countSequence >= MIN_SEQUENCE_LENGTH)
                    {
                        isSequence = true;
                        removeSequence(i, position, 0, countSequence, gem);
                    }
                    else isSequence = false;
                    countSequence = 0;
                    a += k;
                }
            }
        }
    }

    int temp = 0;
    countSequence = 0;
    typeOfGem = gem[0].getType();

    //Checks in row
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COLUMN_SIZE; j++)
        {
            int a = i * 8 + j;
            if (temp < i)
            {
                countSequence = 0;
                temp = i;
            }
            if (gem[a].getType() == typeOfGem)
            {
                countSequence++;
            }
            else
            {
                if (countSequence >= MIN_SEQUENCE_LENGTH)
                {
                    isSequence = true;
                    removeSequence(position, j, 1, countSequence, gem);
                    //code goes here
                }
                else isSequence = false;
                countSequence = 0;
                typeOfGem = gem[a].getType();
            }
        }
    }
    return isSequence;
}

void Gem::swapGems(Gem pressedGem[2])
{
    if ( checkAdjacent(pressedGem[0], pressedGem[1]) )
    {
        //Receives gems' information
        SDL_Point gem0Position = { pressedGem[0].getPosition().x, pressedGem[0].getPosition().y };
        SDL_Point gem1Position = { pressedGem[1].getPosition().x, pressedGem[1].getPosition().y };
        int gem0type = gem[0].getType();
        int gem1Type = gem[1].getType();

        //Swaps gems' type
        pressedGem[0].setType( gem1Type );
        pressedGem[1].setType( gem0type );

        //Swaps gems' position
        pressedGem[0].setPosition( gem1Position.x, gem1Position.y );
        pressedGem[1].setPosition( gem0Position.x, gem0Position.y );
    }
}

void Gem::removeSequence (int x, int y, int direction, int length, Gem gem[TOTAL_GEMS])
{
    if (direction == ROW)
    {
        for (int i = x; i > x - length; i--)
        {
            gem[i * 8 + y].setRemoved(true);
        }
    }
    else
    {
        for (int i = y; i > y - length; i--)
        {
            gem[x * 8 + i].setRemoved(true);
        }
    }
}

void Gem::createNewGems(Gem gem[TOTAL_GEMS])
{
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COLUMN_SIZE; j++)
        {
            if (gem[i * 8 + j].getRemoved())
            {
                gem[i * 8 + j].setType(rand() % 7);
                gem[i * 8 + j].setRemoved(false);
            }
        }
    }
}

void Gem::dropGems()
{

}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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
		gWindow = SDL_CreateWindow( "Bejeweled", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
					success = false;
				}

                //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
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

	//Load background texture
	if( !gPromptTexture.loadFromFile( "Images/Background.png" ) )
	{
		cout << "Failed to load prompt texture!\n";
		success = false;
	}

	//Load blue gem texture
	if ( !blueSpriteSheetTexture.loadFromFile( "Images/Blue.png" ) )
    {
        cout << "Failed to load blue gem texture!\n";
        success = false;
    }
    else
    {
        //Set sprites
		for (int i = 0; i < 2; ++i)
		{
			blueSpriteClips[i].x = 0;
			blueSpriteClips[i].y = i * 55;
			blueSpriteClips[i].w = GEM_WIDTH;
			blueSpriteClips[i].h = GEM_HEIGHT;
		}
    }

    //Load green gem texture
	if ( !greenSpriteSheetTexture.loadFromFile( "Images/Green.png" ) )
    {
        cout << "Failed to load green gem texture!\n";
        success = false;
    }
    else
    {
        //Set sprites
		for (int i = 0; i < 2; ++i)
		{
			greenSpriteClips[i].x = 0;
			greenSpriteClips[i].y = i * 55;
			greenSpriteClips[i].w = GEM_WIDTH;
			greenSpriteClips[i].h = GEM_HEIGHT;
		}
    }

    //Load orange orange texture
	if ( !orangeSpriteSheetTexture.loadFromFile( "Images/Orange.png" ) )
    {
        cout << "Failed to load orange gem texture!\n";
        success = false;
    }
    else
    {
        //Set sprites
		for (int i = 0; i < 2; ++i)
		{
			orangeSpriteClips[i].x = 0;
			orangeSpriteClips[i].y = i * 55;
			orangeSpriteClips[i].w = GEM_WIDTH;
			orangeSpriteClips[i].h = GEM_HEIGHT;
		}
    }

    //Load purple gem texture
	if ( !purpleSpriteSheetTexture.loadFromFile( "Images/Purple.png" ) )
    {
        cout << "Failed to load purple gem texture!\n";
        success = false;
    }
    else
    {
        //Set sprites
		for (int i = 0; i < 2; ++i)
		{
			purpleSpriteClips[i].x = 0;
			purpleSpriteClips[i].y = i * 55;
			purpleSpriteClips[i].w = GEM_WIDTH;
			purpleSpriteClips[i].h = GEM_HEIGHT;
		}
    }

    //Load red gem texture
	if ( !redSpriteSheetTexture.loadFromFile( "Images/Red.png" ) )
    {
        cout << "Failed to load red gem texture!\n";
        success = false;
    }
    else
    {
        //Set sprites
		for (int i = 0; i < 2; ++i)
		{
			redSpriteClips[i].x = 0;
			redSpriteClips[i].y = i * 55;
			redSpriteClips[i].w = GEM_WIDTH;
			redSpriteClips[i].h = GEM_HEIGHT;
		}
    }

    //Load white gem texture
	if ( !whiteSpriteSheetTexture.loadFromFile( "Images/White.png" ) )
    {
        cout << "Failed to load white gem texture!\n";
        success = false;
    }
    else
    {
        //Set sprites
		for (int i = 0; i < 2; ++i)
		{
			whiteSpriteClips[i].x = 0;
			whiteSpriteClips[i].y = i * 55;
			whiteSpriteClips[i].w = GEM_WIDTH;
			whiteSpriteClips[i].h = GEM_HEIGHT;
		}
    }

    //Load yellow gem texture
	if ( !yellowSpriteSheetTexture.loadFromFile( "Images/Yellow.png" ) )
    {
        cout << "Failed to load yellow gem texture!\n";
        success = false;
    }
    else
    {
        //Set sprites
		for (int i = 0; i < 2; ++i)
		{
			yellowSpriteClips[i].x = 0;
			yellowSpriteClips[i].y = i * 55;
			yellowSpriteClips[i].w = GEM_WIDTH;
			yellowSpriteClips[i].h = GEM_HEIGHT;
		}
    }

	//Open the font
	timeFont = TTF_OpenFont( "Fonts/BusoramaMediumBT.ttf", 20 );
	if( timeFont == NULL )
	{
	    cout << "Failed to load BusoramaMediumBT font! SDL_ttf Error: " << TTF_GetError() << endl;
		success = false;
	}
	TTF_SetFontStyle(timeFont, TTF_STYLE_BOLD);

	//Load music
	gMusic = Mix_LoadMUS( "Sounds/backMus.mp3" );
	if( gMusic == NULL )
	{
		cout << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	//Load sound effects
	bad = Mix_LoadWAV( "Sounds/bad.wav" );
	if( bad == NULL )
	{
		cout << "Failed to load bad sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	nice = Mix_LoadWAV( "Sounds/nice.wav" );
	if( nice == NULL )
	{
		cout << "Failed to load nice sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

    click = Mix_LoadWAV( "Sounds/click.wav" );
	if( click == NULL )
	{
		cout << "Failed to load click sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	combo2 = Mix_LoadWAV( "Sounds/combo2.wav" );
	if( combo2 == NULL )
	{
		cout << "Failed to load combo2 sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	combo3 = Mix_LoadWAV( "Sounds/combo3.wav" );
	if( combo3 == NULL )
	{
		cout << "Failed to load combo3 sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	combo4 = Mix_LoadWAV( "Sounds/combo4.wav" );
	if( combo4 == NULL )
	{
		cout << "Failed to load combo4 sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	combo5 = Mix_LoadWAV( "Sounds/combo5.wav" );
	if( combo5 == NULL )
	{
		cout << "Failed to load combo5 sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

	combo6 = Mix_LoadWAV( "Sounds/combo6.wav" );
	if( combo6 == NULL )
	{
		cout << "Failed to load combo6 sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		success = false;
	}

    srand(time(NULL));

	for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COLUMN_SIZE; j++)
        {
            int a = i * 8 + j;
            gem[a].setType(rand() % 7);
            while ((gem[a].getType() == gem[a-1].getType() && gem[a].getType() == gem[a-2].getType()) ||
                   (gem[a].getType() == gem[a-8].getType() && gem[a].getType() == gem[a-16].getType()))
            {
                gem[a].setType(rand() % 7);
            }
            gem[a].setPosition(j * DISTANCE_BETWEEN + GRID_X, i * DISTANCE_BETWEEN + GRID_Y);
            gem[a].x1 = gem[a].getPosition().x;
            gem[a].y1 = gem[a].getPosition().y;
        }
	}

	return success;
}

void close()
{
	//Free loaded images
	gTimeTextTexture.free();

	//Free global font
	TTF_CloseFont( timeFont );
	timeFont = NULL;

	//Free the sound effects
	Mix_FreeChunk( nice );
	Mix_FreeChunk( bad );
	Mix_FreeChunk( click );
	Mix_FreeChunk( combo2 );
	Mix_FreeChunk( combo3 );
	Mix_FreeChunk( combo4 );
	Mix_FreeChunk( combo5 );
	Mix_FreeChunk( combo6 );
	nice = NULL;
	bad = NULL;
	click = NULL;
	combo2 = NULL;
	combo3 = NULL;
	combo4 = NULL;
	combo5 = NULL;
	combo6 = NULL;

	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
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

			//Event handler
			SDL_Event e;

			//Set text color as white
			SDL_Color textColor = { 255, 255, 255, 255 };

			//In memory text stream
			stringstream timeText;
			stringstream scoreText;

			//While application is running
			while( !quit )
			{
			    //Play background music
			    //Mix_PlayMusic( gMusic, -1 );

				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				Uint32 startTime = SDL_GetTicks();

                timeText.str("");
				timeText << MAX_TIME_PLAY - SDL_GetTicks()/1000;

				if (MAX_TIME_PLAY - startTime/1000 == 0) quit = true; //Set temporary

				//Render text
				if( !gTimeTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
				{
					cout << "Unable to render time texture!\n";
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

                //Render background
                gPromptTexture.render( 0, 0 );

                //Render gems
                for (int i = 0; i < TOTAL_GEMS; i++)
                {
                    gem[i].render();
                }

                //Render textures
				gTimeTextTexture.render( 110, 100 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
