#ifndef DRAW_POS_HPP
#define DRAW_POS_HPP

class DrawPos
{
public:
    DrawPos(float startX, float startY, float maxX, float maxY, float rowGap, float colGap);
    float getX();
    float getY();
    void changeRow(bool upward);
    void updatePos(float dx = -1);
    void resetX();
    void resetY();
    bool atMaxY();
    bool atMinY();
    bool atMaxX();
    bool atMinX();
private:
    float rowGap, colGap;
    float startX, startY, maxX, maxY;
    float currX, currY;
};

#endif