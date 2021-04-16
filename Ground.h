#ifndef GROUND_H_INCLUDED
#define GROUND_H_INCLUDED

#include "CommonFunc.h"
#include "BaseObject.h"

const int StartGroundNums = 13;
const int BlockSpeed = 2;
const int GroundSpawnY = SCREEN_HEIGHT - 64;
const int GroundSize = 64;
const int Hole = 0;
const int Center = 1;
const int Left = 2;
const int Right = 3;

class GroundTile : public BaseObject
{
public:
    GroundTile();
    ~GroundTile();

    int get_Left() const { return this->rect_.x; }
    int get_Top() const {return this->rect_.y;}
    int get_Right() const {return ( this->rect_.x + this->rect_.w );}

    double get_size() const {return this->rect_.w;}

    float get_x_val() const {return x_val_;}
    void set_x_val(const float& xVal) {x_val_ = xVal;}

    void set_is_move(const bool& isMove) {is_move_ = isMove;}
    bool get_is_move() {return is_move_;}

    int get_status() const {return status;}
    void set_status(const int& _status) { status = _status; }

    void HandleMove();
private:
    float x_val_;
    bool is_move_;

    int status;
};

class Ground : public BaseObject
{
public:
    Ground();
    ~Ground();

    void MakeStartGround(SDL_Renderer* screen);
    void MakeGround(SDL_Renderer* screen);

    void set_ground_list(std::vector<GroundTile*> ground_list)
    {
        groundTile_list_ = ground_list;
    }
    std::vector<GroundTile*> get_groundTile_list() const {return groundTile_list_;}

    void HandleGround(SDL_Renderer* des);

    void SetupGround(std::string GroundPath, int GroundSpawnX,
                     int GroundStatus, SDL_Renderer* screen);

    bool isGroundBelow (float x_pos_, int width);

    int getPoint () const { return point; }

    void reset();

private:
    std::vector<GroundTile*> groundTile_list_;
    int point;
};

#endif // GROUND_H_INCLUDED
