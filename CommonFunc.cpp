#include "CommonFunc.h"

int SDLCommonFunc::getBestPoint()
{
    std::ifstream myFile("res//yourbestscore.txt");
    if(myFile.is_open())
    {
        int tempPoint;
        myFile >> tempPoint;
        return tempPoint;
    }
    else std::cout << "Can't open file at getBestPoint()" << std::endl;
}

void SDLCommonFunc::fixBestPoint(int newPoint)
{
    std::ofstream myFile("res//yourbestscore.txt", std::ofstream::trunc);
    if(myFile.is_open())
    {
        myFile << newPoint;
    }
    else std::cout << "Can't open file at getBestPoint()" << std::endl;
}
