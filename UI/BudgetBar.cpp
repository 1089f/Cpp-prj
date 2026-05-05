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

RestartIcon::RestartIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void RestartIcon::onClick()
{
	cout << "Restart Icon Clicked" << endl;
}

PauseIcon::PauseIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void PauseIcon::onClick()
{
	cout << "Pause Icon Clicked" << endl;
}

ResumeIcon::ResumeIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void ResumeIcon::onClick()
{
	cout << "Resume Icon Clicked" << endl;
}

SaveIcon::SaveIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void SaveIcon::onClick()
{
	cout << "Save Icon Clicked" << endl;
}

LoadIcon::LoadIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void LoadIcon::onClick()
{
	cout << "Load Icon Clicked" << endl;
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

void ChickIcon::onClick()
{
	if (count >= 50)
	{
		cout << "Max chicks reached!" << endl;
		return;
	}

	cout << "Icon Chick Clicked" << endl;

	if (pGame->budget >= 100)
	{
		pGame->budget = pGame->budget - 100;
		pGame->clearBudget();

		string budget_string = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budget_string);

		random_device rd;
		mt19937 gen(rd());

		uniform_int_distribution<int> distX(range_min_x, range_max_x);
		uniform_int_distribution<int> distY(range_min_y, range_max_y);

		point p;
		p.x = distX(gen);
		p.y = distY(gen);

		chickList[count] = new Chick(pGame, p, 50, 50, image_path);
		chickList[count]->draw();

		count++;
	}
	else
	{
		cout << "Not enough budget!" << endl;
	}
}

void CowIcon::onClick()
{
	if (count >= 50)
	{
		cout << "Max cows reached!" << endl;
		return;
	}

	cout << "Icon Cow Clicked" << endl;

	if (pGame->budget >= 100)
	{
		pGame->budget = pGame->budget - 100;
		pGame->clearBudget();

		string budget_string = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budget_string);

		random_device rd;
		mt19937 gen(rd());

		uniform_int_distribution<int> distX(range_min_x, range_max_x);
		uniform_int_distribution<int> distY(range_min_y, range_max_y);

		point p;
		p.x = distX(gen);
		p.y = distY(gen);

		cowList[count] = new Cow(pGame, p, 50, 50, image_path);
		cowList[count]->draw();

		count++;
	}
	else
	{
		cout << "Not enough budget!" << endl;
	}
}

void WaterIcon::onClick()
{
	if (count >= 15)
	{
		cout << "Max food areas reached!" << endl;
		return;
	}

	cout << "Icon Water Clicked" << endl;

	if (pGame->budget >= 50)
	{
		pGame->budget = pGame->budget - 50;
		pGame->clearBudget();

		string budget_string = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budget_string);

		random_device rd;
		mt19937 gen(rd());

		uniform_int_distribution<int> distX(range_min_x, range_max_x);
		uniform_int_distribution<int> distY(range_min_y, range_max_y);

		point p;
		p.x = distX(gen);
		p.y = distY(gen);

		foodList[count] = new FoodArea(pGame, p, 40, "images\\grass.jpg");
		foodList[count]->draw();

		count++;
	}
	else
	{
		cout << "Not enough budget!" << endl;
	}
}

Budgetbar::Budgetbar(Game* r_pGame, point r_point, int r_width, int r_height)
	: Drawable(r_pGame, r_point, r_width, r_height)
{
	iconsImages[ICON_RESTART] = "images\\restart.jpg";
	iconsImages[ICON_PAUSE] = "images\\pause.jpg";
	iconsImages[ICON_RESUME] = "images\\resume.jpg";
	iconsImages[ICON_SAVE] = "images\\save.jpg";
	iconsImages[ICON_LOAD] = "images\\load.jpg";
	iconsImages[ICON_CHICK] = "images\\chick.jpg";
	iconsImages[ICON_COW] = "images\\cow.jpg";
	iconsImages[ICON_WATER] = "images\\water.jpg";

	iconsList = new BudgetbarIcon * [ANIMAL_COUNT];

	point p;

	p.x = 0;
	p.y = 0;

	iconsList[ICON_RESTART] = new RestartIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_RESTART]);

	p.x += config.iconWidth;
	iconsList[ICON_PAUSE] = new PauseIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_PAUSE]);

	p.x += config.iconWidth;
	iconsList[ICON_RESUME] = new ResumeIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_RESUME]);

	p.x += config.iconWidth;
	iconsList[ICON_SAVE] = new SaveIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_SAVE]);

	p.x += config.iconWidth;
	iconsList[ICON_LOAD] = new LoadIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_LOAD]);

	p.x = 0;
	p.y = config.toolBarHeight;

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
	window* pWind = pGame->getWind();

	for (int i = 0; i < ANIMAL_COUNT; i++)
		iconsList[i]->draw();

	pWind->SetPen(BLACK, 3);
	pWind->DrawLine(0, config.toolBarHeight, pWind->GetWidth(), config.toolBarHeight);
	pWind->DrawLine(0, 2 * config.toolBarHeight, pWind->GetWidth(), 2 * config.toolBarHeight);

	pWind->SetPen(BLACK);
	pWind->DrawString((5 * config.iconWidth) + 20, 10, "Toolbar: Restart | Pause | Resume | Save | Load");
	pWind->DrawString((3 * config.iconWidth) + 20, config.toolBarHeight + 10, "BUDGET = $" + to_string(pGame->budget));
	pWind->DrawString((3 * config.iconWidth) + 180, config.toolBarHeight + 10, "Chicken: $100");
	pWind->DrawString((3 * config.iconWidth) + 320, config.toolBarHeight + 10, "Cow: $100");
	pWind->DrawString((3 * config.iconWidth) + 440, config.toolBarHeight + 10, "Water: $50");
}

bool Budgetbar::handleClick(int x, int y)
{
	if (x < 0)
		return false;

	if (y >= 0 && y < config.toolBarHeight)
	{
		if (x >= 5 * config.iconWidth)
			return false;

		int clickedIconIndex = x / config.iconWidth;
		iconsList[clickedIconIndex]->onClick();

		return true;
	}

	if (y >= config.toolBarHeight && y < 2 * config.toolBarHeight)
	{
		if (x >= 3 * config.iconWidth)
			return false;

		int clickedIconIndex = ICON_CHICK + (x / config.iconWidth);
		iconsList[clickedIconIndex]->onClick();

		return true;
	}

	return false;
}

BudgetbarIcon* Budgetbar::getIcon(int index) const
{
	return iconsList[index];
}
    return iconsList[index];
}
