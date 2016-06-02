/** @file Source.cpp
*  @brief Main file of the game that includes principally major code of the game.
*
*  @author Denis Tolopilo
*/

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include <Windowsx.h>

#include "Model.h"

// Global variables


// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Paratroopers by Denis Tolopilo");

HINSTANCE hInst;
HBITMAP hBitmap = NULL; //!

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define PI (double)3.1415
#define SKYDIVER_COUNT 5
#define CANNON_SIZE 30
#define CANNON_POSITION 525
#define CANNON_V_MAX 300
#define CANNON_A 400
#define CANNON_CD 2000 //����� � �������������
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define LAND_HEIGHT 85
#define ALPHA_MIN 30
#define ALPHA_MAX 150
#define FILE_NAME "save.game"// ��� ����� � ������� ����� ���������� ����, ��������� � ���������� � �����

TCannon cannon;//������
TSkydiver skydrivers[SKYDIVER_COUNT]; // �����������
int level = 1; // ������� ���� ��������������� �������� ������� ������������
bool FlagStop=false; // ���� ��������� ����, ������������ � true, ����� ���� ���� �����������, �������� ��� �������� ���������� ����.
int c=1;

void DoPhysics(ULONGLONG dT);
void StartGame();
void Save();
void Load();

//��������� ���� � ����
/*!
\brief Function that realize saving of the game.
*/
void Save(HWND hWnd){
	FILE * file = fopen(FILE_NAME, "wb");//������� ���� ��� �������� ������
	if(!file){
		MessageBox(hWnd,_T("������. �� ������� ��������� ����"), _T("������ ���������� ����"), NULL);
		FlagStop=true;
		return;
	}
	//���������� � ���� ��� ���� ��������� cannon � ������� ����
	fprintf(file,"%lf %lf %lf %d %d %d %d %lf %d %lf %lf %d",
		cannon.a, cannon.cannonCD, cannon.alpha, cannon.destroy, cannon.health, cannon.score, cannon.traceEnd, cannon.traceX, cannon.triggerOn, cannon.v, cannon.X, level);        

	//���������� ������ � �������������
	for (int i=0;i<SKYDIVER_COUNT;i++)
	{
		//���������� ��� ���� i �����������
		fprintf(file," %lf %lf %lf %d %lf %lf",
			skydrivers[i].dX, skydrivers[i].dY, skydrivers[i].r, skydrivers[i].status, skydrivers[i].X, skydrivers[i].Y);        
	}

	fclose(file);                            // �������� ����� file 

	MessageBox(hWnd,_T("���� ������� ���������"), _T("����������"), NULL);
	FlagStop=true;
}

//������ ���� �� �����
/*!
\brief Function that realize loading of the game.
*/
void Load(HWND hWnd){
	FILE * file = fopen(FILE_NAME, "rb");//������� ���� ��� ������
	if(!file){
		MessageBox(hWnd,_T("������. �� ������� ��������� ����, �������� ���� ��� �� ���� ���������."), _T("������ �������� ����"), NULL);
		FlagStop=true;
		return;
	}

	//������ �� ����� ��� ���� ��������� cannon � ������� ����
	fscanf(file,"%lf %lf %lf %d %d %d %d %lf %d %lf %lf %d",
		&cannon.a, &cannon.cannonCD, &cannon.alpha, &cannon.destroy, &cannon.health, &cannon.score, &cannon.traceEnd, &cannon.traceX, &cannon.triggerOn, &cannon.v, &cannon.X, &level);        

	//���������� ������ � �������������
	for (int i=0;i<SKYDIVER_COUNT;i++)
	{
		//������ ��� ���� i �����������
		fscanf(file," %lf %lf %lf %d %lf %lf",
			&skydrivers[i].dX, &skydrivers[i].dY, &skydrivers[i].r, &skydrivers[i].status, &skydrivers[i].X, &skydrivers[i].Y);        
	}
	fclose(file);                            // �������� ����� file 
	MessageBox(hWnd,_T("���� ������� ���������"), _T("��������"), NULL);
	FlagStop=true;
}

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Paratrooper"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Paratrooper"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	ULONGLONG prevTime = -1, curTime, deltaT;


	StartGame();
	// Enter the infinite message loop
	while(TRUE)
	{
		// Check to see if any messages are waiting in the queue
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Translate the message and dispatch it to WindowProc()
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If the message is WM_QUIT, exit the while loop
		if(msg.message == WM_QUIT)
			break;

		curTime = GetTickCount64();

		if(FlagStop==true){ // ���� ���� ���������, �� ����� ����� ��������
			prevTime= -1;
			FlagStop=false;
		}

		if (prevTime == -1)
			deltaT = 0;
		else 
			deltaT = curTime - prevTime;
		prevTime = curTime;

		DoPhysics(deltaT);

		InvalidateRect(hWnd,NULL,1);
	}

	return (int) msg.wParam;
}
/*!
\brief Function void StartGame() has been created to operate with the start pont of our game
This function operates with numbers of paratroopers and cannon paratemers such as speed, health et cetera.
*/
void StartGame()
{   
	srand(GetTickCount());
	// �������� ������������ � ������������ �������
	RECT r = {0,0,800,20};
	for (int i=0;i<SKYDIVER_COUNT;i++)
	{
		skydrivers[i] = CreateSkydiver(r, level);
	}

	cannon.X = 400; //�������� �����
	cannon.a = 0;
	cannon.v = 0;
	cannon.health = 3; //�������� �����
	cannon.alpha=90; //��������� ���� ������ 90
	cannon.triggerOn=false; //���������� ������ ���������
	cannon.cannonCD=2000; // � ����� ������ ����� �������
	cannon.destroy = false;
	cannon.score=0;
	level = 1;

}

