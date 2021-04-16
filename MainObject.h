#ifndef MAINOBJECT_H_INCLUDED
#define MAINOBJECT_H_INCLUDED

#include "BaseObject.h"

const float GRAVITY_FALL = 0.1;
const float MAX_GRAVITY_FALL = 3.0;
const int MAX_DOWN_SCREEN = SCREEN_HEIGHT - 64 - 32;

class MainObject : BaseObject
{
public:
    MainObject();
    ~MainObject();

    float distanceFromMouse();
    float get_xPos() const {return x_pos_;}
    float get_widthFrame() const {return width_frame_;}
    bool get_Dead() const {return dead;}

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void set_clips();
    void Show(SDL_Renderer* des);

    bool jump(SDL_Event events,SDL_Renderer* screen);
    void Update(bool checkGroundBelow);
    void MoveAndCheck(bool checkGroundBelow);

    void reset();

private:
    float x_val_;
    float y_val_;
    float x_pos_;
    float y_pos_;

    int width_frame_;
    int heigth_frame_;

    SDL_Rect frame_clip_[10];
    int frame_;
    bool dead;
};

#endif // MAINOBJECT_H_INCLUDED
