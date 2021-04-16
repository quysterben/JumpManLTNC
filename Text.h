#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "CommonFunc.h"

class Text
{
public:
    Text();
    ~Text();

    bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
    void Free();

    void SetColor(Uint8 red=0, Uint8 green=0, Uint8 blue=0);

    void RenderText(SDL_Renderer* screen,
                    int xp, int yp,
                    SDL_Rect* clip = NULL,
                    double angle=0.0,
                    SDL_Point* center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    void SetText(const std::string& text) {textContent = text;}
    std::string GetText() const { return textContent; }

private:
    std::string textContent;
    SDL_Color textColor;

    SDL_Texture* texture_;

    int width_;
    int height_;
};

#endif // TEXT_H_INCLUDED
