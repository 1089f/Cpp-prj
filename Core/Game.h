#pragma once
#include "../CMUgraphicsLib/CMUgraphics.h"
#include "../UI/Toolbar.h"
#include "../UI/BudgetBar.h"
#include "../Entities/Warehouse.h"
int getInitialTimer(int level);
class Game
{
private:
	window* pWind;	//Pointer to the CMU graphics window
	Toolbar* gameToolbar;
	Budgetbar* gameBudgetbar;
	 window* pWind;
    Toolbar* gameToolbar;
    Budgetbar* gameBudgetbar;
    Warehouse* gameWarehouse; 

public:
	int budget = 2000;
	bool isPaused = false;
	int level = 1;

	int timer = 60;
	time_t lastTime;
	int lastTimer = -1;
	int lastBudget = -1;

	Game();
	~Game();

	clicktype getMouseClick(int& x, int& y) const; //Get coordinate where user clicks and returns click type (left/right)
	string getSrting() const;	 //Returns a string entered by the user


	window* CreateWind(int, int, int, int) const; //creates the game window
	void restartGame();
	void createToolbar();
	void createBudgetbar();
	void clearBudget() const;
	void printBudget(string msg) const;
	void clearStatusBar() const;	//Clears the status bar
	void printTimer(string msg) const;
	void clearTimer() const;
	Wolf* wolfList[10];
	int wolfCount = 0;
	int wolfLevel = 1;
	int lastWolfSpawn = -1; 
	void clearPlayingArea() const;
	void printMessage(string msg) const;	//Print a message on Status bar

	void go();

	window* getWind() const;		//returns a pointer to the graphics window

};

