#include "Animal.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

Animal::Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Drawable(r_pGame, r_point, r_width, r_height)
{
image_path = img_path;
curr_pos = r_point;
curr_vel.x = 1;
curr_vel.y = 1;

counter = 0;
maxCounter = 10;
producedEgg = nullptr;
producedMilk = nullptr;
}

void Animal::draw() const
{
//draw image of this object
window* pWind = pGame->getWind();
pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);

pWind->SetPen(config.bkGrndColor, 1);
pWind->SetBrush(config.bkGrndColor);
pWind->DrawRectangle(RefPoint.x, RefPoint.y - 20, RefPoint.x + width, RefPoint.y);


pWind->SetPen(BLACK, 2);
pWind->SetFont(14, BOLD, BY_NAME, "Arial");
string counterStr = to_string(counter) + "/" + to_string(maxCounter);
pWind->DrawString(RefPoint.x + width / 4, RefPoint.y - 15, counterStr);
}

Chick::Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
maxCounter = 10;
counter = 0;
producedEgg = nullptr;

curr_vel.x = (rand() % 3) - 1;
curr_vel.y = (rand() % 3) - 1;

if (curr_vel.x == 0 && curr_vel.y == 0)
curr_vel.x = 1;
}

void Chick::moveStep()
{
	static int frameCounter = 0;
	if (frameCounter++ % 30 == 0 && curr_vel.x == 0 && curr_vel.y == 0) {
		static std::mt19937 gen(std::random_device{}());
		static std::uniform_int_distribution<int> dist(-1, 1);
		curr_vel.x = dist(gen);
		curr_vel.y = dist(gen);
	}

	RefPoint.x += curr_vel.x * 6;
	RefPoint.y += curr_vel.y * 6;

	if (RefPoint.x < 0) {
		RefPoint.x = 0;
		curr_vel.x = -curr_vel.x;
	}
	if (RefPoint.x > config.windWidth - width) {
		RefPoint.x = config.windWidth - width;
		curr_vel.x = -curr_vel.x;
	}
	if (RefPoint.y < 2 * config.toolBarHeight) {
		RefPoint.y = 2 * config.toolBarHeight;
		curr_vel.y = -curr_vel.y;
	}
	if (RefPoint.y > config.windHeight - config.statusBarHeight - height) {
		RefPoint.y = config.windHeight - config.statusBarHeight - height;
		curr_vel.y = -curr_vel.y;
	}

	curr_pos = RefPoint;


	if (producedEgg == nullptr)
	{
		if (pGame->timer != pGame->lastTimer)
		{
			incrementCounter();
			if (hasProduct())
			{
				point eggPos = { RefPoint.x + width, RefPoint.y };
				producedEgg = new Egg(pGame, eggPos, 20, 25);
				producedEgg->draw();
				counter = 0;
			}
		}
	}
	cout << "Icon Chick Clicked" << endl;
}

Cow::Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path)
{
maxCounter = 10;
counter = 0;
producedMilk = nullptr;

curr_vel.x = (rand() % 3) - 1;
curr_vel.y = (rand() % 3) - 1;

if (curr_vel.x == 0 && curr_vel.y == 0)
curr_vel.x = 1;
}

void Cow::moveStep()
{
	static int frameCounter = 0;
	if (frameCounter++ % 30 == 0 && curr_vel.x == 0 && curr_vel.y == 0) {
		static std::mt19937 gen(std::random_device{}());
		static std::uniform_int_distribution<int> dist(-1, 1);
		curr_vel.x = dist(gen);
		curr_vel.y = dist(gen);
	}

	RefPoint.x += curr_vel.x * 6;
	RefPoint.y += curr_vel.y * 6;

	if (RefPoint.x < 0) {
		RefPoint.x = 0;
		curr_vel.x = -curr_vel.x;
	}
	if (RefPoint.x > config.windWidth - width) {
		RefPoint.x = config.windWidth - width;
		curr_vel.x = -curr_vel.x;
	}
	if (RefPoint.y < 2 * config.toolBarHeight) {
		RefPoint.y = 2 * config.toolBarHeight;
		curr_vel.y = -curr_vel.y;
	}
	if (RefPoint.y > config.windHeight - config.statusBarHeight - height) {
		RefPoint.y = config.windHeight - config.statusBarHeight - height;
		curr_vel.y = -curr_vel.y;
	}
	
	curr_pos = RefPoint;
	if (producedMilk == nullptr)
	{
		if (pGame->timer != pGame->lastTimer)
		{
			incrementCounter();
			if (hasProduct())
			{
				point milkPos = { RefPoint.x + width, RefPoint.y };
				producedMilk = new Milk(pGame, milkPos, 20, 30);
				producedMilk->draw();
				counter = 0;
			}
		}
	}
	cout << "Icon Cow Clicked" << endl;
}

