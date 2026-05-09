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
void Warehouse::sellEggs()
{
    if (eggCount > 0)
    {
        pGame->addBudget(eggCount * 15);
        eggCount = 0;
    }
}

void Warehouse::sellMilk()
{
    if (milkCount > 0)
    {
        pGame->addBudget(milkCount * 10);
        milkCount = 0;
    }
}

void Warehouse::openSellWindow()
{
    window* sellWind = new window(400, 300, 400, 200);
    sellWind->SetBrush(WHITE);
    sellWind->SetPen(BLACK, 2);
    sellWind->DrawRectangle(0, 0, 400, 300);

    sellWind->SetFont(18, BOLD, BY_NAME, "Arial");
    sellWind->SetPen(BLACK, 1);
    sellWind->DrawString(20, 30, "Eggs: " + to_string(eggCount) + " x $15 = $" + to_string(eggCount * 15));

    sellWind->SetBrush(LIGHTGREEN);
    sellWind->DrawRectangle(300, 20, 380, 50);
    sellWind->DrawString(310, 28, "SELL");

    sellWind->DrawString(20, 100, "Milk: " + to_string(milkCount) + " x $10 = $" + to_string(milkCount * 10));

    sellWind->SetBrush(LIGHTGREEN);
    sellWind->DrawRectangle(300, 90, 380, 120);
    sellWind->DrawString(310, 98, "SELL");

    int x, y;
    sellWind->WaitMouseClick(x, y);

    if (x >= 300 && x <= 380 && y >= 20 && y <= 50)
    {
        sellEggs();
        pGame->printMessage("Sold eggs! Budget: $" + to_string(pGame->budget));
    }

    if (x >= 300 && x <= 380 && y >= 90 && y <= 120)
    {
        sellMilk();
        pGame->printMessage("Sold milk! Budget: $" + to_string(pGame->budget));
    }

    delete sellWind;
}
