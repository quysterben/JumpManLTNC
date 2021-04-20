#include "Ground.h"

GroundTile::GroundTile()
{
    is_move_ = false;
    x_val_ = 0;
    status = 1;
}

GroundTile::~GroundTile()
{

}

void GroundTile::HandleMove()
{
    rect_.x -= x_val_;
    if (this->get_Right() < -GroundSize)
        is_move_ = false;
}

//Ground
Ground::Ground()
{
    point = 0;
}

Ground::~Ground()
{

}

void Ground::SetupGround(std::string GroundPath, int GroundSpawnX, int GroundStatus, SDL_Renderer* screen)
{
    GroundTile* TempGround = new GroundTile();
    TempGround->BaseObject::LoadImg(GroundPath, screen);
    TempGround->SetRect(GroundSpawnX, GroundSpawnY);
    TempGround->set_x_val(BlockSpeed);
    TempGround->set_is_move(true);
    TempGround->set_status(GroundStatus);
    groundTile_list_.push_back(TempGround);
}


void Ground::MakeGround(SDL_Renderer* screen)
{
    int myRandomNormalGround = rand() % 3;
    int myRandomHoleGround = rand() % 4;

    for (int i=0; i < myRandomNormalGround; i++)
    {
        SetupGround("res//ground//center.png", SCREEN_WIDTH, Center, screen);
    }

    if (myRandomHoleGround!=0)
    {
        SetupGround("res//ground//right.png", SCREEN_WIDTH, Right, screen);
        for (int i=0; i < myRandomHoleGround; i++)
        {
            SetupGround("res//ground//hole.png", SCREEN_WIDTH, Hole, screen);
        }
        SetupGround("res//ground//left.png", SCREEN_WIDTH, Left, screen);
    }
}

void Ground::HandleGround(SDL_Renderer* des)
{
    for (int i=0; i < groundTile_list_.size(); ++i)
    {
        GroundTile* groundTile = groundTile_list_.at(i);
        if (groundTile != NULL)
        {
            if( groundTile->get_is_move() == true)
            {
                if (i==0)
                {
                    groundTile->HandleMove();
                    groundTile->Render(des);
                }
                else
                {
                    GroundTile* lastGroundTile = groundTile_list_.at(i-1);
                    if (lastGroundTile->get_Right()<=SCREEN_WIDTH)
                    {
                        groundTile->HandleMove();
                        groundTile->Render(des);
                    }
                }
            }
            else
            {
                groundTile_list_.erase(groundTile_list_.begin()+i);
                delete groundTile;
                groundTile = NULL;
                point++;
            }
        }
    }
}

bool Ground::isGroundBelow (float x_pos_, int width)
{
    for (int i=0; i<groundTile_list_.size(); i++)
    {
        GroundTile* groundTile = groundTile_list_.at(i);
        switch (groundTile->get_status())
        {
        case Center:
		if ( (x_pos_ + width > groundTile->get_Left()) && (x_pos_ < groundTile->get_Right()) )
		{
			return true;
		}
		break;
	case Left:
		if ( (x_pos_ + width > groundTile->get_Left() + 30) && (x_pos_ < groundTile->get_Right()) )
		{
			return true;
		}
		break;
	case Right:
		if ( (x_pos_ + width > groundTile->get_Left()) && (x_pos_ < groundTile->get_Right()-30) )
		{
			return true;
		}
		break;
        }
    }
    return false;
}

void Ground::reset()
{
    for (int i=0; i<groundTile_list_.size(); i++)
    {
        GroundTile* groundTile = groundTile_list_.at(i);
        delete groundTile;
        groundTile = NULL;
    }
    groundTile_list_.clear();
    point = 0;
}

void Ground::MakeStartGround(SDL_Renderer* screen)
{
    for (int i=0; i < StartGroundNums; i++)
    {
        SetupGround("res//ground//center.png", i*GroundSize, Center, screen);
    }
}
