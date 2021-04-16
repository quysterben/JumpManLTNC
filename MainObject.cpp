#include "MainObject.h"

MainObject::MainObject()
{
    frame_ = 0;
    x_pos_ = SCREEN_WIDTH/2;
    y_pos_ = MAX_DOWN_SCREEN;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    heigth_frame_ = 0;
    dead = false;
}

MainObject::~MainObject()
{
    BaseObject::Free();
}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret == true)
    {
        width_frame_ = rect_.w/11;
        heigth_frame_ = rect_.h;
    }
    return ret;
}

void MainObject::set_clips()
{
    if (width_frame_>0 && heigth_frame_>0)
    {
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = width_frame_;
        frame_clip_[0].h = heigth_frame_;

        frame_clip_[1].x = width_frame_;
        frame_clip_[1].y = 0;
        frame_clip_[1].w = width_frame_;
        frame_clip_[1].h = heigth_frame_;

        frame_clip_[2].x = 2*width_frame_;
        frame_clip_[2].y = 0;
        frame_clip_[2].w = width_frame_;
        frame_clip_[2].h = heigth_frame_;

        frame_clip_[3].x = 3*width_frame_;
        frame_clip_[3].y = 0;
        frame_clip_[3].w = width_frame_;
        frame_clip_[3].h = heigth_frame_;

        frame_clip_[4].x = 4*width_frame_;
        frame_clip_[4].y = 0;
        frame_clip_[4].w = width_frame_;
        frame_clip_[4].h = heigth_frame_;

        frame_clip_[5].x = 5*width_frame_;
        frame_clip_[5].y = 0;
        frame_clip_[5].w = width_frame_;
        frame_clip_[5].h = heigth_frame_;

        frame_clip_[6].x = 6*width_frame_;
        frame_clip_[6].y = 0;
        frame_clip_[6].w = width_frame_;
        frame_clip_[6].h = heigth_frame_;

        frame_clip_[7].x = 7*width_frame_;
        frame_clip_[7].y = 0;
        frame_clip_[7].w = width_frame_;
        frame_clip_[7].h = heigth_frame_;

        frame_clip_[8].x = 8*width_frame_;
        frame_clip_[8].y = 0;
        frame_clip_[8].w = width_frame_;
        frame_clip_[8].h = heigth_frame_;

        frame_clip_[9].x = 9*width_frame_;
        frame_clip_[9].y = 0;
        frame_clip_[9].w = width_frame_;
        frame_clip_[9].h = heigth_frame_;

        frame_clip_[10].x = 10*width_frame_;
        frame_clip_[10].y = 0;
        frame_clip_[10].w = width_frame_;
        frame_clip_[10].h = heigth_frame_;
    }
}

void MainObject::Show(SDL_Renderer* des)
{
    if (y_pos_ == MAX_DOWN_SCREEN)
    {
        LoadImg("res//players//idle.png",des);
        frame_++;
        if  ( frame_ >= 10 )
        {
            frame_ = 0;
        }
    }
    else
    {
        if ( y_val_ < 0 )
        {
            BaseObject::LoadImg("res//players//jump.png",des);
        }
        else
        {
            BaseObject::LoadImg("res//players//fall.png",des);
        }
        frame_ = 0;
    }

    rect_.x = x_pos_;
    rect_.y = y_pos_;

    SDL_Rect* current_clip = &frame_clip_[frame_];

    SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, heigth_frame_};

    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

    SDL_DestroyTexture(p_object_);
}

float MainObject::distanceFromMouse()
{
    int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	return sqrt( pow( ( x_pos_ + width_frame_/2 ) - mouseX, 2) + pow( ( y_pos_ + heigth_frame_/2 ) - mouseY, 2 ) );
}

bool MainObject::jump(SDL_Event events,SDL_Renderer* screen)
{
    if (events.type == SDL_MOUSEBUTTONDOWN)
    {
        if (events.button.button == SDL_BUTTON_LEFT && distanceFromMouse()<100 && y_pos_ == MAX_DOWN_SCREEN)
        {
            y_val_ = -(1/distanceFromMouse() * 130);
            return true;
        }
    }
    return false;
}

void MainObject::Update(bool checkGroundBelow)
{
    x_val_ = 0;
    y_val_ += GRAVITY_FALL;

    if (y_val_ >=  MAX_GRAVITY_FALL)
    {
        y_val_ = MAX_GRAVITY_FALL;
    }

    int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

    if (distanceFromMouse() < 100)
	{
		if (mouseX < x_pos_)
		{
			x_val_ = 1/distanceFromMouse() * 60;
		}
		else if (mouseX > x_pos_ + width_frame_)
		{
			x_val_ = 1/distanceFromMouse() * -60;
		}
		else
		{
			x_val_ = 0;
			if (mouseY > y_pos_ && mouseY < y_pos_ + heigth_frame_)
			{
				dead = true;
			}
		}
	}
	else
	{
		x_val_ = 0;
	}

    MoveAndCheck(checkGroundBelow);
}

void MainObject::MoveAndCheck(bool checkGroundBelow)
{
    if (checkGroundBelow == true)
    {
        y_pos_ += y_val_;
        if (y_pos_ > MAX_DOWN_SCREEN && y_pos_ < MAX_DOWN_SCREEN + 10)
        {
            y_pos_ = MAX_DOWN_SCREEN;
            y_val_ = 0;
        }
    }
    else
    {
        if (y_pos_ > MAX_DOWN_SCREEN )
        {
            y_val_ = 2*MAX_GRAVITY_FALL;
            x_val_ = -2;
        }
            y_pos_ += y_val_;
    }

    if ( y_pos_ > SCREEN_HEIGHT ) dead = true;

    if (x_val_>0)
    {
        if ( ( x_pos_ + x_val_ ) > SCREEN_WIDTH + width_frame_ )
        {
            dead = true;
        }
        else
        {
            x_pos_ += x_val_;
        }
    }
    else if (x_val_<0)
    {
        if ( ( x_pos_+ x_val_ ) < -width_frame_ )
        {
            dead = true;
        }
        else
        {
            x_pos_ += x_val_;
        }
    }
}

void MainObject::reset()
{
    frame_ = 0;
    x_pos_ = SCREEN_WIDTH/3;
    y_pos_ = MAX_DOWN_SCREEN;
    x_val_ = 0;
    y_val_ = 0;
    dead = false;
}
