#include "FoodArea.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"

FoodArea::FoodArea(Game* r_pGame, point ref, int r_radius)
    : Drawable(r_pGame, ref, r_radius * 2, r_radius * 2)
{
    radius = r_radius;
    counter = 10; 
}

void FoodArea::draw() const
{
    window* pWind = pGame->getWind();
    if (counter <= 0)   
        return;
    pWind->SetPen(DARKGREEN, 2);
    pWind->SetBrush(GREEN);
    pWind->DrawEllipse(
        RefPoint.x - radius,
        RefPoint.y - radius,
        RefPoint.x + radius,
        RefPoint.y + radius
    );
    pWind->SetPen(WHITE, 1);
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
}
