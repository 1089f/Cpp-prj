#include "Budgetbar.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include "../Entities/FoodArea.h"
#include <iostream>
using namespace std;

BudgetbarIcon::BudgetbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
}

void BudgetbarIcon::draw() const
{
	//draw image of this object
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

ChickIcon::ChickIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
	chickList = new Chick * [50];
	for (int i = 0; i < 50; i++)
		chickList[i] = nullptr;
}

CowIcon::CowIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
	cowList = new Cow * [50];
	for (int i = 0; i < 50; i++)
		cowList[i] = nullptr;
}

void ChickIcon::onClick()
{
	if (count >= 50)
	{
		cout << "Max chicks reached!" << endl;
		return;
	}
	//TO DO: add code for cleanup and game exit here
	/*
	//draw image of this object in the field
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
	*/

	//Chick* new_chick = new Chick(pGame, RefPoint, 30, 30, "images\\Chick.png");
	cout << "Icon Chick Clicked" << endl;
	if (pGame->budget >= 100) {
		pGame->budget = pGame->budget - 100;
		pGame->clearBudget();
		string budget_string = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budget_string);

		point p;
		// 1. Obtain a seed from a non-deterministic source (if available)
		std::random_device rd1;

		// 2. Seed the Mersenne Twister engine
		// std::mt19937 is a high-quality pseudo-random number generator
		std::mt19937 gen1(rd1());
		std::uniform_int_distribution<int> dist1(range_min_x, range_max_x);
		p.x = dist1(gen1);
		//std::cout << "P.X = " << p.x << endl;
		// 1. Obtain a seed from a non-deterministic source (if available)
		std::random_device rd2;

		// 2. Seed the Mersenne Twister engine
		// std::mt19937 is a high-quality pseudo-random number generator
		std::mt19937 gen2(rd2());
		std::uniform_int_distribution<int> dist2(range_min_y, range_max_y);
		p.y = dist2(gen2);
		//std::cout << "P.Y = " << p.y << endl;
		//p.x = 300;
		//p.y = 300;
		chickList[count] = new Chick(pGame, p, 50, 50, image_path);
		chickList[count]->draw();
		count++;
		//window* pWind = pGame->getWind();
		//pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
	}
}
WaterIcon::WaterIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
    : BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
    foodList = new FoodArea*[15];
    for (int i = 0; i < 15; i++)
        foodList[i] = nullptr;
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
		pGame->budget = pGame->budget - 50;
		pGame->clearBudget();
		string budget_string = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budget_string);

		random_device rd1;
		mt19937 gen1(rd1());
		uniform_int_distribution<int> dist1(range_min_x, range_max_x);

		random_device rd2;
		mt19937 gen2(rd2());
		uniform_int_distribution<int> dist2(range_min_y, range_max_y);

		point p;
		p.x = dist1(gen1);
		p.y = dist2(gen2);

		foodList[count] = new FoodArea(pGame, p, 40, "images\\grass.jpg");
		foodList[count]->draw();
		count++;
	}
}

void CowIcon::onClick()
{
	if (count >= 50)
	{
		cout << "Max cows reached!" << endl;
		return;
	}
	//TO DO: add code for cleanup and game exit here
	/*
	//draw image of this object in the field
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
	*/

	//Cow* new_cow = new Cow(pGame, RefPoint, 30, 30, "images\\Cow.png");
	cout << "Icon Cow Clicked" << endl;
	if (pGame->budget >= 100) {
		pGame->budget = pGame->budget - 100;
		pGame->clearBudget();
		string budget_string = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budget_string);

		point p;
		// 1. Obtain a seed from a non-deterministic source (if available)
		std::random_device rd1;

		// 2. Seed the Mersenne Twister engine
		// std::mt19937 is a high-quality pseudo-random number generator
		std::mt19937 gen1(rd1());
		std::uniform_int_distribution<int> dist1(range_min_x, range_max_x);
		p.x = dist1(gen1);
		//std::cout << "P.X = " << p.x << endl;
		// 1. Obtain a seed from a non-deterministic source (if available)
		std::random_device rd2;

		// 2. Seed the Mersenne Twister engine
		// std::mt19937 is a high-quality pseudo-random number generator
		std::mt19937 gen2(rd2());
		std::uniform_int_distribution<int> dist2(range_min_y, range_max_y);
		p.y = dist2(gen2);
		//std::cout << "P.Y = " << p.y << endl;
		//p.x = 300;
		//p.y = 300;
		cowList[count] = new Cow(pGame, p, 50, 50, image_path);
		cowList[count]->draw();
		count++;
		//window* pWind = pGame->getWind();
		//pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
	}
}

Budgetbar::Budgetbar(Game* r_pGame, point r_point, int r_width, int r_height) : Drawable(r_pGame, r_point, r_width, r_height)
{
	//First prepare List of images for each icon
	//To control the order of these images in the menu, reoder them in enum ICONS above	
	iconsImages[ICON_CHICK] = "images\\chick.jpg";
	iconsImages[ICON_COW] = "images\\cow.jpg";
	iconsImages[ICON_WATER] = "images\\water.jpg";

	point p;
	p.x = 0;
	p.y = config.toolBarHeight;

	iconsList = new BudgetbarIcon * [ANIMAL_COUNT];

	//For each icon in the tool bar create an object 
	iconsList[ICON_CHICK] = new ChickIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_CHICK]);
	p.x += config.iconWidth;
	iconsList[ICON_COW] = new CowIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_COW]);
	p.x += config.iconWidth;
	iconsList[ICON_WATER] = new WaterIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_WATER]);  // ADD THIS
    p.x += config.iconWidth;
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

	
	int clickedIconIndex = (x / config.iconWidth);
	iconsList[clickedIconIndex]->onClick();	

	return false;
}
BudgetbarIcon* Budgetbar::getIcon(int index) const
{
    return iconsList[index];
}
