#include "Budgetbar.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include "../Entities/FoodArea.h"
#include <iostream>
using namespace std;

BudgetbarIcon::BudgetbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    image_path = img_path;
}

void BudgetbarIcon::draw() const
{
    window* pWind = pGame->getWind();
    pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

ChickIcon::ChickIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
    chickList = new Chick * [50];
    for (int i = 0; i < 50; i++)
        chickList[i] = nullptr;
}

ChickIcon::~ChickIcon()
{
    for (int i = 0; i < count; i++)
        delete chickList[i];
    delete[] chickList;
}

void ChickIcon::onClick()
{
    if (count >= 50)
    {
        cout << "Max chicks reached!" << endl;
        return;
    }

    if (pGame->budget >= 100)
    {
        pGame->budget -= 100;
        pGame->clearBudget();
        pGame->printBudget("BUDGET = $" + to_string(pGame->budget));

        random_device rd;
        mt19937 gen(rd());

        point p;
        p.x = uniform_int_distribution<int>(range_min_x, range_max_x)(gen);
        p.y = uniform_int_distribution<int>(range_min_y, range_max_y)(gen);

        chickList[count] = new Chick(pGame, p, 50, 50, image_path);
        chickList[count]->draw();
        count++;
    }
}

CowIcon::CowIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
    cowList = new Cow * [50];
    for (int i = 0; i < 50; i++)
        cowList[i] = nullptr;
}

CowIcon::~CowIcon()
{
    for (int i = 0; i < count; i++)
        delete cowList[i];
    delete[] cowList;
}

void CowIcon::onClick()
{
    if (count >= 50)
    {
        cout << "Max cows reached!" << endl;
        return;
    }

    if (pGame->budget >= 100)
    {
        pGame->budget -= 100;
        pGame->clearBudget();
        pGame->printBudget("BUDGET = $" + to_string(pGame->budget));

        random_device rd;
        mt19937 gen(rd());

        point p;
        p.x = uniform_int_distribution<int>(range_min_x, range_max_x)(gen);
        p.y = uniform_int_distribution<int>(range_min_y, range_max_y)(gen);

        cowList[count] = new Cow(pGame, p, 50, 50, image_path);
        cowList[count]->draw();
        count++;
    }
}

WaterIcon::WaterIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
    foodList = new FoodArea * [15];
    for (int i = 0; i < 15; i++)
        foodList[i] = nullptr;
}

WaterIcon::~WaterIcon()
{
    for (int i = 0; i < count; i++)
        delete foodList[i];
    delete[] foodList;
}

void WaterIcon::onClick()
{
    if (count >= 15)
    {
        cout << "Max food areas reached!" << endl;
        return;
    }

    if (pGame->budget >= 50)
    {
        pGame->budget -= 50;
        pGame->clearBudget();
        pGame->printBudget("BUDGET = $" + to_string(pGame->budget));

        random_device rd;
        mt19937 gen(rd());

        point p;
        p.x = uniform_int_distribution<int>(range_min_x, range_max_x)(gen);
        p.y = uniform_int_distribution<int>(range_min_y, range_max_y)(gen);

        foodList[count] = new FoodArea(pGame, p, 40, "images\\grass.jpg");
        foodList[count]->draw();
        count++;
    }
}

Budgetbar::Budgetbar(Game* r_pGame, point r_point, int r_width, int r_height)
    : Drawable(r_pGame, r_point, r_width, r_height)
{
    iconsImages[ICON_CHICK] = "images\\chick.jpg";
    iconsImages[ICON_COW] = "images\\cow.jpg";
    iconsImages[ICON_WATER] = "images\\water.jpg";

    point p;
    p.x = 0;
    p.y = config.toolBarHeight;

    iconsList = new BudgetbarIcon * [ANIMAL_COUNT];

    iconsList[ICON_CHICK] = new ChickIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_CHICK]);
    p.x += config.iconWidth;
    iconsList[ICON_COW] = new CowIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_COW]);
    p.x += config.iconWidth;
    iconsList[ICON_WATER] = new WaterIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_WATER]);
}

Budgetbar::~Budgetbar()
{
    for (int i = 0; i < ANIMAL_COUNT; i++)
        delete iconsList[i];
    delete[] iconsList;
}

void Budgetbar::draw() const
{
    for (int i = 0; i < ANIMAL_COUNT; i++)
        iconsList[i]->draw();

    window* pWind = pGame->getWind();
    pWind->SetPen(BLACK, 3);
    pWind->DrawLine(0, 2 * config.toolBarHeight, pWind->GetWidth(), 2 * config.toolBarHeight);

    pWind->SetPen(BLACK, 1);
    pWind->SetFont(16, BOLD, BY_NAME, "Arial");
    pWind->DrawString(230, config.toolBarHeight + 10, "Chicken: $100");
    pWind->DrawString(390, config.toolBarHeight + 10, "Cow: $100");
    pWind->DrawString(520, config.toolBarHeight + 10, "Water: $50");
    pWind->DrawString(config.windWidth - 220, config.toolBarHeight + 10, "Budget: $" + to_string(pGame->budget));
}

bool Budgetbar::handleClick(int x, int y)
{
    if (x > ANIMAL_COUNT * config.iconWidth)
        return false;

    int clickedIconIndex = x / config.iconWidth;
    iconsList[clickedIconIndex]->onClick();
    return false;
}

BudgetbarIcon* Budgetbar::getIcon(int index) const
{
    return iconsList[index];
}