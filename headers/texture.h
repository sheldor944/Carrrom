#ifndef TEXTURE
#define TEXTURE

#include "libHeaders.h"

struct LTexture
{
    LTexture(); //Initializes variables
    ~LTexture(); //Deallocates memory
    bool loadFromFile( std::string path ); //Loads image at specified path
    
    // #if defined(SDL_TTF_MAJOR_VERSION)
    //Creates image from font string
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor ,TTF_Font* gFont );
    // #endif 

    void free(); // Deallocates texture
    void setColor( Uint8 red, Uint8 green, Uint8 blue ); //Set color modulation
    void setBlendMode( SDL_BlendMode blending ); //Set blending
    void setAlpha( Uint8 alpha ); //Set alpha modulation
    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE ); //Renders texture at given point
    
    //Gets image dimensions
    int getWidth();
    int getHeight();

    // The actual texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

#endif