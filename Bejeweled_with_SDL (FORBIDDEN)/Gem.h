#pragma once

#include <SDL.h>
#include "Constance.h"

enum LGemSprite
{
	GEM_SPRITE_MOUSE_OUT = 0,
	GEM_SPRITE_MOUSE_OVER_MOTION = 0,
	GEM_SPRITE_MOUSE_DOWN = 1,
	GEM_SPRITE_TOTAL = 2
};

enum Direction
{
    COLUMN = 0,
    ROW = 1
};

enum GemType
{
    BLUE_GEM = 0,
    GREEN_GEM = 1,
    ORANGE_GEM = 2,
    PURPLE_GEM = 3,
    RED_GEM = 4,
    WHITE_GEM = 5,
    YELLOW_GEM = 6,
};

class Gem
{
	public:
		//Initializes internal variables
		Gem();

		//Sets gem position
		void setPosition( int x, int y );

		//Gets gem position
		SDL_Point getPosition();

		//Sets gem pressed
		void setPressed( bool isPressed );

		//Checks if gems are adjacent
		bool checkAdjacent (Gem gem0, Gem gem1);

		//Swaps gems
		void swapGems( Gem pressedGem[2] );

		//Removes correct swapped gems
		void removeSequence (int x, int y, int direction, int length, Gem gem[TOTAL_GEMS]);

		//Checks if swapped gems create a sequence
		bool checkSequence ( Gem gem[TOTAL_GEMS] );

		//Drops new gems
		void dropGems();

		//Create new gems to replace empty slots
		void createNewGems (Gem gem[TOTAL_GEMS]);

		//Handles mouse event
		void handleEvent( SDL_Event* e, Gem pressedGem[2] );

		//Returns the gem type
        int getType();

        //Sets the gem type
        void setType(int typeOfGem);

        //Sets removed flag
        void setRemoved (bool isRemoved);

        //Gets removed flag
        bool getRemoved();

        //first position of gem
        int x1;
        int y1;

        //second position of gem
        int x2;
        int y2;

		//Shows gem sprite
		void render();

	private:
		//Top left position
		SDL_Point mPosition;

		//Type of gems
		int type;

        //Pressed flag
		bool pressed;

		//Removed flag
		bool removed;

		//Currently used global sprite
		LGemSprite mCurrentSprite;
};

//Total gems
Gem gem[TOTAL_GEMS];

//Counts number of gems being pressed
int pressedCount = 0;
