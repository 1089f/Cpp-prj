#pragma once
#include "../Core/Drawable.h"

class FoodArea : public Drawable
{
private:
    int radius;
    int counter;
    string image_path;

public:
    FoodArea(Game* r_pGame, point ref, int r_radius, string img_path);
    virtual void draw() const override;
    void decreaseCounter();     
    bool isEmpty() const;
    void setCounter(int c);
    int getCounter() const; 
};
