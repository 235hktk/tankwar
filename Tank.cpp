// tank.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <TIME.H>
#include <mmsystem.h> //��������ͷ�ļ���   
#pragma comment(lib,"winmm.lib")//�������������ӿ� 

#define MAX_LOADSTRING 100

// Global Variables:


HINSTANCE hInst;								// current instance
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

HDC hdc, mhdc, mmhdc;        //��ͼ
int pre_Time, now_Time;      //��Ϣѭ��
int blood[10]={1,1,1,2,2,2,3,3,4,4};    //������еõ����˵�Ѫ�����ֵ���
HBITMAP myTank_bmp[2][4][2]; //�ҵ�̹��ͼ
HBITMAP enemy_bmp[4][4][2];  //����̹��ͼ
HBITMAP background[2];       //����ͼ
HBITMAP bullet_bmp[2];       //�ӵ�ͼ
HBITMAP wall_bmp[11];		 //ǽͼ

HBITMAP gameover_bmp[2];     //gameoverͼ
HBITMAP win_bmp[2];          //winͼ
HBITMAP number_bmp[10];      //numberͼ
HBITMAP light_bmp[4];        //�����ʱ����ǹ�
HBITMAP no_hurt_bmp[2][2];   //�޵�״̬ͼ
HBITMAP level_bmp[4];        //�ؿ�ͼ
int key_pre_time[2], key_now_time[2];  //����ʱ��
int start;                   //ҳ��id
HBITMAP page[10];            //ҳ��ͼ
HBITMAP T[2];                //ҳ��̹��ָ��ͼ
HBITMAP fire[3][2];			 //̹�˱�ը���ӵ���ǽ�Ļ���
HBITMAP prop_bmp[7];		 //����ͼ
int timing;					 //���ʱ�侲ֹ�����ֹͣ��ʱ��
int wall_time;				 //�����Χǽ����Χǽ��ʱ��
int start_time;				 //������Ϸ��һ��ʱ�䲻�ܻ
int end_time;				 //��ͨ����һ�غ�һ��ʱ���Ž�����һ��
bool lv_up[2];				 //�Ƿ�������������Ϊtrue������Ϊfalse
bool win;					 //�Ƿ�ͨ�أ�trueΪͨ�أ�falseΪû��
bool gameover;				 //�Ƿ������trueΪ�䣬falseΪû����

	//0����ͨ��  1�����ף�  2����ǿ�ӵ��� 3����ը���� 
//4����ǽ  5��Ѫƿ 6����ʱ
struct Tank{
	int x, y, d;                   //̹������ͷ���
	int pre_time, now_time;
	int tkind;                     //̹������
	int bkind;				       //bkind �ӵ�����
	int bullet_pre_time, bullet_now_time;//���������ӵ���ʱ����
	int no_hurt_time, no_hurt_num; //�޵�ʱ����
	int prop_time;			       //���߳���ʱ��
	int light_num, num;            //�޵�״̬
	int blood;					   //Ѫ
	Tank(){
		no_hurt_num=no_hurt_time=light_num=prop_time=0;num=1;
	}
}myTank[2], enemy[5];

int life_num[2];				   //̹�˵���
int lv_num[2];					   //�ȼ�
int fight_num[2];				   //ɱ����
int enemy_num[12];				   //ʣ�������

struct bullet{
	int x, y, d;				   //�ӵ�����ͷ���
	int f;						   //�Ƿ���ڣ�1���ڣ�0������
	int belong;					   //�ӵ�����̹�ˣ�<=1�����ҷ�̹�ˣ�>1Ϊ����̹��
	int kind;					   //�ӵ�����
	int fire_num;				   //��ײǽ����ߴ�̹��ʱ�������ʱ��
	bullet(){
		f=0;fire_num=0;
	}
}bullet[500];

struct Prop{
	int show_time;				   //������ʾ������ʱ��
	int pre_time, now_time;		   //���߳���ʱ����
	int kind;					   //���ߵ�����
	int x, y;					   //���߳��ֵ�����
}prop;

int map[31][50];				   //��ͼ��0���յأ�1��2����ǽ��3����ǽ��4���ݵأ�5����

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void draw_myTank();						  //���ҵ�̹��
void draw_enemy();						  //������
void draw_light();						  //��̹�˳���ʱ���ǹ�
void draw_no_hurt(int x,int y,int id);    //���޵�״̬���ڣ�x��y�����ֱ��Ϊid��̹��
void draw_bullet();						  //���ӵ�
void draw_fire(int x,int y,int kind);     //������,x��yΪ��������,kindΪ��������
void draw_prop();						  //������
void draw_wall();						  //��ǽ
void draw_num();						  //�������ұ�������Ϣ

void create_bullet(int d,int id,int kind);//�����ڵ�,dΪ�����ڵ��ķ���,idΪ�����ڵ���̹�ˣ�kindΪ�����ڵ�������
void move_myTank(int d,int id);		      //�ƶ��ҵ�̹��,dΪ�ƶ��ķ���,idΪ�ƶ�̹�˵ı��
void move_enemy();						  //�����ƶ�
BOOL meet(int x,int y,int d,int id);	  //�ж���ײ,x��yΪ����,dΪ����,idΪ̹�˱��
void bullet_hit();						  //�ӵ�������
void remove_wall(int x,int y);			  //���ӵ�Ϊ��ը��ʱ��x,y���긽����ǽ�ƻ���
void key_down(int id);		              //ģ�ⰴ������,idΪ�ҵ�̹�˱��
void get_prop(int id,int x,int y);		  //��õ��ߣ�idΪ��õ���̹�˵ı�ţ��ڣ�x��y�������
void init();							  //ÿ�س�ʼ��
void open_map(int id);					  //���ص�ͼ��idΪ�ؿ����
void myPaint();							  //����ͼ�������ڴ˺�����


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDC_TANK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TANK);

	// Main message loop:
	while (msg.message!=WM_QUIT) 
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			if(start>5&&start_time<0){							//�ж��Ƿ������Ϸҳ��;�ֹʱ���Ƿ����
				key_now_time[0]=key_now_time[1]=GetTickCount();
				if(key_now_time[0]-key_pre_time[0]>=30&&myTank[0].blood&&life_num[0]>0){//̹��1�İ���
					key_down(0);
				}
				if(start>=9&&key_now_time[1]-key_pre_time[1]>=30&&myTank[1].blood&&life_num[1]>0){//̹��2�İ���
					key_down(1);
				}
				if(life_num[0]<=0&&life_num[1]<=0) gameover=true;//������̹�˵�Ѫ��С�ڵ���0��gameover
			}
			now_Time=GetTickCount();
			if(now_Time-pre_Time>=30){
				myPaint();										 //��ͼ
			}
		}
	}
	
	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TANK);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_TANK;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, "̹�˴�ս", WS_OVERLAPPEDWINDOW,
      100, 40, 1150, 680, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   hdc=GetDC(hWnd);
   mhdc=CreateCompatibleDC(hdc);
   mmhdc=CreateCompatibleDC(hdc);
   HBITMAP bmp=CreateCompatibleBitmap(hdc,1300,680);//1015 680
   SelectObject(mhdc,bmp);
