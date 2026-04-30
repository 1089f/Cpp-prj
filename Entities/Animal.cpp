#include "Animal.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

Animal::Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
	curr_pos = r_point;
	curr_vel.x = 1;
	curr_vel.y = 1;

	counter = 0;
	maxCounter = 10;

}

void Animal::draw() const
{
	//draw image of this object
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);

	pWind->SetPen(BLACK, 2);
	pWind->SetFont(14, BOLD, BY_NAME, "Arial");
	string counterStr = to_string(counter) + "/" + to_string(maxCounter);
	pWind->DrawString(RefPoint.x + width / 4, RefPoint.y + 5, counterStr);
}

Chick::Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
	maxCounter = 10;
	counter = 0;
	producedEgg = nullptr;
}

void Chick::moveStep()
{
	//TO DO: add code for cleanup and game exit here
	/*
	//draw image of this object in the field
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
	*/
	incrementCounter();
	if (hasProduct())
	{
		point eggPos = { RefPoint.x + width, RefPoint.y };
		if (producedEgg == nullptr)
		{
			producedEgg = new Egg(pGame, eggPos, 20, 25);
			producedEgg->draw();
		}
		counter = 0;
	}
	
	cout << "Icon Chick Clicked" << endl;
}

Cow::Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
	maxCounter = 10;
	counter = 0;
	producedMilk = nullptr;
}

void Cow::moveStep()
{
	incrementCounter();
	if (hasProduct())
	{
		point milkPos = { RefPoint.x + width, RefPoint.y };
		if (producedMilk == nullptr)
		{
			producedMilk = new Milk(pGame, milkPos, 20, 30);
			producedMilk->draw();
		}
		counter = 0;
	}
	cout << "Icon Cow Clicked" << endl;

}
void Wolf::moveStep()
{
	static std::mt19937 gen(std::random_device{}());
	static std::uniform_int_distribution<int> dist(-1, 1);

	curr_pos.x += dist(gen) * speed;
	curr_pos.y += dist(gen) * speed;
}

bool Wolf::isTouching(Animal* other)
{
	return (abs(curr_pos.x - other->curr_pos.x) < width &&
		abs(curr_pos.y - other->curr_pos.y) < height);
}