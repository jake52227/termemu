#include "draw_position.hpp"

DrawPos::DrawPos(float startX, float startY, float maxX, float minY, float rowGap, float colGap)
{
    this->startX = startX;
    this->startY = startY;

    this->currX = startX;
    this->currY = startY;

    this->rowGap = rowGap;
    this->colGap = colGap;

    this->maxX = maxX;
    this->minY = minY;
}

void DrawPos::updatePos(float dx)
{
    if (dx == -1)
        dx = this->colGap;
    float rightPos = this->currX + dx;
    if (rightPos >= this->maxX)
    {
        this->changeRow(false);
    }
    else
    {
        this->currX = rightPos;
    }
}

void DrawPos::changeRow(bool upward)
{
    this->currX = this->startX;
    float nextPos = upward ? this->currY + this->rowGap : this->currY - this->rowGap;

    if (nextPos > this->minY)
        this->currY = nextPos;
}

float DrawPos::getX()
{
    return this->currX;
}

float DrawPos::getY()
{
    return this->currY;
}

void DrawPos::resetX()
{

    this->currX = this->startX;
}

void DrawPos::resetY()
{

    this->currY = this->startY;
}