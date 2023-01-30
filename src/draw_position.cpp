#include "draw_position.hpp"

DrawPos::DrawPos(float startX, float startY, float maxX, float maxY, float rowGap, float colGap)
{
    this->startX = startX;
    this->startY = startY;

    this->currX = startX;
    this->currY = startY;

    this->rowGap = rowGap;
    this->colGap = colGap;

    this->maxX = maxX;
    this->maxY = maxY;
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

    if (nextPos < this->maxY)
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

bool DrawPos::atMinY()
{
    return this->currY <= 0;
}

bool DrawPos::atMaxY()
{
    return this->currY >= this->maxY;
}

bool DrawPos::atMaxX()
{
    return this->currX >= this->maxX;
}

bool DrawPos::atMinX()
{
    return this->currX <= 0;
}
