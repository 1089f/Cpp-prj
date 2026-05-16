#include "Background.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"

Background::Background(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
}

void Background::draw() const
{
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);

	// Optionally draw the black border lines around the field if desired
	pWind->SetPen(BLACK, 3);
	pWind->DrawLine(RefPoint.x, RefPoint.y, RefPoint.x + width, RefPoint.y);
	pWind->DrawLine(RefPoint.x, RefPoint.y + height, RefPoint.x + width, RefPoint.y + height);
	pWind->DrawLine(RefPoint.x, RefPoint.y, RefPoint.x, RefPoint.y + height);
	pWind->DrawLine(RefPoint.x + width, RefPoint.y, RefPoint.x + width, RefPoint.y + height);
}
