#include "Warehouse.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"

Warehouse::Warehouse(Game* r_pGame, point ref, int r_width, int r_height, string img_path)
    : Drawable(r_pGame, ref, r_width, r_height)
{
    image_path = img_path;
    eggCount = 0;      
    milkCount = 0;
}

void Warehouse::draw() const
{
    window* pWind = pGame->getWind();
    pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);

    pWind->SetPen(BLACK);
    pWind->SetFont(16, BOLD, BY_NAME, "Arial");
    
    string eggsText = "Eggs: " + to_string(eggCount) + " | $15 each | Total: $" + to_string(eggCount * 15);
    string milkText = "Milk: " + to_string(milkCount) + " | $10 each | Total: $" + to_string(milkCount * 10);
    
    pWind->DrawString(RefPoint.x, RefPoint.y + height + 5, eggsText);
    pWind->DrawString(RefPoint.x, RefPoint.y + height + 25, milkText);
}
void Warehouse::incrementEggs()
{
    eggCount++;
}

void Warehouse::incrementMilk()
{
    milkCount++;
}

int Warehouse::getEggCount() const
{
    return eggCount;
}

int Warehouse::getMilkCount() const
{
    return milkCount;
}