/*!
\brief Function has been created to operate with main physics of our game.
This function describes the interaction of objects in the game, a shot gun controls, 
movement direction of the beam, the amount of charge in the gun, inclination, inertia, and so on.
All physics part of the game is realizing in this function.
*/
void DoPhysics(ULONGLONG dT)
{
	RECT r = {0,0,800,20};
	// ���������� ������ ����� ������ 10 ��������� �����
	if(level*10<=cannon.score-10)
		level++;
	for (int i=0;i<SKYDIVER_COUNT;i++)
	{
		//���������� ���������
		if(skydrivers[i].status==false) //���� �����
			skydrivers[i].Y += ((double)dT/1000)*skydrivers[i].dY;// �������������� ��������
		else{ //���
			skydrivers[i].X += ((double)dT/1000)*skydrivers[i].dX;// �������������� ��������
		}

		if (skydrivers[i].Y > (CANNON_POSITION-skydrivers[i].r))//���� ������ �����
		{			
			skydrivers[i].status=true;
		}

		//�������� ������������
		double dx = skydrivers[i].X - cannon.X;
		double dy = skydrivers[i].Y - CANNON_POSITION;

		if (sqrt(dx*dx + dy*dy) <= skydrivers[i].r + CANNON_SIZE)
		{
			cannon.health -= 1;
			skydrivers[i] = CreateSkydiver(r, level);
		}
	}

	//������� �����
	cannon.alpha -= ((double)dT/3000)*cannon.v;
	if (cannon.alpha > ALPHA_MAX)
	{
		cannon.alpha = ALPHA_MAX;
		cannon.v = 0;
	}

	if (cannon.alpha < ALPHA_MIN)
	{
		cannon.alpha = ALPHA_MIN;
		cannon.v = 0;
	}

	cannon.v += ((double)dT/1000)*cannon.a;
	if (cannon.v > CANNON_V_MAX)
		cannon.v = CANNON_V_MAX;
	if (cannon.v < -1*CANNON_V_MAX)
		cannon.v = -1*CANNON_V_MAX;


	//������� �����
	if (cannon.triggerOn) //���� ����� ��������
	{
		double radA = PI*cannon.alpha/180.0;// ���� ����� � ��������
		double tgA = 0;
		if(cannon.alpha!=90)// ��� 90 ������� �� ��������
			tgA=sin(radA)/cos(radA); // ������� ����
		if(cannon.cannonCD<1){// ���� ������� �� �������� �� ��������� �����
			cannon.triggerOn=false;
		}
		if(cannon.alpha==90){//���� ���� ����� 90 �� ������� �� ���������
			cannon.traceX = cannon.X;
			cannon.traceEnd = 0;
		}
		else{
			// ������� � ����� ������� ���� ������ ��� �����, ���� �� �������� �����������, � ���������� ��� ������
			double len=0;
			if((WINDOW_WIDTH - cannon.X)*tgA>CANNON_POSITION||(-cannon.X)*tgA>CANNON_POSITION){
				// ��� ���������� ������� �������
				cannon.traceEnd = 0;
				cannon.traceX = cannon.X+(CANNON_POSITION)/tgA;
			}
			else{
				if(cannon.alpha<90){
					len = sqrt(double((cannon.X - WINDOW_WIDTH)*(cannon.X - WINDOW_WIDTH)+((WINDOW_WIDTH - cannon.X)*tgA)*((WINDOW_WIDTH - cannon.X)*tgA)));//��� ���������� ������ ������� ����
				}
				else{
					len = sqrt(double((cannon.X)*(cannon.X)+((cannon.X)*tgA)*((cannon.X)*tgA))); // ��� ���������� ����� ������� ����
				}
				cannon.traceEnd = CANNON_POSITION - (len)*sin(radA);
				cannon.traceX = (len)*cos(radA)+cannon.X;
			}
			//���������� ���������� ����� ����

		}
		int asteroidHit = -1; //���������� ������ ������������
		for (int i=0;i<SKYDIVER_COUNT;i++)
		{
			if(cannon.alpha==90){//���� ���� �������� 90 �� ����� ������ ���� ���� � ��� �� ���������
				if (abs(cannon.X - skydrivers[i].X) < skydrivers[i].r)
				{
					if (cannon.traceEnd < skydrivers[i].Y)
					{
						cannon.traceEnd = skydrivers[i].Y;
						asteroidHit = i;
					}

				}
			}
			else{
				if(cannon.alpha<90){//���� ���� ������ 90
					int x = skydrivers[i].X-skydrivers[i].r;
					int y=0;
					while(x<(skydrivers[i].X+skydrivers[i].r)){//�������� �� ����� ����� ����������� � �� ��� ����� ����� �� ����� ���� � �������� ������ �����  �� ��������� � �����������
						x++;
						y = CANNON_POSITION-(x-cannon.X)*tgA;
						if(skydrivers[i].r>=sqrt((double)((x-skydrivers[i].X)*(x-skydrivers[i].X)+(y-skydrivers[i].Y)*(y-skydrivers[i].Y)))){//���� ����� � ����������� ������ �� ������
							cannon.traceEnd = y;
							cannon.traceX = x;
							asteroidHit = i;
							break;
						}
					}
				}
				else{//���� ���� ������ 90
					int x = skydrivers[i].X+skydrivers[i].r;
					int y=0;
					while(x>(skydrivers[i].X-skydrivers[i].r)){//�������� �� ����� ����� ����������� � �� ��� ����� ����� �� ����� ���� � �������� ������ �����  �� ��������� � �����������
						x--;
						y = CANNON_POSITION-(x-cannon.X)*tgA;
						if(skydrivers[i].r>=sqrt((double)((x-skydrivers[i].X)*(x-skydrivers[i].X)+(y-skydrivers[i].Y)*(y-skydrivers[i].Y)))){//���� ����� � ����������� ������ �� ������
							cannon.traceEnd = y;
							cannon.traceX = x;
							asteroidHit = i;
							break;
						}
					}

				}
			}

		}

		if (asteroidHit > -1)
		{
			cannon.score += 1;
			skydrivers[asteroidHit] = CreateSkydiver(r,level);

		}


		if (cannon.cannonCD < 0){
			cannon.cannonCD = 0;
		}
		if (cannon.cannonCD > 0)
			cannon.cannonCD -= dT;
	}
	else{
		if(cannon.cannonCD!=CANNON_CD){
			if(cannon.cannonCD<=CANNON_CD)//����������� �������
				cannon.cannonCD += (dT/10);
			else{//�������� ������� ������ ������
				cannon.cannonCD = CANNON_CD;
			}
		}
	}
	if(cannon.health==0){
		cannon.destroy=true;
	}
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ��������� ��������� �� �������� ����.
//
//  WM_PAINT    - ��������� �������� ����
//  WM_DESTROY  - �������� ��������� � ������ � �������
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int savedDC;
	HDC backbuffDC;
	int width, height;
	int wmId, wmEvent; //!
	HBITMAP backbuffer;
	HBRUSH hBrush, hOldBrush;
	TCHAR greeting[] = _T("Hello, World!");
	int xPos, yPos;
	double dx,dy;
	HPEN hPen, hOldPen;
	double alpha,pieX,pieY;
	int r;
	double sinA;// ����� ���� �������� �����
	double cosA;// ������� ���� ��������

	switch (message)
	{
		//��������� ���� 
	case WM_CREATE: //!
		hBitmap = (HBITMAP)LoadImage(hInst, L"1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 'A':  //�������  ������ �����
			cannon.a = 0;
			break;
		case 'D':
			cannon.a = 0; //������� ������ ������
			break;
		case ' ':
			cannon.triggerOn = false; //�������
			break;
		case VK_F5: //���������� ������ � ����
			Save(hWnd);
			break;
		case VK_F8: // ���������� ������ �� �����
			Load(hWnd);
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'A':
			cannon.a = -1 * CANNON_A;
			break;
		case 'D':
			cannon.a = CANNON_A;
			break;
		case ' ':
			cannon.triggerOn = true;
			break;
		}
		InvalidateRect(hWnd,NULL,1);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		RECT rect;
		BITMAP bitmap;//!
		HDC hdcMem;//!
		HGDIOBJ oldBitmap; //!
		GetClientRect(hWnd, &rect);
		width=rect.right;
		height=rect.bottom;

		backbuffDC = CreateCompatibleDC(hdc);

		backbuffer = CreateCompatibleBitmap( hdc, width, height);

		savedDC = SaveDC(backbuffDC);

		SelectObject( backbuffDC, backbuffer );

		HPEN hPenOld;

		//������� ����
		hBrush = CreateSolidBrush(RGB(255,255,255));
		FillRect(backbuffDC,&rect,hBrush);
		DeleteObject(hBrush);
		hdcMem = CreateCompatibleDC(hdc);//!
		oldBitmap = SelectObject(hdcMem, hBitmap);//!
		//��������� ����
		GetObject(hBitmap, sizeof(bitmap), &bitmap);//!
		BitBlt(backbuffDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);//!
		//Rectangle(backbuffDC,0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		//��������� ������������
		POINT skydraw[3];//������ � ������������ ��� ��������� ������������ �����������
		for (int i=0;i<SKYDIVER_COUNT;i++)
		{
			if(skydrivers[i].status==false){//���� ���������� �����, �� ��������� ������������
				hPen = CreatePen(PS_SOLID, 1, RGB(248,1243,43));
				hOldPen = (HPEN)SelectObject(backbuffDC,hPen);
				hBrush = CreateSolidBrush(RGB(248,1243,43));
				hOldBrush = (HBRUSH)SelectObject(backbuffDC,hBrush);
				Chord(backbuffDC,skydrivers[i].X-skydrivers[i].r, skydrivers[i].Y-skydrivers[i].r, skydrivers[i].X +skydrivers[i].r, 
					skydrivers[i].Y+skydrivers[i].r,skydrivers[i].X+skydrivers[i].r, skydrivers[i].Y ,skydrivers[i].X-skydrivers[i].r, skydrivers[i].Y);//������������ ������ ����� �����
				SelectObject(backbuffDC,hOldPen); 
				DeleteObject(hPen);
				SelectObject(backbuffDC,hOldBrush); 
				DeleteObject(hBrush);



				hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
				hOldPen = (HPEN)SelectObject(backbuffDC,hPen); // �������� �������
				hBrush = CreateSolidBrush(RGB(0,0,0));// �������� ����� 
				hOldBrush = (HBRUSH)SelectObject(backbuffDC,hBrush);
				Ellipse( backbuffDC, skydrivers[i].X-7,skydrivers[i].Y+skydrivers[i].r-7, skydrivers[i].X+7,skydrivers[i].Y+skydrivers[i].r+7);
				SelectObject(backbuffDC,hOldPen); 
				DeleteObject(hPen);
				SelectObject(backbuffDC,hOldBrush); 
				DeleteObject(hBrush);


				//������ �����������
				hPen = CreatePen(PS_SOLID, 1, RGB(150,75,0));
				hOldPen = (HPEN)SelectObject(backbuffDC,hPen);
				hBrush = CreateSolidBrush(RGB(150,75,0));
				hOldBrush = (HBRUSH)SelectObject(backbuffDC,hBrush);
				skydraw[0].x = skydrivers[i].X-skydrivers[i].r;//���������� ��������� ������������
				skydraw[0].y = skydrivers[i].Y;
				skydraw[1].x = skydrivers[i].X+skydrivers[i].r;
				skydraw[1].y = skydrivers[i].Y;
				skydraw[2].x = skydrivers[i].X;
				skydraw[2].y = skydrivers[i].Y+skydrivers[i].r;

				Polygon(backbuffDC,skydraw,3);
				SelectObject(backbuffDC,hOldPen); 
				DeleteObject(hPen);
				SelectObject(backbuffDC,hOldBrush); 
				DeleteObject(hBrush);

			}
			else{
				hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
				hOldPen = (HPEN)SelectObject(backbuffDC,hPen);
				hBrush = CreateSolidBrush(RGB(0,0,0));
				hOldBrush = (HBRUSH)SelectObject(backbuffDC,hBrush);
				Ellipse(backbuffDC,skydrivers[i].X-skydrivers[i].r, skydrivers[i].Y-skydrivers[i].r,
					skydrivers[i].X+skydrivers[i].r,skydrivers[i].Y+skydrivers[i].r);
				SelectObject(backbuffDC,hOldPen); 
				DeleteObject(hPen);
				SelectObject(backbuffDC,hOldBrush); 
				DeleteObject(hBrush);
			}
		}

		//��������� ������
		cosA = cos(PI*cannon.alpha/180.0);
		sinA = sin(PI*cannon.alpha/180.0);

		if (cannon.cannonCD > 0&&cannon.triggerOn)
		{
			r = (double)cannon.cannonCD / (double)CANNON_CD * 255;
			hPen = CreatePen(PS_SOLID, 4, RGB(r,255-r,255-r));
			hOldPen = (HPEN)SelectObject(backbuffDC,hPen);

			MoveToEx(backbuffDC,cannon.X,CANNON_POSITION,NULL);
			LineTo(backbuffDC,cannon.traceX, cannon.traceEnd);

			SelectObject(backbuffDC,hOldPen);
			DeleteObject(hPen);
		}
		//��������� �����
		hPen = CreatePen(PS_SOLID, 1, RGB(128,128,128));

		hOldPen = (HPEN)SelectObject(backbuffDC,hPen);
		hBrush = CreateSolidBrush(RGB(188,188,188));
		hOldBrush = (HBRUSH)SelectObject(backbuffDC,hBrush);
		Ellipse(backbuffDC,cannon.X-CANNON_SIZE,CANNON_POSITION-CANNON_SIZE,cannon.X+CANNON_SIZE,CANNON_POSITION+CANNON_SIZE);

		SelectObject(backbuffDC,hOldPen);
		DeleteObject(hPen);
		SelectObject(backbuffDC,hOldBrush);
		DeleteObject(hBrush);


		// ��������� ������ ������
		hPen = CreatePen(PS_SOLID, 10, RGB(0,0,0));
		hOldPen = (HPEN)SelectObject(backbuffDC,hPen);
		MoveToEx(backbuffDC, cannon.X,CANNON_POSITION ,NULL); 
		LineTo(backbuffDC,cosA*(CANNON_SIZE+10)+cannon.X,(CANNON_POSITION)- sinA*(CANNON_SIZE+10));
		SelectObject(backbuffDC,hOldPen); 
		DeleteObject(hPen);

		//��������� ���������
		hBrush = CreateSolidBrush(RGB(210,180,140));
		hOldPen = (HPEN)SelectObject(backbuffDC,hBrush);
		Rectangle(backbuffDC,0, CANNON_POSITION, WINDOW_WIDTH, WINDOW_HEIGHT);
		SelectObject(backbuffDC,hOldPen); 
		DeleteObject(hBrush);

		WCHAR s[1000];

		//����� ��������� ���������� �� �����

		wsprintf(s,_T("v = %d"),(int)cannon.v);
		TextOut(backbuffDC,680,15,s,wcslen(s));

		wsprintf(s,_T("CD = %d"),(int)cannon.cannonCD);
		TextOut(backbuffDC,680,35,s,wcslen(s));

		wsprintf(s,_T("SCORE = %d"),(int)cannon.score);
		TextOut(backbuffDC,680,55,s,wcslen(s));
		wsprintf(s,_T("LEVEL = %d"),(int)level);
		TextOut(backbuffDC,680,75,s,wcslen(s));

		wsprintf(s,_T("LIVE = %d"),(int)cannon.health);
		TextOut(backbuffDC,680,95,s,wcslen(s));


		// End application-specific layout section.

		BitBlt(hdc,0,0,width,height,backbuffDC,0,0,SRCCOPY);
		RestoreDC(backbuffDC,savedDC);

		DeleteObject(backbuffer);
		DeleteDC(backbuffDC);
		SelectObject(hdcMem, oldBitmap);//!
		DeleteDC(hdcMem);//!

		//���� �����������
		if(c==1){
			if(MessageBox(hWnd,_T("��� ������������ ���� Paratrooper! �� ������ ������?"), _T("����� ����������!"), IDOK)==IDOK)
			{
				FlagStop=true;
				c=0;
			}
			else
			{
				exit(1);
			}
		}

		// ��������� � ����� ����
		if(cannon.destroy ==true){
			if(MessageBox(hWnd,_T("������� ����������� �����?"), _T("�� ���������."), IDOK)==IDOK){
				FlagStop=true;
				StartGame();//������ ����

			}
			else{
				FlagStop=true;
				exit(1);//�����
			}

		}

		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND:
		return 1;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}