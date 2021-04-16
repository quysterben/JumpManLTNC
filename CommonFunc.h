#ifndef COMMONFUNC_H_INCLUDED
#define COMMONFUNC_H_INCLUDED

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <fstream>

const int FRAME_PER_SECOND = 55;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
const std::string WINDOW_TITLE = "Jump Man!";

namespace SDLCommonFunc{
    int getBestPoint();
    void fixBestPoint(int newPoint);
}

#endif // COMMONFUNC_H_INCLUDED
