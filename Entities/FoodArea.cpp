#include "FoodArea.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"

FoodArea::FoodArea(Game* r_pGame, point ref, int r_radius)
    : Drawable(r_pGame, ref, r_radius * 2, r_radius * 2)
{
    radius = r_radius;
}

void FoodArea::draw() const
{
    window* pWind = pGame->getWind();
    pWind->SetPen(DARKGREEN, 2);
    pWind->SetBrush(GREEN);
    pWind->DrawEllipse(
        RefPoint.x - radius,
        RefPoint.y - radius,
        RefPoint.x + radius,
        RefPoint.y + radius
    );
}
