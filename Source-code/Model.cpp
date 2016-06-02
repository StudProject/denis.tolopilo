/** @file Model.cpp
*  @brief File includes function of creating paratroopers
with parameters for the game.
*
*  @author Denis Tolopilo
*/

#include "Model.h"

// ������� ����� ����������� � ������� ������� r 
// level - ������� ������� ����
/*!
	\brief TSkydiver function creates paratroopers in a certain random position
	and controls the parameters of paratroopers logic when they are on the ground
*/
TSkydiver CreateSkydiver(RECT r, int level)
{
	TSkydiver result;

	int w = r.right - r.left;
	int h = r.bottom - r.top;

	result.X = r.left + rand() % w;
	result.Y = r.top + rand() % h;
	if(result.X > w/2)//���� ��� ������ ������
		result.dX = -(50*level); //�������� ����� � �����
	else
		result.dX = +(50*level); //�������� ������ � �����
	result.dY = +(50*level) ;

	result.r = 13;  // ������ �����������
	result.status = false; 

	return result;
}