Egg::Egg(Game* r_pGame, point r_point, int r_width, int r_height)
: Drawable(r_pGame, r_point, r_width, r_height) {
}

void Egg::draw() const
{
window* pWind = pGame->getWind();
pWind->SetPen(BLACK, 2);
pWind->SetBrush(WHITE);
pWind->DrawEllipse(RefPoint.x, RefPoint.y,
RefPoint.x + width, RefPoint.y + height);
}

Milk::Milk(Game* r_pGame, point r_point, int r_width, int r_height)
: Drawable(r_pGame, r_point, r_width, r_height) {
}

void Milk::draw() const
{
window* pWind = pGame->getWind();
pWind->SetPen(BLACK, 2);
pWind->SetBrush(WHITE);
pWind->DrawRectangle(RefPoint.x, RefPoint.y,
RefPoint.x + width, RefPoint.y + height);
// Neck of bottle
pWind->DrawRectangle(RefPoint.x + width / 4, RefPoint.y - height / 4,
RefPoint.x + 3 * width / 4, RefPoint.y);
}

void Animal::incrementCounter()
{
counter++;
}

bool Animal::hasProduct() const
{
return counter >= maxCounter;
}

void Wolf::moveStep() {
	if (!visible)
	    return;
	static int frameCounter = 0;
	if (frameCounter++ % 30 == 0 && curr_vel.x == 0 && curr_vel.y == 0) {
		static std::mt19937 gen(std::random_device{}());
		static std::uniform_int_distribution<int> dist(-1, 1);
		curr_vel.x = dist(gen);
		curr_vel.y = dist(gen);
	}

	RefPoint.x += curr_vel.x * speed;
	RefPoint.y += curr_vel.y * speed;

	if (RefPoint.x < 0) {
		RefPoint.x = 0;
		curr_vel.x = -curr_vel.x;
	}
	if (RefPoint.x > config.windWidth - width) {
		RefPoint.x = config.windWidth - width;
		curr_vel.x = -curr_vel.x;
	}
	if (RefPoint.y < 2 * config.toolBarHeight) {
		RefPoint.y = 2 * config.toolBarHeight;
		curr_vel.y = -curr_vel.y;
	}
	if (RefPoint.y > config.windHeight - config.statusBarHeight - height) {
		RefPoint.y = config.windHeight - config.statusBarHeight - height;
		curr_vel.y = -curr_vel.y;
	}
	curr_pos = RefPoint;
}

void Wolf::draw() const
{
	if (!visible)
		return;

	window* Wind = pGame->getWind();
	Wind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}
void Wolf::wolfClicked()
{
	clickCount++;

	if (clickCount >= 5)
	{
		visible = false;
		PlaySound(TEXT("sounds\\wolf_die.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
}

bool Wolf::containsPoint(int x, int y) const
{
	return (x >= RefPoint.x &&
		    x <= RefPoint.x + width &&
		    y >= RefPoint.y &&
		    y <= RefPoint.y + height);
}
bool Wolf::isTouching(Animal* other)
{
	return (abs(curr_pos.x - other->curr_pos.x) < width &&
		abs(curr_pos.y - other->curr_pos.y) < height);
	

}
Animal::~Animal()
{
	if (producedEgg)
	{
		delete producedEgg;
		producedEgg = nullptr;
	}

	if (producedMilk)
	{
		delete producedMilk;
		producedMilk = nullptr;
	}
}
