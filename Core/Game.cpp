#include "Game.h"
#include "../UI/BudgetBar.h"
#include "../Config/GameConfig.h"
#include <ctime>

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

void Game::restartGame()
{
	budget = 2000;
	timer = 60;
	lastTime = time(0);
	lastTimer = -1;
	lastBudget = -1;
	isPaused = false;
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
}

void Game::go()
{
	int x, y;
	bool isExit = false;

	pWind->ChangeTitle("- - - Farm Frenzy - - -");

	do
	{

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
				wolfList[wolfCount]->draw();
				wolfCount++;
			}
		}
		for (int i = 0; i < wolfCount; i++)
		{
			wolfList[i]->moveStep();
			wolfList[i]->draw();
		}
		if (!isPaused)
		{
			time_t currentTime = time(0);
			if (currentTime != lastTime)
			{
				timer--;
				lastTime = currentTime;

				ChickIcon* chickIcon = (ChickIcon*)gameBudgetbar->getIcon(ICON_CHICK);
				CowIcon* cowIcon = (CowIcon*)gameBudgetbar->getIcon(ICON_COW);

				for (int i = 0; i < chickIcon->count; i++)
				{
					chickIcon->chickList[i]->moveStep();
					chickIcon->chickList[i]->draw();
				}

				for (int i = 0; i < cowIcon->count; i++)
				{
					cowIcon->cowList[i]->moveStep();
					cowIcon->cowList[i]->draw();
				}
			}
		}

		if (timer <= 0)
		{
			printMessage("Time Over!");
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
		}

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
				}
				else if (y >= 2 * config.toolBarHeight && y < config.windHeight - config.statusBarHeight)  // ADD THIS BLOCK
				{
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
								printMessage("Egg collected! Total: " + to_string(gameWarehouse->getEggCount()));
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
								printMessage("Milk collected! Total: " + to_string(gameWarehouse->getMilkCount()));
							}
						}
					}
				}  
			}  
		}  
	} while (!isExit);
}  