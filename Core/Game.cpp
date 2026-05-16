#include "Game.h"
#include <windows.h>
#include "../UI/BudgetBar.h"
#include "../Config/GameConfig.h"
#include <random>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

int getInitialTimer(int level)
{
	int t = 240 - (level - 1) * 10;

	if (t < 10)
		t = 10;

	return t;
}
Game::Game()
{
	lastTime = time(0);
	//1 - Create the main window
	pWind = CreateWind(config.windWidth, config.windHeight, config.wx, config.wy);

	//2 - create and draw the toolbar
	createToolbar();
	createBudgetbar();
	chickIconPtr = (ChickIcon*)gameBudgetbar->getIcon(ICON_CHICK);
	point bgPoint;
	bgPoint.x = 0;
	bgPoint.y = 2 * config.toolBarHeight;
	gameBackground = new Background(this, bgPoint, config.windWidth, config.windHeight - config.statusBarHeight - (2 * config.toolBarHeight), "images\\background.jpg");
	gameBackground->draw();
	//3 - create and draw the backgroundPlayingArea
	point p;
	p.x = 900;
	p.y = 150;
	gameWarehouse = new Warehouse(this, p, 100, 100, "images\\warehouse.jpg");
	gameWarehouse->draw();

	//4- Create the Plane
	//TODO: Add code to create and draw the Plane

	//5- Create the Bullet
	//TODO: Add code to create and draw the Bullet

	//6- Create the enemies
	//TODO: Add code to create and draw enemies in random places

	//7- Create and clear the status bar
	clearStatusBar();
	drawStatusBarDetails();

	mciSendString(TEXT("open sounds\\chickbackground.mp3 type mpegvideo alias bgmusic"), NULL, 0, NULL);
	mciSendString(TEXT("play bgmusic repeat"), NULL, 0, NULL);
	mciSendString(TEXT("setaudio bgmusic volume to 500"), NULL, 0, NULL);
}

Game::~Game()
{
	delete gameBackground;
	delete gameToolbar;
	delete gameBudgetbar;
	delete pWind;
}

clicktype Game::getMouseClick(int& x, int& y) const
{
	return pWind->WaitMouseClick(x, y);
}

string Game::getSrting() const
{
	string Label;
	char Key;
	keytype ktype;
	pWind->FlushKeyQueue();
	while (1)
	{
		ktype = pWind->WaitKeyPress(Key);
		if (ktype == ESCAPE)
			return "";
		if (Key == 13)
			return Label;
		if (Key == 8)
			if (Label.size() > 0)
				Label.resize(Label.size() - 1);
			else
				Key = '\0';
		else
			Label += Key;
		printMessage(Label);
	}
}

window* Game::CreateWind(int w, int h, int x, int y) const
{
	window* pW = new window(w, h, x, y);
	pW->SetBrush(config.bkGrndColor);
	pW->SetPen(config.bkGrndColor, 1);
	pW->DrawRectangle(0, 0, w, h);
	return pW;
}

void Game::createToolbar()
{
	point toolbarUpperleft;
	toolbarUpperleft.x = 0;
	toolbarUpperleft.y = 0;
	gameToolbar = new Toolbar(this, toolbarUpperleft, 0, config.toolBarHeight);
	gameToolbar->draw();
}

void Game::createBudgetbar()
{
	point budgetbarUpperleft;
	budgetbarUpperleft.x = 0;
	budgetbarUpperleft.y = config.toolBarHeight;
	gameBudgetbar = new Budgetbar(this, budgetbarUpperleft, 0, config.toolBarHeight);
	gameBudgetbar->draw();
}

void Game::clearBudget() const
{
	pWind->SetPen(config.bkGrndColor, 1);
	pWind->SetBrush(config.bkGrndColor);
	pWind->DrawRectangle(config.windWidth - 500, config.toolBarHeight, config.windWidth, 2 * config.toolBarHeight);
}

void Game::clearTimer() const
{
	pWind->SetPen(config.bkGrndColor, 1);
	pWind->SetBrush(config.bkGrndColor);
	pWind->DrawRectangle(0, config.toolBarHeight, 200, 2 * config.toolBarHeight);
}

void Game::printTimer(string msg) const
{
	clearTimer();
	pWind->SetPen(config.penColor, 50);
	pWind->SetFont(24, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, config.toolBarHeight + 10, msg);
}

