#pragma once
#include "../Core/Drawable.h"

class FoodArea : public Drawable
{
private:
    int radius;
    int counter;

public:
    FoodArea(Game* r_pGame, point ref, int r_radius);
    virtual void draw() const override;
    void decreaseCounter();     
    bool isEmpty() const;       
    int getCounter() const; 
};
