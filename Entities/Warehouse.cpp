#include "Warehouse.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"

Warehouse::Warehouse(Game* r_pGame, point ref, int r_width, int r_height, string img_path)
    : Drawable(r_pGame, ref, r_width, r_height)
{
    image_path = img_path;
}

void Warehouse::draw() const
{
    window* pWind = pGame->getWind();
    pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}