void Game::printBudget(string msg) const
{
	clearBudget();
	pWind->SetPen(config.penColor, 50);
	pWind->SetFont(24, BOLD, BY_NAME, "Arial");
	pWind->DrawString(config.windWidth - 200, config.toolBarHeight + 10, msg);
}

void Game::addBudget(int amount)
{
	budget += amount;
	printBudget("BUDGET = $" + to_string(budget));
}

void Game::restartGame()
{
	budget = 2000;
	timer = 240;

	lastTime = time(0);
	lastTimer = -1;
	lastBudget = -1;

	isPaused = false;
    gameWarehouse->reset();

	ChickIcon* chickIcon = (ChickIcon*)gameBudgetbar->getIcon(ICON_CHICK);

	for (int i = 0; i < chickIcon->count; i++)
	{
		delete chickIcon->chickList[i];
		chickIcon->chickList[i] = nullptr;
	}

	chickIcon->count = 0;

	CowIcon* cowIcon = (CowIcon*)gameBudgetbar->getIcon(ICON_COW);

	for (int i = 0; i < cowIcon->count; i++)
	{
		delete cowIcon->cowList[i];
		cowIcon->cowList[i] = nullptr;
	}

	cowIcon->count = 0;

	WaterIcon* waterIcon = (WaterIcon*)gameBudgetbar->getIcon(ICON_WATER);

	for (int i = 0; i < waterIcon->count; i++)
	{
		delete waterIcon->foodList[i];
		waterIcon->foodList[i] = nullptr;
	}

	waterIcon->count = 0;

	for (int i = 0; i < wolfCount; i++)
	{
		delete wolfList[i];
		wolfList[i] = nullptr;
	}

	wolfCount = 0;

	clearPlayingArea();
	redrawGameObjects();
	gameToolbar->draw();
	gameBudgetbar->draw();
	drawStatusBarDetails();

	printMessage("Game Restarted");
}

void Game::clearStatusBar() const
{
	pWind->SetPen(config.statusBarColor, 1);
	pWind->SetBrush(config.statusBarColor);
	pWind->DrawRectangle(0, config.windHeight - config.statusBarHeight, config.windWidth, config.windHeight);
}

void Game::printMessage(string msg) const
{
	clearStatusBar();
	pWind->SetPen(config.penColor, 50);
	pWind->SetFont(24, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, config.windHeight - (int)(0.85 * config.statusBarHeight), msg);
	drawStatusBarDetails();
}

window* Game::getWind() const
{
	return pWind;
}

void Game::clearPlayingArea() const
{
	pWind->SetPen(config.bkGrndColor, 1);
	pWind->SetBrush(config.bkGrndColor);
	pWind->DrawRectangle(0, 2 * config.toolBarHeight, config.windWidth, config.windHeight - config.statusBarHeight);
	if (gameBackground)
		gameBackground->draw();
}

int Game::getAnimalCount() const
{
	int total = 0;

	if (gameBudgetbar)
	{
		ChickIcon* chickIcon = (ChickIcon*)gameBudgetbar->getIcon(ICON_CHICK);
		CowIcon* cowIcon = (CowIcon*)gameBudgetbar->getIcon(ICON_COW);

		if (chickIcon)
			total += chickIcon->count;

		if (cowIcon)
			total += cowIcon->count;
	}

	return total;
}

void Game::drawStatusBarDetails() const
{
	pWind->SetPen(config.statusBarColor, 1);
	pWind->SetBrush(config.statusBarColor);
	pWind->DrawRectangle(0, config.windHeight - config.statusBarHeight,
		config.windWidth, config.windHeight);

	pWind->SetPen(WHITE, 1);
	pWind->SetFont(16, BOLD, BY_NAME, "Arial");

	int y = config.windHeight - (int)(0.7 * config.statusBarHeight);

	pWind->DrawString(10, y, "Timer: " + to_string(timer));
	pWind->DrawString(200, y, "Goal: $" + to_string(goal));
	pWind->DrawString(400, y, "Level: " + to_string(level));
	pWind->DrawString(600, y, "Animals: " + to_string(getAnimalCount()));
}

