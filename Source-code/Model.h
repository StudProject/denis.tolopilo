/** @file Model.h
*  @brief Structures with parameters for the paratroopers game.
*
*  @author Denis Tolopilo
*/

#ifndef __MODEL__
#define __MODEL__

#include <Windows.h>

// структура данных,  характеризующая модель и движение парашютистов
/*!
	\brief  TSkydiver structure describes the parameters of the paratroopers. 
	Such as coordinates, coordinate increment, radius and the status of his movements.
*/
struct TSkydiver
{
	double X,Y; //координаты
	double dX,dY;//прирост координат от времени
	double r; // радиус
	bool status; // статус парашютиста: летит или идёт, false если летит
};

// структура данных, характеризующая орудие
/*!
\brief  TCanon structure describes the parameters of the Cannon.
Such as coordinates, speed, guns charge, cooldown, ordinate and abscissa of tha ray,
value of health, status of the trigger, player's score, angle and the status of the gun.
*/
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
/*!
	\brief  The function creates skydivers for the game according to the level of the game.
*/
TSkydiver CreateSkydiver(RECT r, int level);

#endif