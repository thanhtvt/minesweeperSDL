#include <cstring>
using namespace std;

class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( string path );

		//Creates image from font string
		bool loadFromRenderedText( string textureText, SDL_Color textColor );

		//Deallocates texture
		void free();

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//Scene textures
LTexture gTimeTextTexture;
LTexture gScoreTextTexture;
LTexture gPromptTexture;

LTexture blueSpriteSheetTexture;
LTexture greenSpriteSheetTexture;
LTexture orangeSpriteSheetTexture;
LTexture purpleSpriteSheetTexture;
LTexture redSpriteSheetTexture;
LTexture whiteSpriteSheetTexture;
LTexture yellowSpriteSheetTexture;