void Game::redrawGameObjects() const
{
	if (gameWarehouse)
		gameWarehouse->draw();

	if (gameBudgetbar)
	{
		ChickIcon* chickIcon = (ChickIcon*)gameBudgetbar->getIcon(ICON_CHICK);
		CowIcon* cowIcon = (CowIcon*)gameBudgetbar->getIcon(ICON_COW);
		WaterIcon* waterIcon = (WaterIcon*)gameBudgetbar->getIcon(ICON_WATER);

		if (chickIcon)
		{
			for (int i = 0; i < chickIcon->count; i++)
			{
				if (chickIcon->chickList[i])
				{
					chickIcon->chickList[i]->draw();
					if (chickIcon->chickList[i]->producedEgg)
						chickIcon->chickList[i]->producedEgg->draw();
				}
			}
		}

		if (cowIcon)
		{
			for (int i = 0; i < cowIcon->count; i++)
			{
				if (cowIcon->cowList[i])
				{
					cowIcon->cowList[i]->draw();
					if (cowIcon->cowList[i]->producedMilk)
						cowIcon->cowList[i]->producedMilk->draw();
				}
			}
		}

		if (waterIcon)
		{
			for (int i = 0; i < waterIcon->count; i++)
			{
				if (waterIcon->foodList[i])
					waterIcon->foodList[i]->draw();
			}
		}
	}

	for (int i = 0; i < wolfCount; i++)
	{
		if (wolfList[i])
			wolfList[i]->draw();
	}
}


void Game::saveGame()
{
	ofstream file("savegame.txt");
	if (!file.is_open()) return;

	file << budget << "\n";
	file << timer << "\n";
	file << level << "\n";
	file << goal << "\n";
	file << wolfCount << "\n";
	file << wolfLevel << "\n";
	file << gameWarehouse->getEggCount() << "\n";
	file << gameWarehouse->getMilkCount() << "\n";
	ChickIcon* chickIcon = (ChickIcon*)gameBudgetbar->getIcon(ICON_CHICK);
	file << chickIcon->count << "\n";
	for (int i = 0; i < chickIcon->count; i++)
	{
		Chick* c = chickIcon->chickList[i];
		file << c->getRefPoint().x << " " << c->getRefPoint().y << " ";
		file << c->counter << " ";
		file << (c->producedEgg ? 1 : 0) << "\n";
	}

	CowIcon* cowIcon = (CowIcon*)gameBudgetbar->getIcon(ICON_COW);
	file << cowIcon->count << "\n";
	for (int i = 0; i < cowIcon->count; i++)
	{
		Cow* c = cowIcon->cowList[i];
		file << c->getRefPoint().x << " " << c->getRefPoint().y << " ";
		file << c->counter << " ";
		file << (c->producedMilk ? 1 : 0) << "\n";
	}
	WaterIcon* waterIcon = (WaterIcon*)gameBudgetbar->getIcon(ICON_WATER);

	file << waterIcon->count << "\n";
	for (int i = 0; i < waterIcon->count; i++)
	{
		FoodArea* f = waterIcon->foodList[i];

		file << f->getRefPoint().x << " "
			<< f->getRefPoint().y << " "
			<< f->getCounter() << "\n";
	}
	for (int i = 0; i < wolfCount; i++)
	{
		Wolf* w = wolfList[i];
		file << w->getRefPoint().x << " " << w->getRefPoint().y << " ";
		file << w->getClickCount() << " ";
		file << (w->isVisible() ? 1 : 0) << "\n";
	}

	file.close();
	printMessage("Game Saved!");
}

