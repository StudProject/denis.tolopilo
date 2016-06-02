/** @file Model.h
*  @brief Structures with parameters for the paratroopers game.
*
*  @author Denis Tolopilo
*/

#ifndef __MODEL__
#define __MODEL__

#include <Windows.h>

// ��������� ������,  ��������������� ������ � �������� ������������
/*!
	\brief  TSkydiver structure describes the parameters of the paratroopers. 
	Such as coordinates, coordinate increment, radius and the status of his movements.
*/
struct TSkydiver
{
	double X,Y; //����������
	double dX,dY;//������� ��������� �� �������
	double r; // ������
	bool status; // ������ �����������: ����� ��� ���, false ���� �����
};

// ��������� ������, ��������������� ������
/*!
\brief  TCanon structure describes the parameters of the Cannon.
Such as coordinates, speed, guns charge, cooldown, ordinate and abscissa of tha ray,
value of health, status of the trigger, player's score, angle and the status of the gun.
*/
struct TCannon
{
	double X;  //���������� ������
	double v,a;  // �������� � ��������� �������� ������
	double cannonCD;  // �������� ������ ������
	double traceX; // �������� ����� ����
	int traceEnd; //�������� ����� ����
	int health;  // �������� ����� �������� ������
	bool triggerOn; 
	int score;  //����
	double alpha; //���� ��������
	bool destroy; //���� true �� ����� ����������
};
//������ ����������� � �������� �������
/*!
	\brief  The function creates skydivers for the game according to the level of the game.
*/
TSkydiver CreateSkydiver(RECT r, int level);

#endif