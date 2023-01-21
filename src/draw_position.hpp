#ifndef DRAW_POS_HPP
#define DRAW_POS_HPP

// TODO: hyvä aloitus tälle luokalle, mutta ei toimi vielä kuten halutaan. Vaatii säätöä
class DrawPos
{
public:
    DrawPos(float startX, float startY, float maxX, float minY, float rowGap, float colGap);
    float getX();
    float getY();
    void changeRow(bool upward);
    void updatePos(float dx = -1);
    void resetX();
    void resetY();
private:
    float rowGap, colGap;
    float startX, startY, maxX, minY;
    float currX, currY;
};

#endif