void Game::loadGame()
{
	ifstream file("savegame.txt");

	if (!file.is_open())
	{
		printMessage("No save file found!");
		return;
	}

	
	restartGame();

	file >> budget;
	file >> timer;
	file >> level;
	file >> goal;
	file >> wolfCount;
	file >> wolfLevel;
	int eggs, milk;

	file >> eggs;
	file >> milk;
	for (int i = 0; i < eggs; i++)
		gameWarehouse->incrementEggs();

	for (int i = 0; i < milk; i++)
		gameWarehouse->incrementMilk();
	ChickIcon* chickIcon = (ChickIcon*)gameBudgetbar->getIcon(ICON_CHICK);

	int chickCount;
	file >> chickCount;

	for (int i = 0; i < chickCount; i++)
	{
		int x, y;
		int counter;
		int hasEgg;

		file >> x >> y >> counter >> hasEgg;

		point p;
		p.x = x;
		p.y = y;

		Chick* chick = new Chick(this, p, 50, 50, "images\\chick.jpg");

		chick->counter = counter;

		if (hasEgg)
		{
			point eggPos;
			eggPos.x = x + 20;
			eggPos.y = y + 20;

			chick->producedEgg = new Egg(this, eggPos, 20, 25);
		}

		chickIcon->chickList[chickIcon->count] = chick;
		chickIcon->count++;
	}

	CowIcon* cowIcon = (CowIcon*)gameBudgetbar->getIcon(ICON_COW);

	int cowCount;
	file >> cowCount;

	for (int i = 0; i < cowCount; i++)
	{
		int x, y;
		int counter;
		int hasMilk;

		file >> x >> y >> counter >> hasMilk;

		point p;
		p.x = x;
		p.y = y;

		Cow* cow = new Cow(this, p, 50, 50, "images\\cow.jpg");

		cow->counter = counter;

		if (hasMilk)
		{
			point milkPos;
			milkPos.x = x + 20;
			milkPos.y = y + 20;

			cow->producedMilk = new Milk(this, milkPos, 20, 30);
		}

		cowIcon->cowList[cowIcon->count] = cow;
		cowIcon->count++;
	}
	WaterIcon* waterIcon = (WaterIcon*)gameBudgetbar->getIcon(ICON_WATER);

	int waterCount;
	file >> waterCount;

	for (int i = 0; i < waterCount; i++)
	{
		int x, y, counter;
		file >> x >> y >> counter;

		point p;
		p.x = x;
		p.y = y;

		FoodArea* food = new FoodArea(this, p, 40, "images\\grass.jpg");
		food->setCounter(counter);

		waterIcon->foodList[waterIcon->count++] = food;
	}
	for (int i = 0; i < wolfCount; i++)
	{
		int x, y;
		int clicks;
		int visible;

		file >> x >> y >> clicks >> visible;

		point p;
		p.x = x;
		p.y = y;

		Wolf* wolf = new Wolf(this, p, wolfLevel);

		for (int j = 0; j < clicks; j++)
			wolf->wolfClicked();

		if (!visible)
		{
			delete wolf;
			wolfList[i] = nullptr;
		}
		else
		{
			wolfList[i] = wolf;
		}

	}

	file.close();

	clearPlayingArea();
	redrawGameObjects();
	gameToolbar->draw();
	gameBudgetbar->draw();
	drawStatusBarDetails();

	printMessage("Game Loaded!");
}

