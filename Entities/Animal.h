#pragma once
#include "../Core/Drawable.h"

class Egg : public Drawable
{
public:
	Egg(Game* r_pGame, point r_point, int r_width, int r_height);
	virtual void draw() const override;
};
class Milk : public Drawable
{
public:
	Milk(Game* r_pGame, point r_point, int r_width, int r_height);
	virtual void draw() const override;
};

class Animal :public Drawable
{
private:
	string image_path;
public:
	point curr_pos;
	point curr_vel;

	int counter;
	int maxCounter;
	Egg* producedEgg;
	Milk* producedMilk;

	Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void draw() const override;
	virtual void moveStep() = 0;   //The action that should be taken each time step

	void incrementCounter();
	bool hasProduct() const;
};

class Chick : public Animal
{
public:
	Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void moveStep();
};

class Cow : public Animal
{
public:
	Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void moveStep();
};
