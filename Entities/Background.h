#pragma once
#include "../Core/Drawable.h"
#include <string>

using namespace std;    

class Background : public Drawable
{
private:
	string image_path;

public:
	Background(Game* r_pGame, point r_point, int r_width, int r_height, string img_path);
	virtual void draw() const override;
};
