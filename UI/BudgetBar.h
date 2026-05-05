#pragma once
#include "../Core/Drawable.h"
#include "../Entities/Animal.h"
#include "../Config/GameConfig.h"
#include "../Entities/FoodArea.h"
#include <random>
#include <limits>
#include <string>

using namespace std;

const int range_min_x = 50;
const int range_max_x = config.windWidth - 50;
const int range_min_y = (config.toolBarHeight * 2) + 50;
const int range_max_y = config.windHeight - config.statusBarHeight - 50;

class BudgetbarIcon : public Drawable
{
public:
	string image_path;

	BudgetbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual ~BudgetbarIcon() {}

	virtual void draw() const override;
	virtual void onClick() = 0;
};

class RestartIcon : public BudgetbarIcon
{
public:
	RestartIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void onClick();
};

class PauseIcon : public BudgetbarIcon
{
public:
	PauseIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void onClick();
};

class ResumeIcon : public BudgetbarIcon
{
public:
	ResumeIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void onClick();
};

class SaveIcon : public BudgetbarIcon
{
public:
	SaveIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void onClick();
};

class LoadIcon : public BudgetbarIcon
{
public:
	LoadIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void onClick();
};

class ChickIcon : public BudgetbarIcon
{
public:
	Chick** chickList;
	int count = 0;

	ChickIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual ~ChickIcon();
	virtual void onClick();
};

class CowIcon : public BudgetbarIcon
{
public:
	Cow** cowList;
	int count = 0;

	CowIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual ~CowIcon();
	virtual void onClick();
};

class WaterIcon : public BudgetbarIcon
{
public:
	FoodArea** foodList;
	int count = 0;

	WaterIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual ~WaterIcon();
	virtual void onClick();
};

enum ANIMAL_ICONS
{
	ICON_RESTART,
	ICON_PAUSE,
	ICON_RESUME,
	ICON_SAVE,
	ICON_LOAD,
	ICON_CHICK,
	ICON_COW,
	ICON_WATER,
	ANIMAL_COUNT
};

class Budgetbar : public Drawable
{
private:
	BudgetbarIcon** iconsList;
	string iconsImages[ANIMAL_COUNT];

public:
	Budgetbar(Game* r_pGame, point r_point, int r_width, int r_height);
	~Budgetbar();

	void draw() const override;
	bool handleClick(int x, int y);
	BudgetbarIcon* getIcon(int index) const;
};
	
};
