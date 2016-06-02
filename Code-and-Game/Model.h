#ifndef __MODEL__
#define __MODEL__

#include <Windows.h>

// структура данных,  характеризующая модель и движение парашютистов
struct TSkydiver
{
	double X,Y; //координаты
	double dX,dY;//прирост координат от времени
	double r; // радиус
	bool status; // статус парашютиста: летит или идёт, false если летит
};

// структура данных, характеризующая орудие
struct TCannon
{
	double X;  //координата орудия
	double v,a;  // скорость и ускорение поворота ствола
	double cannonCD;  // значение заряда лазера
	double traceX; // ордината конца луча
	int traceEnd; //абсцисса конца луча
	int health;  // значение очков здоровья орудия
	bool triggerOn; 
	int score;  //счет
	double alpha; //угол поворота
	bool destroy; //если true то пушка уничтожена
};
//создаёт парашютиста в заданной области
TSkydiver CreateSkydiver(RECT r, int level);

#endif