/**********************************************************************************************/
/***********************************����Ϊ����λͼ*********************************************/
/**********************************************************************************************/
   char str[100];                               
   int i, j, k;
   for(i=0;i<2;i++){								//�����ҵ�̹��λͼ
	   for(j=0;j<4;j++){
		   for(k=0;k<2;k++){
				sprintf(str,"Image\\MyTank\\myTank%d%d%d.bmp",i,j,k);
				myTank_bmp[i][j][k]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,40,40,LR_LOADFROMFILE);
		   }
		  
		   
	   }
   }
   for(i=0;i<4;i++){							     //���ص���λͼ
	   for(j=0;j<4;j++){
		   for(k=0;k<2;k++){
			   sprintf(str,"Image\\Enemy\\enemy%d%d%d.bmp",i,j,k);
			   enemy_bmp[i][j][k]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,40,40,LR_LOADFROMFILE);
		   }
	   }
   }

   for(i=0;i<2;i++){                                 //�����ӵ�λͼ
		sprintf(str,"Image\\Bullet\\bullet%d.bmp",i);
		bullet_bmp[i]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,10,10,LR_LOADFROMFILE);
   }
		
   for(i=1;i<=10;i++){
		sprintf(str,"Image\\Wall\\wall%d.bmp",i);
		wall_bmp[i]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,20,20,LR_LOADFROMFILE);//����ǽ��λͼ
   }
   for(i=0;i<=5;i++){
	   if(i<=3)
		sprintf(str,"Image\\page%d.bmp",0);
	   else sprintf(str,"Image\\page%d.bmp",i);
		page[i]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,1150,680,LR_LOADFROMFILE);//����ҳ��
   }
   int x, y;
   for(i=0;i<3;i++){							
	   for(j=0;j<2;j++){
		sprintf(str,"Image\\Fire\\fire%d%d.bmp",i,j);
		if(i==0) x=30,y=30;
		else x=100,y=100;
		fire[i][j]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,x,y,LR_LOADFROMFILE);//���ػ���
	   }
   }
   for(i=1;i<=6;i++){
	   sprintf(str,"Image\\Prop\\prop_bmp%d.bmp",i);
	   prop_bmp[i]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,40,40,LR_LOADFROMFILE);//���ص���
   }
   for(i=0;i<2;i++){
	   sprintf(str,"Image\\gameover%d.bmp",i);
	   gameover_bmp[i]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,600,300,LR_LOADFROMFILE);//����gameover
   }
   for(i=0;i<2;i++){
	   sprintf(str,"Image\\win%d.bmp",i);
	   win_bmp[i]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,600,300,LR_LOADFROMFILE);//����win
   }
   for(i=0;i<10;i++){
		   sprintf(str,"Image\\Number\\num%d.bmp",i);
		   number_bmp[i]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,30,30,LR_LOADFROMFILE);//��������
   }
   for(i=1;i<=3;i++){
	   sprintf(str,"Image\\level%d.bmp",i);
	   level_bmp[i]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,50,20,LR_LOADFROMFILE);//���عؿ���Ϣ
   }
   for(i=0;i<4;i++){
	   sprintf(str,"Image\\Light\\light%d.bmp",i);
	   light_bmp[i]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,40,40,LR_LOADFROMFILE);//�����ǹ�
   }
   for(i=0;i<2;i++){
	   for(j=0;j<2;j++){
		   sprintf(str,"Image\\No_hurt\\no_hurt%d%d.bmp",i,j);
		   no_hurt_bmp[i][j]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,40,40,LR_LOADFROMFILE);//�����޵й⻷
	   }
   }
   for(i=0;i<2;i++){									//����ҳ��1��̹��ָ��
	    sprintf(str,"Image\\T%d.bmp",i);
		T[i]=(HBITMAP)LoadImage(NULL,str,IMAGE_BITMAP,40,40,LR_LOADFROMFILE);
   }
   start=0;
	
   background[0]=(HBITMAP)LoadImage(NULL,"Image\\bj0.bmp",IMAGE_BITMAP,1015,680,LR_LOADFROMFILE);//���ر���
   background[1]=(HBITMAP)LoadImage(NULL,"Image\\bj1.bmp",IMAGE_BITMAP,130,680,LR_LOADFROMFILE);

   	srand(time(NULL));
	for(i=0;i<2;i++){
		lv_num[i]=1;
		life_num[i]=3;
	}
	

   
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
			
	switch (message) 
	{
	case WM_KEYDOWN:
		switch(wParam){
		case VK_ESCAPE:
			if(start<=3)
			PostQuitMessage(0);
			else start=0;
			break;
		case VK_UP:
			if(start<=3){
				start--;
				if(start<0) start=3;
			}
			break;
		case VK_DOWN:
			if(start<=3){
				start++;
				if(start>3) start=0;
			}
			break;
		case VK_RETURN:
			if(start==0) start=6;
			else if(start==1) start=9;
			else if(start==2) start=4;
			else if(start==3) start=5;
			if(start==6||start==9){
				int i;
				for(i=0;i<2;i++){
					lv_num[i]=1;
					life_num[i]=3;
				}
			}
			init();
			break;
		case VK_PRIOR:
			if(!gameover&&!win){
				if(start>=6&&start<=8){
					start--;
					if(start<6) start=8;
				}
				else if(start>=9&&start<=11){
					start--;
					if(start<9) start=11;
				}
				init();
			}
			break;
		case VK_NEXT:
			if(!gameover&&!win){
				if(start>=6&&start<=8){
					start++;
					if(start>8) start=6;
				}
				else if(start>=9&&start<=11){
					start++;
					if(start>11) start=9;
				}
				init();
			}
			break;
		}
		break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			
			myPaint();
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

void open_map(int id){
	FILE *fp;
	char str[20];
	sprintf(str,"Map\\map%d.txt",id);
	char *filename = str;
	fp = fopen(filename,"rt");
	int i, j;
	for(i = 0;i < 31;i++) {
		for(j = 0;j < 50;j++) 
			fscanf(fp,"%d",&map[i][j]);
	}
	fclose(fp);
}

void init(){
	int i;
	if(start==0)									//���ص�ͼ
	open_map(1);
	else if(start>=6&&start<=8) open_map(start-5);
	else if(start>=9) open_map(start-8);

	for(i=0;i<2;i++) {								//���¶�Ϊ��ʼ��Ϣ
		fight_num[i]=0;
		lv_up[i]=true;
		if(start<9&&i==1){
			lv_num[i]=0;
			life_num[i]=0;
		}
		myTank[i].no_hurt_time=100;
	}
	for(i=6;i<=11;i++) enemy_num[i]=20;
	
	myTank[0].x=360;myTank[0].y=580;
	myTank[0].d=0;myTank[0].blood=lv_num[0];
	if(start>=9){
		myTank[1].x=600;myTank[1].y=580;
		myTank[1].d=0;myTank[1].blood=lv_num[1];
	}
	for(i=0;i<2;i++)
		myTank[i].bullet_pre_time=GetTickCount(),myTank[i].bkind=0;
	for(i=0;i<5;i++){
		if(i==0) enemy[i].x=0;
		else if(i==1) enemy[i].x=300;
		else if(i==2) enemy[i].x=500;
		else if(i==3) enemy[i].x=700;
		else enemy[i].x=960;
		enemy[i].y=5;
		enemy[i].d=2;
		enemy[i].blood=1;
		enemy[i].bkind=0;
		enemy[i].tkind=0;
		enemy[i].no_hurt_time=100;
		enemy[i].bullet_pre_time=GetTickCount();
	}
	key_pre_time[0]=key_pre_time[1]=GetTickCount();
	wall_time=timing=0;
	start_time=100;
	for(i=0;i<500;i++) bullet[i].f=0;
	gameover=false;
	win=false;
	end_time=-1;
}

void key_down(int belong){
	if(gameover||win) return;
	if(belong==0){
		if(::GetKeyState(87)<0){
			myTank[0].d=0;
			if(meet(myTank[0].x,myTank[0].y,0,0))
				move_myTank(0,0);
		}
		else if(::GetKeyState(68)<0){
			myTank[0].d=1;
			if(meet(myTank[0].x,myTank[0].y,1,0))
				move_myTank(1,0);
		}
		else if(::GetKeyState(83)<0){
			myTank[0].d=2;
			if(meet(myTank[0].x,myTank[0].y,2,0))
				move_myTank(2,0);
		}
		else if(::GetKeyState(65)<0){
			myTank[0].d=3;
			if(meet(myTank[0].x,myTank[0].y,3,0))
				move_myTank(3,0);
		}
		if(::GetKeyState(74)<0){
			myTank[0].bullet_now_time=GetTickCount();
			if(myTank[0].bullet_now_time-myTank[0].bullet_pre_time>=400){
				create_bullet(myTank[0].d,0,myTank[0].bkind);
				mciSendString("play Sound\\Gunfire.wav", NULL, 0, 0);   
			}
		}
		key_pre_time[0]=GetTickCount();
	}
	else{
		if(::GetKeyState(VK_UP)<0){
			myTank[1].d=0;
			if(meet(myTank[1].x,myTank[1].y,0,0))
				move_myTank(0,1);
		}
		else if(::GetKeyState(VK_RIGHT)<0){
			myTank[1].d=1;
			if(meet(myTank[1].x,myTank[1].y,1,0))
				move_myTank(1,1);
		}
		else if(::GetKeyState(VK_DOWN)<0){
			myTank[1].d=2;
			if(meet(myTank[1].x,myTank[1].y,2,0))
				move_myTank(2,1);
		}
		else if(::GetKeyState(VK_LEFT)<0){
			myTank[1].d=3;
			if(meet(myTank[1].x,myTank[1].y,3,0))
				move_myTank(3,1);
		}
		if(::GetKeyState(96)<0||::GetKeyState(48)<0){
			myTank[1].bullet_now_time=GetTickCount();
			if(myTank[1].bullet_now_time-myTank[1].bullet_pre_time>=400){
				create_bullet(myTank[1].d,1,myTank[1].bkind);
				mciSendString("play Sound\\Gunfire.wav", NULL, 0, 0);  
			}
		}
		key_pre_time[1]=GetTickCount();
	}
}


void create_bullet(int d,int id,int kind){
	int i;

	for(i=0;i<500;i++){
		if(!bullet[i].f){
			
			bullet[i].f=1;
			bullet[i].belong=id;
			bullet[i].kind=kind;
			if(id<=1){							//�ҵ�̹��
				if(d==0){						//����
					bullet[i].x=myTank[id].x+15;bullet[i].y=myTank[id].y;
				}
				else if(d==1){					//����
					bullet[i].x=myTank[id].x+20;bullet[i].y=myTank[id].y+15;
				}
				else if(d==2){					//����
					bullet[i].x=myTank[id].x+15;bullet[i].y=myTank[id].y+20;
				}
				else{							//����
					bullet[i].x=myTank[id].x;bullet[i].y=myTank[id].y+15;
				}
				bullet[i].d=myTank[id].d;
				myTank[id].bullet_pre_time=GetTickCount();
			}
			else{								//����
				if(d==0){
					bullet[i].x=enemy[id-2].x+15;bullet[i].y=enemy[id-2].y;
				}
				else if(d==1){
					bullet[i].x=enemy[id-2].x+20;bullet[i].y=enemy[id-2].y+15;
				}
				else if(d==2){
					bullet[i].x=enemy[id-2].x+15;bullet[i].y=enemy[id-2].y+20;
				}
				else{
					bullet[i].x=enemy[id-2].x;bullet[i].y=enemy[id-2].y+15;
				}
				bullet[i].d=enemy[id-2].d;
				enemy[id-2].bullet_pre_time=GetTickCount();
			}
			break;
		}
	}
	
};


void draw_bullet(){
	int i;
	for(i=0;i<500;i++){
		if(bullet[i].f){
			if(bullet[i].d==0){
				if(bullet[i].y-10<=0) bullet[i].f=0;
				else bullet[i].y-=10;
			}
			else if(bullet[i].d==1){
				if(bullet[i].x+10>=1000) bullet[i].f=0;
				else bullet[i].x+=10;
			}
			else if(bullet[i].d==2){
				if(bullet[i].y+10>=620) bullet[i].f=0;
				else bullet[i].y+=10;
			}
			else if(bullet[i].d==3){
				if(bullet[i].x-10<=0) bullet[i].f=0;
				else bullet[i].x-=10;
			}
			if(bullet[i].f){
				
				SelectObject(mmhdc,bullet_bmp[0]);
				BitBlt(mhdc,bullet[i].x,bullet[i].y,10,10,mmhdc,0,0,SRCAND);
				SelectObject(mmhdc,bullet_bmp[1]);
				BitBlt(mhdc,bullet[i].x,bullet[i].y,10,10,mmhdc,0,0,SRCPAINT);
			}
		}
	}
}

void move_enemy(){
	
	int i;
	for(i=0;i<5;i++){
		if(enemy[i].blood<=0) continue;
		int r=rand()%200;				//��������ƶ�
		int r1=rand()%40;				//������������ӵ�
		enemy[i].bullet_now_time=GetTickCount();
		if(r1<5&&enemy[i].bullet_now_time-enemy[i].bullet_pre_time>=400){//���˿��Է����ӵ�
			create_bullet(enemy[i].d,i+2,enemy[i].bkind);
		}
		//����
		if(r==0) {						//����
			if(meet(enemy[i].x,enemy[i].y,0,i+2)){
				enemy[i].y-=5;
				enemy[i].d=0;
			}
		}
		else if(r==1){					//����
			if(meet(enemy[i].x,enemy[i].y,1,i+2)){
				enemy[i].x+=5;
				enemy[i].d=1;
			}
		}
		else if(r==2){					//����
			if(meet(enemy[i].x,enemy[i].y,2,i+2)){
				enemy[i].y+=5;
				enemy[i].d=2;
			}
		}
		else if(r==3){					//����
			if(meet(enemy[i].x,enemy[i].y,3,i+2)){
				enemy[i].x-=5;
				enemy[i].d=3;
			}
		}
		else {							//��ǰ�߲�����
			if(meet(enemy[i].x,enemy[i].y,enemy[i].d,i+2)){//��������
					if(enemy[i].d==0)
						enemy[i].y-=5;
					else if(enemy[i].d==1)
						enemy[i].x+=5;
					else if(enemy[i].d==2)
						enemy[i].y+=5;
					else if(enemy[i].d==3)
						enemy[i].x-=5;
			}
			else{						//����������Ҫô���䣬Ҫô�����ӵ��ƻ�ǽ������
				enemy[i].bullet_now_time=GetTickCount();
				if(enemy[i].bullet_now_time-enemy[i].bullet_pre_time>=400){
					create_bullet(enemy[i].d,i+2,enemy[i].bkind);
				}
				else{
					r1=rand()%4;
					enemy[i].d=r1;
				}
			}
		}
		get_prop(i+2,enemy[i].x,enemy[i].y);//��õ���
	}
}

void move_myTank(int d,int belong){
	if(d==0){					 //UP
		myTank[belong].y-=5;
		myTank[belong].d=0;
	}
	else if(d==1){				//RIGHT
		myTank[belong].x+=5;
		myTank[belong].d=1;
	}
	else if(d==2){				//DOWN
		myTank[belong].y+=5;
		myTank[belong].d=2;
	}
	else if(d==3){				//LEFT
		myTank[belong].x-=5;
		myTank[belong].d=3;
	}
	get_prop(belong,myTank[belong].x,myTank[belong].y);//��õ���
}

void draw_light(int x,int y,int id){
	if(id<=1){					//���ҵ�̹�˸�����ǹ�
		if(myTank[id].light_num+myTank[id].num>3){
			myTank[id].num=-1;
		}
		else if(myTank[id].light_num+myTank[id].num<0){
			myTank[id].num=1;
		}
		myTank[id].light_num=myTank[id].light_num+myTank[id].num;
		SelectObject(mmhdc,light_bmp[myTank[id].light_num]);
		BitBlt(mhdc,x,y,40,40,mmhdc,0,0,SRCCOPY);
	}
	else {						//�����˸�����ǹ�
		id-=2;
		if(enemy[id].light_num+enemy[id].num>3){
			enemy[id].num=-1;
		}
		else if(enemy[id].light_num+enemy[id].num<0){
			enemy[id].num=1;
		}
		enemy[id].light_num=enemy[id].light_num+enemy[id].num;
		SelectObject(mmhdc,light_bmp[enemy[id].light_num]);
		BitBlt(mhdc,x,y,40,40,mmhdc,0,0,SRCCOPY);
	}
}

void draw_no_hurt(int x,int y,int id){
	if(id<=1){						//���ҵ�̹���޵й⻷
		if(!myTank[id].no_hurt_num){
			myTank[id].no_hurt_num=1;
		}
		else myTank[id].no_hurt_num=0;
		SelectObject(mmhdc,no_hurt_bmp[myTank[id].no_hurt_num][0]);
		BitBlt(mhdc,x,y,40,40,mmhdc,0,0,SRCAND);
		SelectObject(mmhdc,no_hurt_bmp[myTank[id].no_hurt_num][1]);
		BitBlt(mhdc,x,y,40,40,mmhdc,0,0,SRCPAINT);
	}
	else {							//�������޵й⻷
		id-=2;
		if(!enemy[id].no_hurt_num){
			enemy[id].no_hurt_num=1;
		}
		else enemy[id].no_hurt_num=0;
		SelectObject(mmhdc,no_hurt_bmp[enemy[id].no_hurt_num][0]);
		BitBlt(mhdc,x,y,40,40,mmhdc,0,0,SRCAND);
		SelectObject(mmhdc,no_hurt_bmp[enemy[id].no_hurt_num][1]);
		BitBlt(mhdc,x,y,40,40,mmhdc,0,0,SRCPAINT);
	}
}

void draw_num(){
	int x=-1, y=-1;
	x=enemy_num[start]/10;
	y=enemy_num[start]%10;
	if(x!=0){
		SelectObject(mmhdc,number_bmp[x]);
		BitBlt(mhdc,1035,45,30,30,mmhdc,0,0,SRCCOPY);
	}
	SelectObject(mmhdc,number_bmp[y]);
	BitBlt(mhdc,1065,45,30,30,mmhdc,0,0,SRCCOPY);

	x=life_num[0]/10;
	y=life_num[0]%10;
	if(x!=0){
		SelectObject(mmhdc,number_bmp[x]);
		BitBlt(mhdc,1035,145,30,30,mmhdc,0,0,SRCCOPY);
	}
	SelectObject(mmhdc,number_bmp[y]);
	BitBlt(mhdc,1065,145,30,30,mmhdc,0,0,SRCCOPY);

	x=lv_num[0]/10;
	y=lv_num[0]%10;
	if(x!=0){
		SelectObject(mmhdc,number_bmp[x]);
		BitBlt(mhdc,1035,215,30,30,mmhdc,0,0,SRCCOPY);
	}
	SelectObject(mmhdc,number_bmp[y]);
	BitBlt(mhdc,1065,215,30,30,mmhdc,0,0,SRCCOPY);

	x=fight_num[0]/10;
	y=fight_num[0]%10;
	if(x!=0){
		SelectObject(mmhdc,number_bmp[x]);
		BitBlt(mhdc,1035,285,30,30,mmhdc,0,0,SRCCOPY);
	}
	SelectObject(mmhdc,number_bmp[y]);
	BitBlt(mhdc,1065,285,30,30,mmhdc,0,0,SRCCOPY);

	x=life_num[1]/10;
	y=life_num[1]%10;
	if(x!=0){
		SelectObject(mmhdc,number_bmp[x]);
		BitBlt(mhdc,1035,405,30,30,mmhdc,0,0,SRCCOPY);
	}
	SelectObject(mmhdc,number_bmp[y]);
	BitBlt(mhdc,1065,405,30,30,mmhdc,0,0,SRCCOPY);

	x=lv_num[1]/10;
	y=lv_num[1]%10;
	if(x!=0){
		SelectObject(mmhdc,number_bmp[x]);
		BitBlt(mhdc,1035,485,30,30,mmhdc,0,0,SRCCOPY);
	}
	SelectObject(mmhdc,number_bmp[y]);
	BitBlt(mhdc,1065,485,30,30,mmhdc,0,0,SRCCOPY);

	x=fight_num[1]/10;
	y=fight_num[1]%10;
	if(x!=0){
		SelectObject(mmhdc,number_bmp[x]);
		BitBlt(mhdc,1035,555,30,30,mmhdc,0,0,SRCCOPY);
	}
	SelectObject(mmhdc,number_bmp[y]);
	BitBlt(mhdc,1065,555,30,30,mmhdc,0,0,SRCCOPY);

	if(start>=6&&start<=8) x=start-5;
	else if(start>=9&&start<=11) x=start-8;
	SelectObject(mmhdc,level_bmp[x]);
	BitBlt(mhdc,1040,595,50,20,mmhdc,0,0,SRCCOPY);
}

void draw_enemy(){
	
	int i, r;
	if(start_time>0) {				//����һ�غ���ǹ�
		for(i=0;i<5;i++) draw_light(enemy[i].x,enemy[i].y,i+2);	return;
	}
	int num=0;
	for(i=0;i<5;i++)				//��õ�ǰ��ͼ������Ŀ
		if(enemy[i].blood>0)
			num++;
	if(end_time==-1&&enemy_num[start]==0&&num==0){//��ǰ��ͼ������ĿΪ0��ʣ�������ĿΪ0����Խ�����һ��
			for(i=0;i<2;i++){		//���ҵ�̹����һ��û�ܵ��˺��Ϳ�������
				if(start<9&&i==1) continue;
				if(lv_up[i]) lv_num[i]++;
			}
			end_time=100;return;
	}
	for(i=0;i<5;i++){				
		if(enemy[i].prop_time>0){  //���˵���ʱ��
			enemy[i].prop_time--;
		}
		else enemy[i].bkind=0;	   //�����˵���ʱ���ѹ������ӵ���Ϊ��ʼ״̬
		if(enemy[i].no_hurt_time>0){//�����޵�ʱ��
			enemy[i].no_hurt_time--;
			draw_no_hurt(enemy[i].x,enemy[i].y,i+2);
		}
		if(enemy[i].blood<=0&&enemy_num[start]-num>0){//��������
			enemy[i].now_time=GetTickCount();
			if(enemy[i].now_time-enemy[i].pre_time>=1000&&enemy[i].now_time-enemy[i].pre_time<2000){//���˸����ǹ�
				if(i==0) enemy[i].x=0;
				else if(i==1) enemy[i].x=300;
				else if(i==2) enemy[i].x=500;
				else if(i==3) enemy[i].x=700;
				else enemy[i].x=960;
				enemy[i].y=5,enemy[i].d=2;
				draw_light(enemy[i].x,enemy[i].y,i+2);
			}

			if(enemy[i].now_time-enemy[i].pre_time>=2000){//���˿��Ը���
				if(start==6||start==9)	r=rand()%8;
				else if(start==7||start==10){
					r=rand()%10;
					if(r<=2) r=3;
					else if(r<=5) r=6;
					else r=9;
				}
				else if(start==8||start==11){
					r=9;
				}
				enemy[i].blood=blood[r];
				enemy[i].tkind=blood[r]-1;
				enemy[i].no_hurt_time=100;
			}
		}
		if(enemy[i].blood>0){
			SelectObject(mmhdc,enemy_bmp[enemy[i].tkind][enemy[i].d][0]);
			BitBlt(mhdc,enemy[i].x,enemy[i].y,40,40,mmhdc,0,0,SRCAND);
			SelectObject(mmhdc,enemy_bmp[enemy[i].tkind][enemy[i].d][1]);
			BitBlt(mhdc,enemy[i].x,enemy[i].y,40,40,mmhdc,0,0,SRCPAINT);
		}
	}
}

void draw_myTank(){
	int i;
	if(start_time>0) {				//�ؿ���ʼ���ǹ�
		for(i=0;i<2;i++){
			if(start<9&&i==1) continue;
			draw_light(myTank[i].x,myTank[i].y,i);	
		}
		return;
	}
	for(i=0;i<2;i++){
		if(i==1&&start<9) continue;
		if(life_num[i]<=0) continue;
		if(myTank[i].prop_time>0){	//����ʱ��
			myTank[i].prop_time--;
		}
		else myTank[i].bkind=0;		//������ʱ���ѹ������ӵ���Ϊ��ʼ��̬
		if(myTank[i].no_hurt_time>0){//�޵�ʱ��
			myTank[i].no_hurt_time--;
			draw_no_hurt(myTank[i].x,myTank[i].y,i);
		}
		if(myTank[i].blood<=0){
			myTank[i].now_time=GetTickCount();
			if(myTank[i].now_time-myTank[i].pre_time>=1000&&myTank[i].now_time-myTank[i].pre_time<2000){//�ҵ�̹�˸����ǹ�
				if(i==0) myTank[i].x=360;
				else myTank[i].x=600;
				myTank[i].y=580;
				draw_light(myTank[i].x,myTank[i].y,i);	
			}
			if(myTank[i].now_time-myTank[i].pre_time>=2000){//���Ը���
				myTank[i].d=0,myTank[i].blood=lv_num[i];
				myTank[i].no_hurt_time=100;
			}
		}
		if(myTank[i].blood){
			SelectObject(mmhdc,myTank_bmp[i][myTank[i].d][0]);
			BitBlt(mhdc,myTank[i].x,myTank[i].y,40,40,mmhdc,0,0,SRCAND);
			SelectObject(mmhdc,myTank_bmp[i][myTank[i].d][1]);
			BitBlt(mhdc,myTank[i].x,myTank[i].y,40,40,mmhdc,0,0,SRCPAINT);
		}
	}
}

void draw_wall(){                 
	int i, j;
	for(i=0;i<31;i++){
		for(j=0;j<50;j++){
			if(map[i][j]){
				if(map[i][j]==4){
					SelectObject(mmhdc,wall_bmp[6]);
					BitBlt(mhdc,j*20,i*20,20,20,mmhdc,0,0,SRCAND);
					SelectObject(mmhdc,wall_bmp[4]);
					BitBlt(mhdc,j*20,i*20,20,20,mmhdc,0,0,SRCPAINT);
				}
				else{	
					SelectObject(mmhdc,wall_bmp[map[i][j]]);
					BitBlt(mhdc,j*20,i*20,20,20,mmhdc,0,0,SRCCOPY);
				}
			}
		}
	}
}

BOOL meet(int x,int y,int d,int belong){
	int xx[]={30,31};
	int yy[]={24,25};
	//ײǽ
	if(d==0){ 
		if(y-5<0||map[(y-5)/20][(x+30)/20]>=1&&map[(y-5)/20][(x+30)/20]<=3||map[(y-5)/20][(x+30)/20]==5
			||map[(y-5)/20][(x+5)/20]>=1&&map[(y-5)/20][(x+5)/20]<=3||map[(y-5)/20][(x+5)/20]==5
			||map[(y-5)/20][(x+15)/20]>=1&&map[(y-5)/20][(x+15)/20]<=3||map[(y-5)/20][(x+15)/20]==5) return false;
	}
	else if(d==1){
		if(x+5>960||map[(y+30)/20][(x+35)/20]>=1&&map[(y+30)/20][(x+35)/20]<=3||map[(y+30)/20][(x+35)/20]==5
			||map[(y+5)/20][(x+35)/20]>=1&&map[(y+5)/20][(x+35)/20]<=3||map[(y+5)/20][(x+35)/20]==5
			||map[(y+15)/20][(x+35)/20]>=1&&map[(y+15)/20][(x+35)/20]<=3||map[(y+15)/20][(x+35)/20]==5) return false;
	}
	else if(d==2){
		if(y+5>590||map[(y+35)/20][(x+7)/20]>=1&&map[(y+35)/20][(x+7)/20]<=3||map[(y+35)/20][(x+7)/20]==5
			||map[(y+35)/20][(x+33)/20]>=1&&map[(y+35)/20][(x+33)/20]<=3||map[(y+35)/20][(x+33)/20]==5
			||map[(y+35)/20][(x+15)/20]>=1&&map[(y+35)/20][(x+15)/20]<=3||map[(y+35)/20][(x+15)/20]==5) return false;
	}
	else {
		if(x-5<0||map[(y+30)/20][(x)/20]>=1&&map[(y+30)/20][(x)/20]<=3||map[(y+30)/20][(x)/20]==5
			||map[(y+8)/20][(x)/20]>=1&&map[(y+8)/20][(x)/20]<=3||map[(y+8)/20][(x)/20]==5
			||map[(y+20)/20][(x)/20]>=1&&map[(y+20)/20][(x)/20]<=3||map[(y+20)/20][(x)/20]==5) return false;
	}
	//�ҵ�̹�˺͵�����ײ
	int i;
	if(belong<=1){
		if(d==0){
			for(i=0;i<5;i++){
				if(enemy[i].blood>0&&y-10>=enemy[i].y&&y-10<=enemy[i].y+30&&x>=enemy[i].x-35&&x<=enemy[i].x+30) return false;
			}
		}
		else if(d==1){
			for(i=0;i<5;i++){
				if(enemy[i].blood>0&&x+40>=enemy[i].x&&x+40<=enemy[i].x+40&&y+20>=enemy[i].y&&y<=enemy[i].y+20) return false;
			}
		}
		else if(d==2){
			for(i=0;i<5;i++){
				if(enemy[i].blood>0&&y+40>=enemy[i].y&&y<=enemy[i].y&&x+35>=enemy[i].x&&x<=enemy[i].x+35) return false;
			}
		}
		else {
			for(i=0;i<5;i++){
				if(enemy[i].blood>0&&x-10>=enemy[i].x&&x-10<=enemy[i].x+30&&y+20>=enemy[i].y&&y<=enemy[i].y+20) return false;
			}
		}
	}

	//���˺��ҵ�̹����ײ
	else{
		if(d==0){
			for(i=0;i<2;i++){
				if(i==1&&start<9) continue;
				if(myTank[i].blood&&y-10>=myTank[i].y&&y-10<=myTank[i].y+30&&x>=myTank[i].x-35&&x<=myTank[i].x+30) return false;
			}
		}
		else if(d==1){
			for(i=0;i<2;i++){
				if(i==1&&start<9) continue;
				if(myTank[i].blood&&x+40>=myTank[i].x&&x+40<=myTank[i].x+40&&y+20>=myTank[i].y&&y<=myTank[i].y+20) return false;
			}
		}
		else if(d==2){
			for(i=0;i<2;i++){
				if(i==1&&start<9) continue;
				if(myTank[i].blood&&y+40>=myTank[i].y&&y<=myTank[i].y&&x+35>=myTank[i].x&&x<=myTank[i].x+35) return false;
			}
		}
		else {
			for(i=0;i<2;i++){
				if(i==1&&start<9) continue;
				if(myTank[i].blood&&x-10>=myTank[i].x&&x-10<=myTank[i].x+30&&y+20>myTank[i].y&&y<=myTank[i].y+20) return false;
			}
		}
	}
	return true;
}

void draw_fire(int x,int y,int kind){
	int xx, yy;
	if(kind==0)
		x=x-10,y=y-10,xx=30,yy=30;
	else
		xx=100,yy=100;
	SelectObject(mmhdc,fire[kind][0]);
	BitBlt(mhdc,x,y,xx,yy,mmhdc,0,0,SRCAND);
	SelectObject(mmhdc,fire[kind][1]);
	BitBlt(mhdc,x,y,xx,yy,mmhdc,0,0,SRCPAINT);
}

void draw_prop(){
	if(wall_time>0)			
		wall_time--;
	else if(wall_time==0){
		map[30][23]=map[28][25]=map[28][23]=map[29][26]=2;//����Χǽʱ���ѹ�����ǽ�����ǽ
		map[29][23]=map[28][26]=map[28][24]=map[30][26]=1;
		wall_time=-1;
	}
	prop.now_time=GetTickCount();
	if(prop.now_time-prop.pre_time>=20000){				 //���߿��Գ���
		prop.show_time=300;
		prop.x=rand()%900;
		prop.y=rand()%400;
		prop.kind=(rand()%6)+1;
		prop.pre_time=prop.now_time;
	}

	if(prop.show_time>0){								//������ʱ��û���򻭵���
		prop.show_time--;
		SelectObject(mmhdc,prop_bmp[prop.kind]);
		BitBlt(mhdc,prop.x,prop.y,40,40,mmhdc,0,0,SRCCOPY);
	}
}


void get_prop(int id,int x,int y){
	if(prop.show_time<=0) return;
	if(id>1&&(prop.kind==6||prop.kind==4)) return;		//�����޷���õ���4�͵���6
	int i, j;
	if(x+40>=prop.x+5&&x<=prop.x+35&&y+40>=prop.y+5&&y<=prop.y+35){
		if(id<=1){										//�ҵ�̹�˵õ�����
			if(prop.kind<=3){
				myTank[id].bkind=prop.kind;
				myTank[id].prop_time=500;
			}
			else if(prop.kind==4){
				wall_time=300;
				map[30][23]=map[29][23]=map[28][23]=map[28][24]=3;
				map[28][25]=map[28][26]=map[29][26]=map[30][26]=3;
			}
			else if(prop.kind==5) life_num[id]++;
			else timing=500;
		}
		else {											//���˵õ�����
			if(prop.kind<=3){
				enemy[id-2].bkind=prop.kind;
				enemy[id-2].prop_time=300;
			}
			else if(prop.kind==5) enemy[id-2].blood++;
		}
		prop.show_time=0;
	}	
}

void remove_wall(int x,int y){
	if(map[x][y]<=2) map[x][y]=0;
	if(map[x+1][y-1]<=2) map[x+1][y-1]=0;
	if(map[x+1][y+1]<=2) map[x+1][y+1]=0;
	if(map[x-1][y+1]<=2) map[x-1][y+1]=0;
	if(map[x-1][y-1]<=2) map[x-1][y-1]=0;
	if(map[x][y-1]<=2) map[x][y-1]=0;
	if(map[x][y+1]<=2) map[x][y+1]=0;
	if(map[x-1][y]<=2) map[x-1][y]=0;
	if(map[x+1][y]<=2) map[x+1][y]=0;
}

void bullet_hit(){
	int i, j, k;
	int x, y;
	for(i=0;i<500;i++){						//�������
		if(bullet[i].fire_num>0){
			if(bullet[i].kind==0||bullet[i].kind==2){
				if(bullet[i].d==0) x=0,y=10;
				else if(bullet[i].d==1) x=-10,y=0;
				else if(bullet[i].d==2) x=0,y=-10;
				else x=10,y=0;
				draw_fire(bullet[i].x+x,bullet[i].y+y,0);
			}
			else if(bullet[i].kind==3){
				if(bullet[i].d==0) x=-45,y=-45;
				else if(bullet[i].d==1) x=-40,y=-40;
				else if(bullet[i].d==2) x=-40,y=-40;
				else x=-45,y=-50;
				draw_fire(bullet[i].x+x,bullet[i].y+y,1);
			}
			else if(bullet[i].kind==4){
				draw_fire(bullet[i].x,bullet[i].y,2);
			}
			bullet[i].fire_num--;
		}
	}
	//�򵽻���
	for(i=0;i<500;i++){
		if(bullet[i].f){
			if(bullet[i].d==1){
				x=bullet[i].y/20;y=(bullet[i].x+10)/20;
				if(x>=0&&x<31&&y>=0&&y<50&&(map[x][y]==7||map[x][y]==8||map[x][y]==9||map[x][y]==10)) {
					gameover=true;return;
				}
			}
			else if(bullet[i].d==2){
				x=(bullet[i].y+10)/20;y=bullet[i].x/20;
				if(x>=0&&x<31&&y>=0&&y<50&&(map[x][y]==7||map[x][y]==8||map[x][y]==9||map[x][y]==10)) {
					gameover=true;return;
				}
			}
			else if(bullet[i].d==3){
				x=bullet[i].y/20;y=(bullet[i].x-10)/20;
				if(x>=0&&x<31&&y>=0&&y<50&&(map[x][y]==7||map[x][y]==8||map[x][y]==9||map[x][y]==10)) {
					gameover=true;return;
				}
			}
		}
	}
	//��ǽ
	for(i=0;i<500;i++){
		if(bullet[i].f&&bullet[i].kind!=1){
			if(bullet[i].d==0){
				x=(bullet[i].y-10)/20;y=bullet[i].x/20;
				//j=map[(bullet[i].y-10)/20][bullet[i].x/40];
				if(map[x][y]&&(bullet[i].y-10)/20==bullet[i].y/20){
					if(bullet[i].kind==0||bullet[i].kind==2){
						if(map[x][y]>=1&&map[x][y]<=3){
							bullet[i].f=0;
							bullet[i].fire_num=3;
							draw_fire(bullet[i].x,bullet[i].y+10,0);
						}
						if(bullet[i].kind==0) k=2;
						else k=3;
						if(map[x][y]<=k){
						map[x][y]=0;
						if(bullet[i].x%20<8&&y>0&&map[x][y-1]<=k) map[x][y-1]=0;
						else if(bullet[i].x%20>8&&y<49&&map[x][y+1]<=k) map[x][y+1]=0;
						}
					}
					else if(bullet[i].kind==3){
						
						if(map[x][y]<=3){
							remove_wall(x,y);
							bullet[i].f=0;
							bullet[i].fire_num=3;
							draw_fire(bullet[i].x-45,bullet[i].y-45,1);
						}
					}		
					
				}
			}
			else if(bullet[i].d==1){
				x=bullet[i].y/20;y=(bullet[i].x+10)/20;
				if(map[x][y]&&(bullet[i].x+10)/20==bullet[i].x/20){
					if(bullet[i].kind==0||bullet[i].kind==2){
						if(map[x][y]>=1&&map[x][y]<=3){
							bullet[i].f=0;
							bullet[i].fire_num=3;
							draw_fire(bullet[i].x-10,bullet[i].y,0);
						}
						if(bullet[i].kind==0) k=2;
						else k=3;
						if(map[x][y]<=k){
							map[x][y]=0;
						if(bullet[i].y%20<8&&x>0&&map[x-1][y]<=k) map[x-1][y]=0;
						else if(bullet[i].y%20>8&&x<30&&map[x+1][y]<=k) map[x+1][y]=0;
						}
					}
					else if(bullet[i].kind==3){
						
						if(map[x][y]<=3){
							remove_wall(x,y);
							bullet[i].f=0;
							bullet[i].fire_num=3;
							draw_fire(bullet[i].x-40,bullet[i].y-40,1);
						}
					}		
				}
			}
			else if(bullet[i].d==2){
				x=(bullet[i].y+10)/20;y=bullet[i].x/20;
				if(map[x][y]&&(bullet[i].y+10)/20==bullet[i].y/20){
					if(bullet[i].kind==0||bullet[i].kind==2){
						if(map[x][y]>=1&&map[x][y]<=3){
							bullet[i].f=0;
							bullet[i].fire_num=3;
							draw_fire(bullet[i].x,bullet[i].y-10,0);
						}
						if(bullet[i].kind==0) k=2;
						else k=3;
						if(map[x][y]<=k){
							map[x][y]=0;
							if(bullet[i].x%20<8&&y>0&&map[x][y-1]<=k) map[x][y-1]=0;
							else if(bullet[i].x%20>8&&y<49&&map[x][y+1]<=k) map[x][y+1]=0;
						}
					}
					else if(bullet[i].kind==3){
						
						if(map[x][y]<=3){
							remove_wall(x,y);
							bullet[i].f=0;
							bullet[i].fire_num=3;
							draw_fire(bullet[i].x-40,bullet[i].y-40,1);
						}
					}		
				}
			}
			else {
				x=bullet[i].y/20;y=(bullet[i].x-10)/20;
				if(map[x][y]&&(bullet[i].x-10)/20==bullet[i].x/20) {
					if(bullet[i].kind==0||bullet[i].kind==2){
						if(map[x][y]>=1&&map[x][y]<=3){
							bullet[i].f=0;
							bullet[i].fire_num=3;
							draw_fire(bullet[i].x+10,bullet[i].y,0);
						}
						if(bullet[i].kind==0) k=2;
						else k=3;
						if(map[x][y]<=k){
							map[x][y]=0;
							if(bullet[i].y%20<8&&y>0&&map[x-1][y]<=k) map[x-1][y]=0;
							else if(bullet[i].y%20>8&&y<30&&map[x+1][y]<=k) map[x+1][y]=0;
						}
					}
					else if(bullet[i].kind==3){
					
						if(map[x][y]<=3){
							remove_wall(x,y);
							bullet[i].f=0;
							bullet[i].fire_num=3;
							draw_fire(bullet[i].x-45,bullet[i].y-50,1);
						}
					}		
				}
			}
		}
	}
	//�򵽵���̹��
	for(i=0;i<500;i++){
		if(bullet[i].f&&bullet[i].belong<=1){
			for(j=0;j<5;j++){
					if(enemy[j].blood>0&&bullet[i].x>=enemy[j].x&&bullet[i].x<=enemy[j].x+40&&
						bullet[i].y>=enemy[j].y&&bullet[i].y<=enemy[j].y+40){
						bullet[i].f=0;

						if(enemy[j].no_hurt_time>0) continue;

						if(bullet[i].kind==3) enemy[j].blood-=2;
						else enemy[j].blood--;
						
						if(enemy[j].blood<=0){
							enemy[j].pre_time=GetTickCount();
							enemy[j].prop_time=0;
							if(bullet[i].d==0) x=-35,y=-35;
							else if(bullet[i].d==1) x=-35,y=-40;
							else if(bullet[i].d==2) x=-35,y=-40;
							else x=-35,y=-45;
							bullet[i].kind=4;
							bullet[i].fire_num=3;
							bullet[i].x=enemy[j].x+x;bullet[i].y=enemy[j].y+y;
							draw_fire(enemy[j].x+x,enemy[j].y+y,2);
							mciSendString("close Sound\\bang.wav", NULL, 0, 0);
							mciSendString("play Sound\\bang.wav", NULL, 0, 0);   
							enemy_num[start]--;
							fight_num[bullet[i].belong]++;
						}
						else mciSendString("play Sound\\hit.wav", NULL, 0, 0);   
					}
				}

		}
	}
	//���Լ���̹��
	for(i=0;i<500;i++){
		if(bullet[i].f&&bullet[i].belong>1){
			for(j=0;j<2;j++){
				if(j==1&&start<9) continue;
				if(myTank[j].blood&&bullet[i].x>=myTank[j].x&&bullet[i].x<=myTank[j].x+40&&
					bullet[i].y>=myTank[j].y&&bullet[i].y<=myTank[j].y+40){
					bullet[i].f=0;
					if(myTank[j].no_hurt_time>0) continue;

					myTank[j].blood--;

					if(!myTank[j].blood){
						myTank[j].prop_time=0;
						myTank[j].pre_time=GetTickCount();
						if(bullet[i].d==0) x=-35,y=-35;
						else if(bullet[i].d==1) x=-35,y=-40;
						else if(bullet[i].d==2) x=-35,y=-40;
						else x=-35,y=-45;
						bullet[i].kind=4;
						bullet[i].fire_num=3;
						bullet[i].x=myTank[j].x+x;bullet[i].y=myTank[j].y+y;
						draw_fire(myTank[j].x+x,myTank[j].y+y,2);
						mciSendString("close Sound\\bang.wav", NULL, 0, 0);
						mciSendString("play Sound\\bang.wav", NULL, 0, 0); 
						life_num[j]--;
						lv_up[j]=false;
					}
					else mciSendString("play Sound\\hit.wav", NULL, 0, 0);   
				}
			}
		}
	}
	
}

void myPaint(){
	int x, y;
	if(start<=5){//��û�н�����Ϸ����
		
		SelectObject(mmhdc,page[start]);
		BitBlt(mhdc,0,0,1150,680,mmhdc,0,0,SRCCOPY);
		if(start==0){
			y=258;
		}
		else if(start==1){
			y=335;
		}
		else if(start==2){
			y=413;
		}
		else if(start==3){
			y=500;
		}
		if(start<=3){
			x=330;
			SelectObject(mmhdc,T[0]);
			BitBlt(mhdc,x,y,40,40,mmhdc,0,0,SRCAND);
			SelectObject(mmhdc,T[1]);
			BitBlt(mhdc,x,y,40,40,mmhdc,0,0,SRCPAINT);
		}
		BitBlt(hdc,0,0,1150,680,mhdc,0,0,SRCCOPY);
	}
	else{//������Ϸ����
		SelectObject(mmhdc,background[0]);
		BitBlt(mhdc,0,0,1015,680,mmhdc,0,0,SRCCOPY);
		SelectObject(mmhdc,background[1]);
		BitBlt(mhdc,1000,0,130,680,mmhdc,0,0,SRCCOPY);
		if(start_time<0){		//��Ϸ��ʼʱ���ѹ�
			if(timing<=0)		//����û�б���ֹ
				move_enemy();
			else timing--;
		}
		else start_time--;
		if(end_time<=0){		//��Ϸ����ʱ���ѹ�
			if(end_time!=-1){
				if(start==8||start==11){win=true;}//����һ��Ϊһ����һ�������ҵĵ�������win
				else {
					start=start+1;//������һ��
					init();
				}
			}
		}
		else end_time--;
		
		bullet_hit();
		draw_bullet();
		draw_enemy();
		draw_myTank();
		draw_wall();
		draw_prop();
		draw_num();
		if(win){
			SelectObject(mmhdc,win_bmp[0]);
			BitBlt(mhdc,200,180,600,300,mmhdc,0,0,SRCAND);
			SelectObject(mmhdc,win_bmp[1]);
			BitBlt(mhdc,200,180,600,300,mmhdc,0,0,SRCPAINT);
		}
		if(gameover&&end_time<=0&&!win){
			SelectObject(mmhdc,gameover_bmp[0]);
			BitBlt(mhdc,200,180,600,300,mmhdc,0,0,SRCAND);
			SelectObject(mmhdc,gameover_bmp[1]);
			BitBlt(mhdc,200,180,600,300,mmhdc,0,0,SRCPAINT);
		}
		BitBlt(hdc,0,0,1300,680,mhdc,0,0,SRCCOPY);
		pre_Time=GetTickCount();	
	}
}