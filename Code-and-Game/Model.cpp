#include "Model.h"

// функция задаёт парашютиста в зданной области r 
// level - текущий уровень игры
TSkydiver CreateSkydiver(RECT r, int level)
{
	TSkydiver result;

	int w = r.right - r.left;
	int h = r.bottom - r.top;

	result.X = r.left + rand() % w;
	result.Y = r.top + rand() % h;
	if(result.X > w/2)//если шар правее центра
		result.dX = -(50*level); //движение влево к пушке
	else
		result.dX = +(50*level); //движение вправо к пушке
	result.dY = +(50*level) ;

	result.r = 13;  // радиус парашютиста
	result.status = false; 

	return result;
}