void Game::go()
{
	int x, y;
	bool isExit = false;

	pWind->ChangeTitle("- - - Farm Frenzy - - -");
	pWind->SetBuffering(true);

	gameToolbar->draw();
	gameBudgetbar->draw();

	do
	{


		ChickIcon* chickIcon = (ChickIcon*)gameBudgetbar->getIcon(ICON_CHICK);
		CowIcon* cowIcon = (CowIcon*)gameBudgetbar->getIcon(ICON_COW);

		WaterIcon* waterIcon = (WaterIcon*)gameBudgetbar->getIcon(ICON_WATER);


		if (!isPaused)
		{
			
			time_t currentTime = time(0);
			if (currentTime != lastTime)
			{
				timer--;
				
				if (timer < 0)
					timer = 0;
				lastTime = currentTime;
				
				if (budget >= goal)
				{
					level++;
					goal += 500;
					timer = getInitialTimer(level);
					wolfLevel = level;

					clearPlayingArea();
					redrawGameObjects();
					gameToolbar->draw();
					gameBudgetbar->draw();
					drawStatusBarDetails();
					printMessage("Level Up! Now Level " + to_string(level) + " | New Goal: $" + to_string(goal));
					pWind->UpdateBuffer();
					Sleep(1500);
				}

				for (int i = 0; i < chickIcon->count; i++)
				{
					chickIcon->chickList[i]->moveStep();
				}

				for (int i = 0; i < cowIcon->count; i++)
				{
					cowIcon->cowList[i]->moveStep();
				}

				WaterIcon* waterIcon = (WaterIcon*)gameBudgetbar->getIcon(ICON_WATER);

				for (int i = 0; i < chickIcon->count; i++)
				{
					Chick* chick = chickIcon->chickList[i];
					if (!chick) continue;

					for (int j = 0; j < waterIcon->count; j++)
					{
						FoodArea* food = waterIcon->foodList[j];
						if (!food || food->isEmpty()) continue;

						if (abs(chick->curr_pos.x - food->getRefPoint().x) < 50 &&
							abs(chick->curr_pos.y - food->getRefPoint().y) < 50)
						{
							food->decreaseCounter();
							chick->incrementCounter();
						}
					}
				}

				for (int i = 0; i < cowIcon->count; i++)
				{
					Cow* cow = cowIcon->cowList[i];
					if (!cow) continue;

					for (int j = 0; j < waterIcon->count; j++)
					{
						FoodArea* food = waterIcon->foodList[j];
						if (!food || food->isEmpty()) continue;

						if (abs(cow->curr_pos.x - food->getRefPoint().x) < 50 &&
							abs(cow->curr_pos.y - food->getRefPoint().y) < 50)
						{
							food->decreaseCounter();
							cow->incrementCounter();
						}
					}
				}
			}
		}

		for (int i = 0; i < wolfCount; i++)
		{
			if (wolfList[i])
				wolfList[i]->draw();
		}

		for (int i = 0; i < chickIcon->count; i++)
		{
			chickIcon->chickList[i]->draw();
			if (chickIcon->chickList[i]->producedEgg)
			{
				chickIcon->chickList[i]->producedEgg->draw();
			}
		}

		for (int i = 0; i < cowIcon->count; i++)
		{
			cowIcon->cowList[i]->draw();
			if (cowIcon->cowList[i]->producedMilk)
			{
				cowIcon->cowList[i]->producedMilk->draw();
			}
		}

		if (timer <= 0)
		{
			printMessage("Time Over!");
			pWind->UpdateBuffer();
			continue;
		}

		if (timer != lastTimer)
		{
			printMessage("TIME = " + to_string(timer));
			lastTimer = timer;
		}

		if (budget != lastBudget)
		{
			printBudget("BUDGET = $" + to_string(budget));
			lastBudget = budget;
			gameBudgetbar->draw();
		}

		drawStatusBarDetails();
		pWind->UpdateBuffer();
		Sleep(20);


		if (pWind->GetMouseClick(x, y))
		{
			if (y >= 0 && y < config.toolBarHeight)
			{
				isExit = gameToolbar->handleClick(x, y);
			}
			else if (!isPaused)
			{
				if (y >= config.toolBarHeight && y < 2 * config.toolBarHeight)
				{
					isExit = gameBudgetbar->handleClick(x, y);
					gameBudgetbar->draw();
					drawStatusBarDetails();
				}
				else if (y >= 2 * config.toolBarHeight && y < config.windHeight - config.statusBarHeight)
				{
					if (gameWarehouse->containsPoint(x, y))
                        {
			             	gameWarehouse->openWindow();
                        }
					for (int i = 0; i < wolfCount; i++)
					{

						if (wolfList[i] &&
							wolfList[i]->isVisible() &&
							wolfList[i]->containsPoint(x, y))
						{
							wolfList[i]->wolfClicked();
							printMessage("Wolf clicked!");
							break;
						}
					}

					for (int i = 0; i < chickIcon->count; i++)
					{
						Chick* chick = chickIcon->chickList[i];
						if (chick && chick->producedEgg)
						{
							Egg* egg = chick->producedEgg;
							if (x >= egg->getRefPoint().x && x <= egg->getRefPoint().x + 20 &&
								y >= egg->getRefPoint().y && y <= egg->getRefPoint().y + 25)
							{
								gameWarehouse->incrementEggs();
								chick->producedEgg = nullptr;
								addBudget(45);
								printMessage("Egg collected! +$15 | Total: " + to_string(gameWarehouse->getEggCount()));
							}
						}
					}

					for (int i = 0; i < cowIcon->count; i++)
					{
						Cow* cow = cowIcon->cowList[i];
						if (cow && cow->producedMilk)
						{
							Milk* milk = cow->producedMilk;
							if (x >= milk->getRefPoint().x && x <= milk->getRefPoint().x + 20 &&
								y >= milk->getRefPoint().y && y <= milk->getRefPoint().y + 30)
							{
								gameWarehouse->incrementMilk();
								cow->producedMilk = nullptr;
								addBudget(60);
								printMessage("Milk collected! +$10 | Total: " + to_string(gameWarehouse->getMilkCount()));
							}
						}
					}
				}
			}
		}

		if (timer <= 0)
		{
			printMessage("Game Over!");
			pWind->UpdateBuffer();
			Sleep(20);
			continue;
		}

		if (!isPaused)
		{
			time_t currentTime = time(0);
			if (currentTime != lastTime)
			{
				timer--;
				lastTime = currentTime;
			}

			if (timer % 10 == 0 && timer != lastWolfSpawn)
			{
				lastWolfSpawn = timer;
				static mt19937 gen(random_device{}());
				uniform_int_distribution<int> distX(0, config.windWidth);
				uniform_int_distribution<int> distY(2 * config.toolBarHeight, config.windHeight - config.statusBarHeight);

				point p;
				p.x = distX(gen);
				p.y = distY(gen);

				if (wolfCount < 10)
				{
					wolfList[wolfCount] = new Wolf(this, p, wolfLevel);
					wolfCount++;
				}
			}

			
				for (int i = 0; i < wolfCount; i++)
					if (wolfList[i])
						wolfList[i]->moveStep();

				for (int i = 0; i < chickIcon->count; i++)
					chickIcon->chickList[i]->moveStep();

				for (int i = 0; i < cowIcon->count; i++)
					cowIcon->cowList[i]->moveStep();
			

				for (int i = 0; i < chickIcon->count; i++)
				{
					Chick* chick = chickIcon->chickList[i];
					if (!chick || chick->producedEgg) continue;

					for (int j = 0; j < waterIcon->count; j++)
					{
						FoodArea* food = waterIcon->foodList[j];
						if (!food || food->isEmpty()) continue;

						if (abs(chick->curr_pos.x - food->getRefPoint().x) < 50 &&
							abs(chick->curr_pos.y - food->getRefPoint().y) < 50)
						{
							if (timer != lastTimer)
							{
								food->decreaseCounter();
								chick->incrementCounter();
							}
						}
					}
				}

				for (int i = 0; i < cowIcon->count; i++)
				{
					Cow* cow = cowIcon->cowList[i];
					if (!cow || cow->producedMilk) continue;

					for (int j = 0; j < waterIcon->count; j++)
					{
						FoodArea* food = waterIcon->foodList[j];
						if (!food || food->isEmpty()) continue;

						if (abs(cow->curr_pos.x - food->getRefPoint().x) < 50 &&
							abs(cow->curr_pos.y - food->getRefPoint().y) < 50)
						{
							if (timer != lastTimer)
							{
								food->decreaseCounter();
								cow->incrementCounter();
							}
						}
					}
				}
			

			for (int i = 0; i < wolfCount; i++)
			{
				Wolf* wolf = wolfList[i];
				if (!wolf || !wolf->isVisible()) continue;

				for (int j = 0; j < chickIcon->count; j++)
				{
					Chick* chick = chickIcon->chickList[j];
					if (chick && wolf->isTouching(chick))
					{
						delete chick;
						for (int k = j; k < chickIcon->count - 1; k++)
							chickIcon->chickList[k] = chickIcon->chickList[k + 1];
						chickIcon->chickList[chickIcon->count - 1] = nullptr;
						chickIcon->count--;
						j--;
					}
				}

				for (int j = 0; j < cowIcon->count; j++)
				{
					Cow* cow = cowIcon->cowList[j];
					if (cow && wolf->isTouching(cow))
					{
						delete cow;
						for (int k = j; k < cowIcon->count - 1; k++)
							cowIcon->cowList[k] = cowIcon->cowList[k + 1];
						cowIcon->cowList[cowIcon->count - 1] = nullptr;
						cowIcon->count--;
						j--;
					}
				}
			}
		}

		clearPlayingArea();
		redrawGameObjects();

		if (timer != lastTimer)
		{
			printMessage("TIME = " + to_string(timer));
			lastTimer = timer;
		}

		if (budget != lastBudget)
		{
			printBudget("BUDGET = $" + to_string(budget));
			lastBudget = budget;
			gameBudgetbar->draw();
		}

		drawStatusBarDetails();
		pWind->UpdateBuffer();
		Sleep(20);

	} while (!isExit);
}
