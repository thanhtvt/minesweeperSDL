#include <ctime>
#include "Button.h"
#include "Variables.h"

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + TILE_SIZE )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + TILE_SIZE )
		{
			inside = false;
		}

		//Mouse is inside button
		if ( inside )
		{
			if ( e->type == SDL_MOUSEBUTTONDOWN )
            {
                //Play the sound effect
				Mix_PlayChannel(-1, click, 0);
            }
		}
	}
}

void LButton::render(int i, int j)
{
	//Show current button sprite
	gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ sBoard[i][j] ] );
}
