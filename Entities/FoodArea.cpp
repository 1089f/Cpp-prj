#include "FoodArea.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"

FoodArea::FoodArea(Game* r_pGame, point ref, int r_radius, string img_path)
    : Drawable(r_pGame, ref, r_radius * 2, r_radius * 2)
{
    radius = r_radius;
    counter = 10;
    image_path = img_path;
}
void FoodArea::draw() const
{
    if (counter <= 0)
        return;

    window* pWind = pGame->getWind();
    pWind->DrawImage(image_path, RefPoint.x - radius, RefPoint.y - radius, width, height);

    
    pWind->SetPen(BLACK, 1);
    pWind->SetFont(16, BOLD, BY_NAME, "Arial");
    pWind->DrawString(RefPoint.x - 5, RefPoint.y - 10, to_string(counter));
}

void FoodArea::decreaseCounter()
{
    if (counter > 0)
        counter--;
}

bool FoodArea::isEmpty() const
{
    return counter <= 0;
}

int FoodArea::getCounter() const
{
    return counter;
}
void FoodArea::setCounter(int c)
{
    counter = c;
}

