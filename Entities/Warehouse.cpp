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
bool Warehouse::containsPoint(int x, int y) const
{
    return (x >= RefPoint.x &&
        x <= RefPoint.x + width &&
        y >= RefPoint.y &&
        y <= RefPoint.y + height);
}
void Warehouse::openWindow() const
{
    window* warehouseWindow = new window(400, 300, 200, 200);

    warehouseWindow->SetBrush(WHITE);
    warehouseWindow->SetPen(BLACK, 2);

    warehouseWindow->DrawRectangle(0, 0, 400, 300);

    warehouseWindow->SetFont(22, BOLD, BY_NAME, "Arial");

    warehouseWindow->DrawString(120, 30, "WAREHOUSE");

    string eggsText =
        "Eggs Stored: " + to_string(eggCount);

    string milkText =
        "Milk Stored: " + to_string(milkCount);

    string totalText =
        "Total Value: $" +
        to_string((eggCount * 15) + (milkCount * 10));

    warehouseWindow->DrawString(50, 100, eggsText);
    warehouseWindow->DrawString(250, 100, "SELL EGG");

    warehouseWindow->DrawString(50, 150, milkText);
    warehouseWindow->DrawString(250, 150, "SELL MILK");
    warehouseWindow->DrawString(50, 200, totalText);

    // EXIT BUTTON
    warehouseWindow->SetPen(RED, 3);
    warehouseWindow->DrawRectangle(140, 240, 260, 280);

    warehouseWindow->SetFont(20, BOLD, BY_NAME, "Arial");
    warehouseWindow->DrawString(175, 250, "EXIT");

    int x, y;

    while (true)
    {
        warehouseWindow->WaitMouseClick(x, y);

        // EXIT button
        if (x >= 140 && x <= 260 && y >= 240 && y <= 280)
            break;

        // SELL EGG button
        if (x >= 250 && x <= 350 && y >= 90 && y <= 120)
        {
            const_cast<Warehouse*>(this)->sellEgg(pGame);
            break;
        }

        // SELL MILK button
        if (x >= 250 && x <= 350 && y >= 140 && y <= 170)
        {
            const_cast<Warehouse*>(this)->sellMilk(pGame);
            break;
        }
    }

    delete warehouseWindow;
}
void Warehouse::reset()
{
    eggCount = 0;
    milkCount = 0;
}
void Warehouse::sellEgg(Game* game)
{
    if (eggCount > 0)
    {
        eggCount--;
        game->addBudget(15);   
    }
}
void Warehouse::sellMilk(Game* game)
{
    if (milkCount > 0)
    {
        milkCount--;
        game->addBudget(10);
    }
}
