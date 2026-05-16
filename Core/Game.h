#pragma once
#include "../CMUgraphicsLib/CMUgraphics.h"
#include "../UI/Toolbar.h"
#include "../UI/BudgetBar.h"
#include "../Entities/Warehouse.h"
#include "../Entities/Animal.h"
#include "../Entities/FoodArea.h"
#include "../Entities/Background.h"
#include <vector>
#include <ctime>
int getInitialTimer(int level);
string username;
int score = 0;
class Game
{
private:
	window* pWind;	//Pointer to the CMU graphics window
	Toolbar* gameToolbar;
	Budgetbar* gameBudgetbar;
     

public:
	int budget = 2000;
	bool isPaused = false;
	int level = 1;
	int goal = 2500;

	int timer = 240;
	time_t lastTime;
	int lastTimer = -1;
	int lastBudget = -1;
	Warehouse* gameWarehouse;
	ChickIcon* chickIconPtr;
	Game();
	~Game();

	clicktype getMouseClick(int& x, int& y) const; //Get coordinate where user clicks and returns click type (left/right)
	string getSrting() const;	 //Returns a string entered by the user


	window* CreateWind(int, int, int, int) const; //creates the game window
	void saveGame();
	void restartGame();
    void loadGame();
	void createToolbar();
	void createBudgetbar();
	void clearBudget() const;
	void printBudget(string msg) const;
	void addBudget(int amount);		//Adds amount to budget and refreshes the budget display
	void clearStatusBar() const;	//Clears the status bar
	void printTimer(string msg) const;
	void clearTimer() const;
	Wolf* wolfList[10];
	int wolfCount = 0;
	int wolfLevel = 1;
	int lastWolfSpawn = -1; 
	void clearPlayingArea() const;
	Background* gameBackground;
	void drawStatusBarDetails() const;
	int getAnimalCount() const;
	void redrawGameObjects() const;
	void printMessage(string msg) const;	//Print a message on Status bar

	void go();

	window* getWind() const;		//returns a pointer to the graphics window

};


