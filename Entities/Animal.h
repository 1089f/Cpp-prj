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
protected:
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



class Wolf : public Animal
{
public:
	const int WOLF_BASE_SPEED = 3;
	int level;
	int speed;

	Wolf(Game* r_pGame, point r_point, int r_level, string img_path = "images\\cow.jpg")
		: Animal(r_pGame, r_point, 50, 50, img_path)
	{
		
		level = 5;
		speed = WOLF_BASE_SPEED + (r_level / 2);
	}
	virtual void draw() const override;
	virtual void moveStep();
	bool isTouching(Animal* other);
};
