#include "Game.h"
#include <windows.h>
#include "../UI/BudgetBar.h"
#include "../Config/GameConfig.h"
#include <random>

int getInitialTimer(int level)
{
	int t = 60 - (level - 1) * 10;

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
	drawFieldBackground();
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
}

Game::~Game()
{
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
	// reset game values
	budget = 2000;
	timer = 60;

	lastTime = time(0);
	lastTimer = -1;
	lastBudget = -1;

	isPaused = false;

	// delete chicks
	ChickIcon* chickIcon = (ChickIcon*)gameBudgetbar->getIcon(ICON_CHICK);

	for (int i = 0; i < chickIcon->count; i++)
	{
		delete chickIcon->chickList[i];
		chickIcon->chickList[i] = nullptr;
	}

	chickIcon->count = 0;

	// delete cows
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

	// delete wolves
	for (int i = 0; i < wolfCount; i++)
	{
		delete wolfList[i];
		wolfList[i] = nullptr;
	}

	wolfCount = 0;

	// clear screen
	clearPlayingArea();
	drawFieldBackground();
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
	drawFieldBackground();
}

void Game::drawFieldBackground() const
{
	int left = 0;
	int top = 2 * config.toolBarHeight;
	int right = config.windWidth;
	int bottom = config.windHeight - config.statusBarHeight;

	pWind->SetBrush(LIGHTGREEN);
	pWind->SetPen(GREEN, config.penWidth);
	pWind->DrawRectangle(left, top, right, bottom);

	pWind->SetPen(BLACK, 3);
	pWind->DrawLine(left, top, right, top);
	pWind->DrawLine(left, bottom, right, bottom);
	pWind->DrawLine(left, top, left, bottom);
	pWind->DrawLine(right, top, right, bottom);
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
	pWind->SetPen(WHITE, 1);
	pWind->SetFont(16, BOLD, BY_NAME, "Arial");

	int y = config.windHeight - (int)(0.45 * config.statusBarHeight);

	pWind->DrawString(360, y, "Timer: " + to_string(timer));
	pWind->DrawString(520, y, "Goal: $" + to_string(goal));
	pWind->DrawString(680, y, "Level: " + to_string(level));
	pWind->DrawString(820, y, "Animals: " + to_string(getAnimalCount()));
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
					chickIcon->chickList[i]->draw();
			}
		}

		if (cowIcon)
		{
			for (int i = 0; i < cowIcon->count; i++)
			{
				if (cowIcon->cowList[i])
					cowIcon->cowList[i]->draw();
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
		clearPlayingArea();
		redrawGameObjects();
		gameToolbar->draw();
		gameBudgetbar->draw();
		drawStatusBarDetails();

		// spawn wolf every 10 seconds
		if (!isPaused && timer % 10 == 0 && timer != lastWolfSpawn)
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
		if (!isPaused)
		{
			for (int i = 0; i < wolfCount; i++)
			{
				wolfList[i]->moveStep();
			}
		}


		ChickIcon* chickIcon = (ChickIcon*)gameBudgetbar->getIcon(ICON_CHICK);
		CowIcon* cowIcon = (CowIcon*)gameBudgetbar->getIcon(ICON_COW);

		if (!isPaused)
		{
			time_t currentTime = time(0);
			if (currentTime != lastTime)
			{
				timer--;
				lastTime = currentTime;

				for (int i = 0; i < chickIcon->count; i++)
				{
					chickIcon->chickList[i]->moveStep();
				}

				for (int i = 0; i < cowIcon->count; i++)
				{
					cowIcon->cowList[i]->moveStep();
				}
			}
		}

		WaterIcon* waterIcon = (WaterIcon*)gameBudgetbar->getIcon(ICON_WATER);
		for (int i = 0; i < waterIcon->count; i++)
		{
			if (waterIcon->foodList[i] && !waterIcon->foodList[i]->isEmpty())
			{
				waterIcon->foodList[i]->draw();
			}
		}

		for (int i = 0; i < wolfCount; i++)
		{
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
					bool itemCollected = false;
					ChickIcon* chickIcon = (ChickIcon*)gameBudgetbar->getIcon(ICON_CHICK);
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
								addBudget(15);
								printMessage("Egg collected! +$15 | Total: " + to_string(gameWarehouse->getEggCount()));
								itemCollected = true;
							}
						}
					}
					CowIcon* cowIcon = (CowIcon*)gameBudgetbar->getIcon(ICON_COW);
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
								addBudget(10);
								printMessage("Milk collected! +$10 | Total: " + to_string(gameWarehouse->getMilkCount()));
								itemCollected = true;
							}
						}
					}
				}
			}
		}
		if (!isPaused) {
			for (int w = 0; w < wolfCount; w++) {

				for (int c = 0; c < chickIcon->count; c++) {
					if (chickIcon->chickList[c] != nullptr && wolfList[w]->isTouching(chickIcon->chickList[c])) {
						if (chickIcon->chickList[c]->producedEgg) {
							delete chickIcon->chickList[c]->producedEgg;
						}
						delete chickIcon->chickList[c];
						for (int k = c; k < chickIcon->count - 1; k++) {
							chickIcon->chickList[k] = chickIcon->chickList[k + 1];
						}
						chickIcon->chickList[chickIcon->count - 1] = nullptr;
						chickIcon->count--;
						c--;
					}
				}

				for (int c = 0; c < cowIcon->count; c++) {
					if (cowIcon->cowList[c] != nullptr && wolfList[w]->isTouching(cowIcon->cowList[c])) {
						if (cowIcon->cowList[c]->producedMilk) {
							delete cowIcon->cowList[c]->producedMilk;
						}
						delete cowIcon->cowList[c];
						for (int k = c; k < cowIcon->count - 1; k++) {
							cowIcon->cowList[k] = cowIcon->cowList[k + 1];
						}
						cowIcon->cowList[cowIcon->count - 1] = nullptr;
						cowIcon->count--;
						c--;
					}
				}
			}
		}

		pWind->UpdateBuffer();
	} while (!isExit);
}