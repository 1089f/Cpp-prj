#pragma once
#include "../Core/Drawable.h"

class Warehouse : public Drawable
{
private:
    string image_path;
    int eggCount;       
    int milkCount;

public:
    Warehouse(Game* r_pGame, point ref, int r_width, int r_height, string img_path);
    virtual void draw() const override;
    void incrementEggs();       
    void incrementMilk();       
    int getEggCount() const;    
    int getMilkCount() const;
};
