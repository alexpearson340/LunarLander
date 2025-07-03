#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

// Custom deleters for SDL resources
struct SDLTextureDeleter
{
    void operator()(SDL_Texture* texture)
    {
        SDL_DestroyTexture(texture);
    }
};

// Texture wrapper class
class Texture
{
public:
    Texture();

    Texture(SDL_Renderer*);

    Texture(SDL_Renderer*, TTF_Font*);

    ~Texture();

    int getWidth() const { return mWidth; };
    int getHeight() const { return mHeight; };

    // Set Texture to the passed value
    void setTexture(SDL_Texture *, const int, const int);

    // Loads image at specified path
    bool loadFromFile(const std::string& path);

    // Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Color backgroundColour);

    // Have SDL draw directly to the encapsulated Texture
    void setAsRenderingTarget();

    // Deallocates texture
    void reset();

    // Renders texture at given point
    void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
    // The actual hardware texture
    std::unique_ptr<SDL_Texture, SDLTextureDeleter> mTexture;
    SDL_Renderer* mRenderer;
    TTF_Font* mFont;

    // Image dimensions
    int mWidth;
    int mHeight;
};

#endif