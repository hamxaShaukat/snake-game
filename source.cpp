#include<iostream>
#include<conio.h>
#include<windows.h>
#include<iomanip>
#include<fstream>
#include<string>
#include<time.h>
#include<mmsystem.h>
#include<Windows.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
#define gameRows 80
#define gameCols 80
#define PI 3.142
#define DBLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGREY 7
#define DARKGREY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define DWHITE 15
#define BLINK 128
enum DIRECTION{UP,DOWN,LEFT,RIGHT};
struct position {
	int ri, ci;
};
struct snake
{
	position* ps;
	int size;
	int score;
	char sym;
	DIRECTION dir;
	string p_Name;
	bool isDie;
	int LEFTKEY, RIGHTKEY, UPKEY, DOWNKEY;
};
struct food {
	position fp;
	bool isAlive;
	int store;
	char sym;
};
struct twoPlayerSnake {
	snake mS[2];
};
struct threePlayerSnake {
	snake mS[3];
};
struct fourPlayerSnake {
	snake mS[4];
};
struct baraFood {
	position fp;
	bool isAlive;
	bool isAvailable;
	int store;
	char sym;
};
struct hurdle {
	position S1H1[70];
	position S1H2[70];
	position S1H3[70];
	position S2H1[70];
	position S2H2[70];
	position S2H3[70];
	position S2H4[70];
	position S3H1[60];
	position S3H2[60];
	position S3H3[60];
	position S3H4[40];
};
struct stage {
	hurdle Sn[3];
};


// 1Player
void genFood(food& f, snake& S);
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
void stageOneInit(stage &sn)
{
	for (int i = 0, a = 5, b = gameCols / 4; i < 70; i++)
	{
		sn.Sn[0].S1H1[i].ri = b;
		sn.Sn[0].S1H1[i].ci = a;
		a++;
	}
	for (int i = 0, a = 5, b = 3 * gameCols / 4; i < 70; i++)
	{
		sn.Sn[0].S1H2[i].ri = b;
		sn.Sn[0].S1H2[i].ci = a;
		a++;
	}
	for (int i = 0, a = 5, b = gameCols / 2; i < 70; i++)
	{
		sn.Sn[0].S1H3[i].ri = a;
		sn.Sn[0].S1H3[i].ci = b;
		a++;
	}
}
void stageTwoInit(stage& sn)
{
	for (int i = 0, a = 5, b = gameCols / 4; i < 70; i++)
	{
		sn.Sn[1].S2H1[i].ri = b;
		sn.Sn[1].S2H1[i].ci = a;
		a++;
	}
	for (int i = 0, a = 5, b =gameCols / 2; i < 70; i++)
	{
		sn.Sn[1].S2H2[i].ri = b;
		sn.Sn[1].S2H2[i].ci = a;
		a++;
	}
	for (int i = 0, a = 5, b =3* gameCols / 4; i < 70; i++)
	{
		sn.Sn[1].S2H3[i].ri = b;
		sn.Sn[1].S2H3[i].ci = a;
		a++;
	}
	for (int i = 0, a = 0, b = gameCols / 2; i < 70; i++)
	{
		sn.Sn[1].S2H4[i].ri = a;
		sn.Sn[1].S2H4[i].ci = b;
		a++;
	}
}
void stageThreeInit(stage& sn)
{
	for (int i = 0, a = 0, b = gameCols / 4; i < 60; i++)
	{
		sn.Sn[2].S3H1[i].ri = a;
		sn.Sn[2].S3H1[i].ci = b;
		a++;
	}
	for (int i = 0, a = 10, b = gameCols / 2; i < 60; i++)
	{
		sn.Sn[2].S3H2[i].ri = a;
		sn.Sn[2].S3H2[i].ci = b;
		a++;
	}
	for (int i = 0, a = 0, b = 3 * gameCols / 4; i < 60; i++)
	{
		sn.Sn[2].S3H3[i].ri = a;
		sn.Sn[2].S3H3[i].ci = b;
		a++;
	}
	for (int i = 0, a = 0, b = (3 * gameCols / 4)+10; i < 40; i++)
	{
		sn.Sn[2].S3H4[i].ri = b;
		sn.Sn[2].S3H4[i].ci = a;
		a++;
	}
}
void printstageOneConsole(stage sn)
{
	for (int i = 0; i < 70; i++)
	{
		gotoRowCol(sn.Sn[0].S1H1[i].ri, sn.Sn[0].S1H1[i].ci);
		cout << char(-37);
	}
	for (int i = 0, a = 5, b = 3 * gameCols / 4; i < 70; i++)
	{
		gotoRowCol(sn.Sn[0].S1H2[i].ri, sn.Sn[0].S1H2[i].ci);
		cout << char(-37);
	}
	for (int i = 0; i < 70; i++)
	{
		gotoRowCol(sn.Sn[0].S1H3[i].ri, sn.Sn[0].S1H3[i].ci);
		cout << char(-37);
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(0, i);
		cout << char(-37);
	}
	for (int i = 0; i <= gameCols; i++)
	{
		gotoRowCol(80, i);
		cout << char(-37);
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(i, 80);
		cout << char(-37);
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(i, 0);
		cout << char(-37);
	}
}
void printstageTwoConsole(stage sn)
{
	for (int i = 0; i < 70; i++)
	{
		gotoRowCol(sn.Sn[1].S2H1[i].ri, sn.Sn[1].S2H1[i].ci);
		cout << char(-37);
	}
	for (int i = 0, a = 5, b = 3 * gameCols / 4; i < 70; i++)
	{
		gotoRowCol(sn.Sn[1].S2H2[i].ri, sn.Sn[1].S2H2[i].ci);
		cout << char(-37);
	}
	for (int i = 0; i < 70; i++)
	{
		gotoRowCol(sn.Sn[1].S2H3[i].ri, sn.Sn[1].S2H3[i].ci);
		cout << char(-37);
	}
	for (int i = 0; i < 70; i++)
	{
		gotoRowCol(sn.Sn[1].S2H4[i].ri, sn.Sn[1].S2H4[i].ci);
		cout << char(-37);
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(0, i);
		cout << char(-37);
	}
	for (int i = 0; i <= gameCols; i++)
	{
		gotoRowCol(80, i);
		cout << char(-37);
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(i, 80);
		cout << char(-37);
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(i, 0);
		cout << char(-37);
	}
}
void printstageThreeConsole(stage sn)
{
	for (int i = 0; i < 60; i++)
	{
		gotoRowCol(sn.Sn[2].S3H1[i].ri, sn.Sn[2].S3H1[i].ci);
		cout << char(-37);
	}
	for (int i = 0; i < 60; i++)
	{
		gotoRowCol(sn.Sn[2].S3H2[i].ri, sn.Sn[2].S3H2[i].ci);
		cout << char(-37);
	}
	for (int i = 0; i < 60; i++)
	{
		gotoRowCol(sn.Sn[2].S3H3[i].ri, sn.Sn[2].S3H3[i].ci);
		cout << char(-37);
	}
	for (int i = 0; i < 40; i++)
	{
		gotoRowCol(sn.Sn[2].S3H4[i].ri, sn.Sn[2].S3H4[i].ci);
		cout << char(-37);
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(0, i);
		cout << char(-37);
	}
	for (int i = 0; i <= gameCols; i++)
	{
		gotoRowCol(80, i);
		cout << char(-37);
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(i, 80);
		cout << char(-37);
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(i, 0);
		cout << char(-37);
	}
}
void consoleScrn1()
{
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(0, i);
		cout << char(-37);
	}
	for (int i = 0; i <= gameCols; i++)
	{
		gotoRowCol(80, i);
		cout << char(-37);
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(i, 80);
		cout << char(-37);
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(i, 0);
		cout << char(-37);
	}
}
void consoleScrnInfinity()
{
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(0, i);
		cout << '`';
	}
	for (int i = 0; i <= gameCols; i++)
	{
		gotoRowCol(80, i);
		cout << '`';
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(i, 80);
		cout << '.';
	}
	for (int i = 0; i < gameCols; i++)
	{
		gotoRowCol(i, 0);
		cout << '.';
	}
}
void init(snake& S,food &f)
{
	S.ps = new position[3];
	S.ps[0].ri = gameRows / 2;
	S.ps[0].ci = gameCols / 2;

	S.ps[1].ri = gameRows / 2;
	S.ps[1].ci = gameCols / 2 - 1;

	S.ps[2].ri = gameRows / 2;
	S.ps[2].ci = gameCols / 2 - 2;

	S.size = 3;
	S.score = 0;
	S.sym = -37;
	S.p_Name = "Ali";
	S.dir = RIGHT;
	S.RIGHTKEY = 77;
	S.LEFTKEY = 75;
	S.UPKEY = 72;
	S.DOWNKEY = 80;
	genFood(f, S);
	f.sym = 'X';
}
void displaySnake(const snake& S)
{
	for (int i = 0; i < S.size; i++)
	{
		if (i == 0)
		{
			gotoRowCol(S.ps[i].ri, S.ps[i].ci);
			SetClr(2);
			cout <<'Q';
		}
		else
		{
			gotoRowCol(S.ps[i].ri, S.ps[i].ci);
			SetClr(15);
			cout << 'O';
		}
	}
}
void removeSnake(const snake& S)
{
	for (int i = 0; i < S.size; i++)
	{
		if (i == 0)
		{
			gotoRowCol(S.ps[i].ri, S.ps[i].ci);
			cout << '0';
		}
		gotoRowCol(S.ps[i].ri, S.ps[i].ci);
		cout << ' ';
	}
}
void moveSnake(snake& S)
{
	for (int i = S.size - 1; i - 1 >= 0; i--)
	{
		S.ps[i] = S.ps[i - 1];
	}

	switch (S.dir)
	{
	case UP:
		S.ps[0].ri--;
		if (S.ps[0].ri == 0)
		{
			S.ps[0].ri = gameRows - 1;
		}
		break;
	case DOWN:
		S.ps[0].ri++;
		if (S.ps[0].ri == gameCols)
		{
			S.ps[0].ri = 1;
		}
		break;
	case LEFT:
		S.ps[0].ci--;
		if (S.ps[0].ci == 0)
		{
			S.ps[0].ci = gameRows - 1;
		}
		break;
	case RIGHT:
		S.ps[0].ci++;
		if (S.ps[0].ci == gameCols)
		{
			S.ps[0].ci = 1;
		}

		break;
	}
}
void changeDirection(snake& S, char key)
{
	if (key == S.LEFTKEY)
	{
		if (S.dir != RIGHT)
		{
			S.dir = LEFT;
		}
	}
	if (key == S.RIGHTKEY)
	{
		if (S.dir != LEFT)
		{
			S.dir = RIGHT;
		}
	}
	if (key == S.UPKEY)
	{
		if (S.dir != DOWN)
		{
			S.dir = UP;
		}
	}
	if (key == S.DOWNKEY)
	{
		if (S.dir != UP)
		{
			S.dir = DOWN;
		}
	}
}
bool isLegalCord(position P, snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		if (s.ps[i].ri == P.ri && s.ps[i].ci == P.ci)
			if ((P.ri == 0 || P.ri == gameRows) && (P.ci == 0 || P.ci == gameCols))
				return false;
	}
	return true;
}
bool isLegalCordBoss(position P, snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		if (s.ps[i].ri == P.ri && s.ps[i].ci == P.ci)
			if ((P.ri == 0 || P.ri == gameRows) && (P.ci == 0 || P.ci == gameCols))
				if (isLegalCord(P, s) == false)
					return false;
	}
	return true;
}
void genFood(food& f, snake& S)
{
	do {
		f.fp.ri = (rand() % (gameCols - 2) + 1);
		f.fp.ci = (rand() % (gameCols - 2) + 1);
	} while (isLegalCord(f.fp, S) == false);
	f.isAlive = true;
	f.store = 1;
}
void displayFood(food f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	SetClr(9);
	cout << f.sym;
}
bool eatFood(food f,snake &S)
{
	if (S.ps[0].ri == f.fp.ri && S.ps[0].ci == f.fp.ci)
	{
		return true;
	}
	return false;
}
void snakeExpand(snake& S)
{
	position *newS = new position[S.size + 1];
	for (int i = 0; i < S.size; i++)
	{
		newS[i] = S.ps[i];
	}
	S.size++;
	newS[S.size] = S.ps[S.size - 1];
	delete[]S.ps;
	S.ps = newS;

}
void singleGenBaraFood(baraFood& f, snake& Mp)
{
	do {
		f.fp.ri = (rand() % (gameCols - 2) + 1);
		f.fp.ci = (rand() % (gameCols - 2) + 1);
	} while (isLegalCord(f.fp, Mp) == false);
	f.isAlive = true;
	f.store = 1;
}
bool eatBaraFood(baraFood f, snake& Mp)
{
		if (Mp.ps[0].ri == f.fp.ri && Mp.ps[0].ci == f.fp.ci)
		{
			return true;
		}
	return false;
}
bool selfDeath(snake S)
{
	for (int i = 1; i < S.size; i++)
	{
		if (S.ps[0].ri == S.ps[i].ri && S.ps[0].ci == S.ps[i].ci)
			return true;
	}
	return false;

}
//1Player

// Boxes 



void printBoxSinglePlayer()
{
	gotoRowCol(25, 38);
	cout << "One P";
	for (int i= gameRows / 2 - 5, j = 0; j < 10; j++,i++)
	{
		gotoRowCol(gameRows / 4, i);
		cout << char(-37);
	}
	for (int i = gameRows / 4 , j = 0; j < 10; j++,i++)
	{
		gotoRowCol(i, gameRows / 2 + 5);
		cout << char(-37);
	}
	for (int i = gameRows / 2 - 5, j = 0; j <=10; j++, i++)
	{
		gotoRowCol(gameRows / 4+10, i);
		cout << char(-37);
	}
	for (int i = gameRows / 4, j = 0; j <10; j++, i++)
	{
		gotoRowCol(i, gameRows / 2 - 5);
		cout << char(-37);
	}
	
}
void printBoxTwoPlayer()
{
	gotoRowCol(35, 38);
	cout << "Two Ps";
	for (int i = gameRows / 2 - 5, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(gameRows / 4+11, i);
		cout << char(-37);
	}
	for (int i = gameRows / 4+11, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(i, gameRows / 2 + 5);
		cout << char(-37);
	}
	for (int i = gameRows / 2 - 5, j = 0; j <= 10; j++, i++)
	{
		gotoRowCol(gameRows / 2 , i);
		cout << char(-37);
	}
	for (int i = gameRows / 4 +11, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(i, gameRows / 2 - 5);
		cout << char(-37);
	}

}
void printBoxThreePlayer()
{
	gotoRowCol(45, 36);
	cout << "Three Ps";
	for (int i = gameRows / 2 - 5, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(gameRows / 2 + 1, i);
		cout << char(-37);
	}
	for (int i = gameRows / 2 + 1, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(i, gameRows / 2 + 5);
		cout << char(-37);
	}
	for (int i = gameRows / 2 - 5, j = 0; j <= 10; j++, i++)
	{
		gotoRowCol(gameRows / 2+11, i);
		cout << char(-37);
	}
	for (int i = gameRows / 2 + 1, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(i, gameRows / 2 - 5);
		cout << char(-37);
	}
}
void printBoxFourPlayer()
{
	gotoRowCol(55, 38);
	cout << "FourPs";
	for (int i = gameRows / 2 - 5, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(gameRows / 2 + 11, i);
		cout << char(-37);
	}
	for (int i = gameRows / 2 + 11, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(i, gameRows / 2 + 5);
		cout << char(-37);
	}
	for (int i = gameRows / 2 - 5, j = 0; j <= 10; j++, i++)
	{
		gotoRowCol(3*gameRows / 4 , i);
		cout << char(-37);
	}
	for (int i = gameRows / 2 + 11, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(i, gameRows / 2 - 5);
		cout << char(-37);
	}
}
void modeChoiceTimed()
{
	gotoRowCol(25, 38);
	cout << "Timed";
	for (int i = gameRows / 2 - 5, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(gameRows / 4, i);
		cout << char(-37);
	}
	for (int i = gameRows / 4, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(i, gameRows / 2 + 5);
		cout << char(-37);
	}
	for (int i = gameRows / 2 - 5, j = 0; j <= 10; j++, i++)
	{
		gotoRowCol(gameRows / 4 + 10, i);
		cout << char(-37);
	}
	for (int i = gameRows / 4, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(i, gameRows / 2 - 5);
		cout << char(-37);
	}

}
void modeChoiceUntimed()
{
	gotoRowCol(35, 38);
	cout << "Untimed";
	for (int i = gameRows / 2 - 5, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(gameRows / 4 + 11, i);
		cout << char(-37);
	}
	for (int i = gameRows / 4 + 11, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(i, gameRows / 2 + 5);
		cout << char(-37);
	}
	for (int i = gameRows / 2 - 5, j = 0; j <= 10; j++, i++)
	{
		gotoRowCol(gameRows / 2, i);
		cout << char(-37);
	}
	for (int i = gameRows / 4 + 11, j = 0; j < 10; j++, i++)
	{
		gotoRowCol(i, gameRows / 2 - 5);
		cout << char(-37);
	}

}
void stagesMode()
{

}
bool checkOnePlayer()
{
	int r, c;
	getRowColbyLeftClick(r, c);
	if ((r < 30 && r>20) && (c > 35 && c < 45))
		return true;
	return false;
}
bool checkTwoPlayer()
{
	int r, c;
	getRowColbyLeftClick(r, c);
	if ((r < 40 && r>30) && (c > 35 && c < 45))
		return true;
	return false;
}
bool checkThreePlayer()
{
	int r, c;
	getRowColbyLeftClick(r, c);
	if ((r < 50 && r>40) && (c > 35 && c < 45))
		return true;
	return false;
}
bool checkFourPlayer()
{
	int r, c;
	getRowColbyLeftClick(r, c);
	if ((r < 60 && r>50) && (c > 35 && c < 45))
		return true;
	return false;
}
void selectMode()
{
	system("cls");
	modeChoiceTimed();
	modeChoiceUntimed();
}
void mainMenu()
{
	printBoxSinglePlayer();
	printBoxTwoPlayer();
	printBoxThreePlayer();
	printBoxFourPlayer();
	
}

/*
--------------------------------------2Players---------------------------------
*/
// 2Players start
void twoPlayerGenFood(food& f, twoPlayerSnake& Mp);
void twoPlayerInit(twoPlayerSnake& Mp,food &f)
{
	// 1P snake
	Mp.mS[0].ps = new position[3];
	Mp.mS[0].ps[0].ri = gameRows / 2;
	Mp.mS[0].ps[0].ci = gameCols / 2;

	Mp.mS[0].ps[1].ri = gameRows / 2;
	Mp.mS[0].ps[1].ci = gameCols / 2 - 1;

	Mp.mS[0].ps[2].ri = gameRows / 2;
	Mp.mS[0].ps[2].ci = gameCols / 2 - 2;

	Mp.mS[0].size = 3;
	Mp.mS[0].score = 0;

	Mp.mS[0].p_Name = "Hamza";
	Mp.mS[0].sym = 'O';
	Mp.mS[0].dir = RIGHT;
	Mp.mS[0].RIGHTKEY = 77, Mp.mS[0].LEFTKEY = 75, Mp.mS[0].UPKEY = 72, Mp.mS[0].DOWNKEY = 80;

	//2P smake
	Mp.mS[1].ps = new position[3];
	Mp.mS[1].ps[0].ri = gameRows / 2 - 5;
	Mp.mS[1].ps[0].ci = gameCols / 2;

	Mp.mS[1].ps[1].ri = gameRows / 2 - 5;
	Mp.mS[1].ps[1].ci = gameCols / 2 - 1;

	Mp.mS[1].ps[2].ri = gameRows / 2 - 5;
	Mp.mS[1].ps[2].ci = gameCols / 2 - 2;

	Mp.mS[1].size = 3;
	Mp.mS[1].score = 0;

	Mp.mS[1].p_Name = "Hamza";
	Mp.mS[1].sym = 'O';
	Mp.mS[1].dir = RIGHT;
	Mp.mS[1].RIGHTKEY = 100, Mp.mS[1].LEFTKEY = 97, Mp.mS[1].UPKEY = 119, Mp.mS[1].DOWNKEY = 115;
	twoPlayerGenFood(f, Mp);
	f.sym = 'X';


}
void twoPlayerDisplaySnake(twoPlayerSnake Mp)
{
	//1P
	for (int i = 0; i < Mp.mS[0].size; i++)
	{
		gotoRowCol(Mp.mS[0].ps[i].ri, Mp.mS[0].ps[i].ci);
		if (Mp.mS[0].ps[0].ri == 0 && Mp.mS[0].ps[0].ci == 0)
		{
			SetClr(4);
			cout << Mp.mS[0].sym;
		}
		else
		{
			SetClr(10);
			cout << Mp.mS[0].sym;
		}
	}
	//2P 
	for (int i = 0; i < Mp.mS[1].size; i++)
	{
		gotoRowCol(Mp.mS[1].ps[i].ri, Mp.mS[1].ps[i].ci);
		if (Mp.mS[1].ps[0].ri == 0 && Mp.mS[1].ps[0].ci == 0)
		{
			SetClr(4);
			cout << Mp.mS[1].sym;
		}
		else
		{
			SetClr(11);
			cout << Mp.mS[1].sym;
		}
	}
}
void twoPlayerEraseSnake(twoPlayerSnake Mp)
{
	for (int i = 0; i < Mp.mS[0].size; i++)
	{
		gotoRowCol(Mp.mS[0].ps[i].ri, Mp.mS[0].ps[i].ci);
		cout << ' ';
	}
	for (int i = 0; i < Mp.mS[1].size; i++)
	{
		gotoRowCol(Mp.mS[1].ps[i].ri, Mp.mS[1].ps[i].ci);
		cout << ' ';
	}
}
void twoPlayerMoveSnake(twoPlayerSnake& Mp)
{
	for (int i = Mp.mS[0].size - 1; i > 0; i--)
	{
		Mp.mS[0].ps[i] = Mp.mS[0].ps[i - 1];
	}
	switch (Mp.mS[0].dir)
	{
	case UP:
		Mp.mS[0].ps[0].ri--;
		if (Mp.mS[0].ps[0].ri == 0)
		{
			Mp.mS[0].ps[0].ri = gameRows - 1;
		}
		break;
	case DOWN:
		Mp.mS[0].ps[0].ri++;
		if (Mp.mS[0].ps[0].ri == gameRows)
		{
			Mp.mS[0].ps[0].ri = 1;
		}
		break;
	case LEFT:
		Mp.mS[0].ps[0].ci--;
		if (Mp.mS[0].ps[0].ci == 0)
		{
			Mp.mS[0].ps[0].ci = gameCols - 1;
		}
		break;
	case RIGHT:
		Mp.mS[0].ps[0].ci++;
		if (Mp.mS[0].ps[0].ci == gameCols)
		{
			Mp.mS[0].ps[0].ci = 1;
		}
		break;
	}


	for (int i = Mp.mS[1].size - 1; i > 0; i--)
	{
		Mp.mS[1].ps[i] = Mp.mS[1].ps[i - 1];
	}
	switch (Mp.mS[1].dir)
	{
	case UP:
		Mp.mS[1].ps[0].ri--;
		if (Mp.mS[1].ps[0].ri == 0)
		{
			Mp.mS[1].ps[0].ri = gameRows - 1;
		}
		break;
	case DOWN:
		Mp.mS[1].ps[0].ri++;
		if (Mp.mS[1].ps[0].ri == gameRows)
		{
			Mp.mS[1].ps[0].ri = 1;
		}
		break;
	case LEFT:
		Mp.mS[1].ps[0].ci--;
		if (Mp.mS[1].ps[0].ci == 0)
		{
			Mp.mS[1].ps[0].ci = gameCols - 1;
		}
		break;
	case RIGHT:
		Mp.mS[1].ps[0].ci++;
		if (Mp.mS[1].ps[0].ci == gameCols)
		{
			Mp.mS[1].ps[0].ci = 1;
		}
		break;
	}


}
void twoPlayerChangeDirection(twoPlayerSnake& Mp, char key_pressed)
{
	if (key_pressed == Mp.mS[0].UPKEY)
	{

		if (Mp.mS[0].dir != DOWN)
		{
			Mp.mS[0].dir = UP;
		}
	}
	if (key_pressed == Mp.mS[0].DOWNKEY)
	{
		if (Mp.mS[0].dir != UP)
		{
			Mp.mS[0].dir = DOWN;
		}
	}
	if (key_pressed == Mp.mS[0].LEFTKEY)
	{
		if (Mp.mS[0].dir != RIGHT)
		{
			Mp.mS[0].dir = LEFT;
		}
	}
	if (key_pressed == Mp.mS[0].RIGHTKEY)
	{
		if (Mp.mS[0].dir != LEFT)
		{
			Mp.mS[0].dir = RIGHT;
		}
	}

	//2P

	if (key_pressed == Mp.mS[1].UPKEY)
	{

		if (Mp.mS[1].dir != DOWN)
		{
			Mp.mS[1].dir = UP;
		}
	}
	if (key_pressed == Mp.mS[1].DOWNKEY)
	{
		if (Mp.mS[1].dir != UP)
		{
			Mp.mS[1].dir = DOWN;
		}
	}
	if (key_pressed == Mp.mS[1].LEFTKEY)
	{
		if (Mp.mS[1].dir != RIGHT)
		{
			Mp.mS[1].dir = LEFT;
		}
	}
	if (key_pressed == Mp.mS[1].RIGHTKEY)
	{
		if (Mp.mS[1].dir != LEFT)
		{
			Mp.mS[1].dir = RIGHT;
		}
	}


}
void twoPlayerGrowSnake(twoPlayerSnake& Mp)
{
	position* arr_1 = new position[Mp.mS[0].size + 1];
	for (int i = 0; i < Mp.mS[0].size; i++)
	{
		arr_1[i] = Mp.mS[0].ps[i];
	}
	arr_1[Mp.mS[0].size] = Mp.mS[0].ps[Mp.mS[0].size - 1];
	Mp.mS[0].size++;
	delete[] Mp.mS[0].ps;
	Mp.mS[0].ps = arr_1;
	//2p
	position* arr_2 = new position[Mp.mS[1].size + 1];
	for (int i = 0; i < Mp.mS[1].size; i++)
	{
		arr_2[i] = Mp.mS[1].ps[i];
	}
	arr_2[Mp.mS[1].size] = Mp.mS[1].ps[Mp.mS[1].size - 1];
	Mp.mS[1].size++;
	delete[] Mp.mS[1].ps;
	Mp.mS[1].ps = arr_2;

}
bool twoPlayerSelfDeath1stPlayer(twoPlayerSnake Mp)
{
	for (int i = 1; i < Mp.mS[0].size; i++)
	{
		if (Mp.mS[0].ps[0].ri == Mp.mS[0].ps[i].ri && Mp.mS[0].ps[0].ci == Mp.mS[0].ps[i].ci)
			return true;
	}
	return false;
}
bool twoPlayerSelfDeath2ndPlayer(twoPlayerSnake Mp)
{
	for (int i = 1; i < Mp.mS[1].size; i++)
	{
		if (Mp.mS[1].ps[0].ri == Mp.mS[1].ps[i].ri && Mp.mS[1].ps[0].ci == Mp.mS[1].ps[i].ci)
			return true;
	}
	return false;
}
bool twoPlayer1stPlayerLoseCondition(twoPlayerSnake Mp)
{
	for (int i = 1; i < Mp.mS[1].size; i++)
	{
		if (Mp.mS[0].ps[0].ri == Mp.mS[1].ps[i].ri && Mp.mS[0].ps[0].ci == Mp.mS[1].ps[i].ci)
			return true;
	}
	return false;
}
bool twoPlayer2ndPlayerLoseCondition(twoPlayerSnake Mp)
{
	for (int i = 1; i < Mp.mS[1].size; i++)
	{
		if (Mp.mS[1].ps[0].ri == Mp.mS[0].ps[i].ri && Mp.mS[1].ps[0].ci == Mp.mS[0].ps[i].ci)
			return true;
	}
	return false;
}
bool twoPlayerDrawingCondition(twoPlayerSnake Mp)
{
	if (Mp.mS[0].ps[0].ri == Mp.mS[1].ps[0].ri && Mp.mS[0].ps[0].ci == Mp.mS[1].ps[0].ci)
		return true;
	else if (Mp.mS[1].ps[0].ri == Mp.mS[0].ps[0].ri && Mp.mS[1].ps[0].ci == Mp.mS[0].ps[0].ci)
		return true;
	return false;
}
bool twoPlayerDecision(twoPlayerSnake Mp)
{
	if (twoPlayerSelfDeath1stPlayer(Mp) == true)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 2 has Won the game--------------------\n\n";
		cout << "--------------Due to self death------------------------\n\n";
		return true;
	}
	else if (twoPlayer1stPlayerLoseCondition(Mp)==true)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 2 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
		return true;
	}
	else if (twoPlayerSelfDeath2ndPlayer(Mp) == true)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 1 has Won the game--------------------\n\n";
		cout << "--------------Due to self death------------------------\n\n";
		return true;
	}
	else if (twoPlayer2ndPlayerLoseCondition(Mp) == true)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 1 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
		return true;
	}
	else if (twoPlayerDrawingCondition(Mp) == true)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Game has Drawn--------------------\n\n";
		cout << "--------------------------------------\n\n";
		return true;
	}
	return false;
}
bool twoPlayerIsLegalCord(position P, twoPlayerSnake&Mp)
{
	for (int i = 0; i < Mp.mS[0].size; i++)
	{
		if (Mp.mS[0].ps[i].ri == P.ri && Mp.mS[0].ps[i].ci == P.ci)
			if ((P.ri == 0 || P.ri == gameRows) && (P.ci == 0 || P.ci == gameCols))
				return false;
	}
	for (int i = 0; i < Mp.mS[1].size; i++)
	{
		if (Mp.mS[1].ps[i].ri == P.ri && Mp.mS[1].ps[i].ci == P.ci)
			if ((P.ri == 0 || P.ri == gameRows) && (P.ci == 0 || P.ci == gameCols))
				return false;
	}
	return true;
}
void twoPlayerGenFood(food& f, twoPlayerSnake& Mp)
{
	do {
		f.fp.ri = (rand() % (gameCols - 2) + 1);
		f.fp.ci = (rand() % (gameCols - 2) + 1);
	} while (twoPlayerIsLegalCord(f.fp, Mp) == false);
	f.isAlive = true;
	f.store = 1;
}
void twoPlayerDisplayFood(food f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	SetClr(9);
	cout << f.sym;
}
void twoPlayerGenBaraFood(baraFood& F, twoPlayerSnake& Mp)
{
	do {
		F.fp.ri = (rand() % (gameCols - 2) + 1);
		F.fp.ci = (rand() % (gameCols - 2) + 1);
	} while (twoPlayerIsLegalCord(F.fp, Mp) == false);
	F.isAlive = true;
	F.store = 6;
	F.isAvailable = true;
}
void twoPlayerDisplayBaraFood(baraFood f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	SetClr(9);
	cout << f.sym;
}
void twoPlayerEraseBaraFood(baraFood f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	cout << ' ';
}
bool twoPlayerEat1stPlayerFood(food f, twoPlayerSnake &Mp)
{
	if (Mp.mS[0].ps[0].ri == f.fp.ri && Mp.mS[0].ps[0].ci == f.fp.ci)
	{
		position* arr_1 = new position[Mp.mS[0].size + 1];
		for (int i = 0; i < Mp.mS[0].size; i++)
		{
			arr_1[i] = Mp.mS[0].ps[i];
		}
		arr_1[Mp.mS[0].size] = Mp.mS[0].ps[Mp.mS[0].size - 1];
		Mp.mS[0].size++;
		delete[] Mp.mS[0].ps;
		Mp.mS[0].ps = arr_1;
		return true;
	}
	return false;
}
bool twoPlayerEat2ndPlayerFood(food f, twoPlayerSnake& Mp)
{
	if (Mp.mS[1].ps[0].ri == f.fp.ri && Mp.mS[1].ps[0].ci == f.fp.ci)
	{
		position* arr_1 = new position[Mp.mS[1].size + 1];
		for (int i = 0; i < Mp.mS[1].size; i++)
		{
			arr_1[i] = Mp.mS[1].ps[i];
		}
		arr_1[Mp.mS[1].size] = Mp.mS[1].ps[Mp.mS[1].size - 1];
		Mp.mS[1].size++;
		delete[] Mp.mS[1].ps;
		Mp.mS[1].ps = arr_1;
		return true;
	}
	return false;
}
bool twoPlayer1stPlayerEatBaraFood(baraFood f, twoPlayerSnake& Mp)
{
	if (Mp.mS[0].isDie == false)
	{
		if (Mp.mS[0].ps[0].ri == f.fp.ri && Mp.mS[0].ps[0].ci == f.fp.ci)
		{
			return true;
		}
	}
	return false;
}
bool twoPlayer2ndPlayerEatBaraFood(baraFood f, twoPlayerSnake& Mp)
{
	if (Mp.mS[1].isDie == false)
	{
		if (Mp.mS[1].ps[0].ri == f.fp.ri && Mp.mS[1].ps[0].ci == f.fp.ci)
		{
			return true;
		}
	}
	return false;
}

//2Players End
/*
------------------------------------------------------------------------------------------
*/

// 3Players start
void threePlayerGenFood(food& f, threePlayerSnake& Mp);
void threePlayerInit(threePlayerSnake& Mp,food &f)
{
	// 1P snake
	Mp.mS[0].ps = new position[3];
	Mp.mS[0].ps[0].ri = gameRows / 2;
	Mp.mS[0].ps[0].ci = gameCols / 2;

	Mp.mS[0].ps[1].ri = gameRows / 2;
	Mp.mS[0].ps[1].ci = gameCols / 2 - 1;

	Mp.mS[0].ps[2].ri = gameRows / 2;
	Mp.mS[0].ps[2].ci = gameCols / 2 - 2;

	Mp.mS[0].size = 3;
	Mp.mS[0].score = 0;

	Mp.mS[0].p_Name = "Hamza";
	Mp.mS[0].sym = 'O';
	Mp.mS[0].dir = RIGHT;
	Mp.mS[0].RIGHTKEY = 77, Mp.mS[0].LEFTKEY = 75, Mp.mS[0].UPKEY = 72, Mp.mS[0].DOWNKEY = 80;
	Mp.mS[0].isDie = false;

	//2P smake
	Mp.mS[1].ps = new position[3];
	Mp.mS[1].ps[0].ri = gameRows / 2 - 1;
	Mp.mS[1].ps[0].ci = gameCols / 2;

	Mp.mS[1].ps[1].ri = gameRows / 2 - 1;
	Mp.mS[1].ps[1].ci = gameCols / 2 - 1;

	Mp.mS[1].ps[2].ri = gameRows / 2 - 1;
	Mp.mS[1].ps[2].ci = gameCols / 2 - 2;

	Mp.mS[1].size = 3;
	Mp.mS[1].score = 0;

	Mp.mS[1].p_Name = "Hamza";
	Mp.mS[1].sym = 'O';
	Mp.mS[1].dir = RIGHT;
	Mp.mS[1].RIGHTKEY = 100, Mp.mS[1].LEFTKEY = 97, Mp.mS[1].UPKEY = 119, Mp.mS[1].DOWNKEY = 115;
	Mp.mS[1].isDie = false;

	//3P smake
	Mp.mS[2].ps = new position[3];
	Mp.mS[2].ps[0].ri = gameRows / 2 - 2;
	Mp.mS[2].ps[0].ci = 3*gameCols / 4;

	Mp.mS[2].ps[1].ri = gameRows / 2 - 2;
	Mp.mS[2].ps[1].ci = gameCols / 2 - 1;

	Mp.mS[2].ps[2].ri = gameRows / 2 - 2;
	Mp.mS[2].ps[2].ci = gameCols / 2 - 2;

	Mp.mS[2].size = 3;
	Mp.mS[2].score = 0;

	Mp.mS[2].p_Name = "Hamza";
	Mp.mS[2].sym = 'O';
	Mp.mS[2].dir = RIGHT;
	Mp.mS[2].RIGHTKEY = 104, Mp.mS[2].LEFTKEY = 102, Mp.mS[2].UPKEY = 116, Mp.mS[2].DOWNKEY = 103;
	Mp.mS[2].isDie = false;

	threePlayerGenFood(f, Mp);
	f.sym = 'X';
}
void threePlayerDisplaySnake(threePlayerSnake Mp)
{
	//1P
	if (Mp.mS[0].isDie == false)
	{
		for (int i = 0; i < Mp.mS[0].size; i++)
		{
			gotoRowCol(Mp.mS[0].ps[i].ri, Mp.mS[0].ps[i].ci);
			if (Mp.mS[0].ps[0].ri == 0 && Mp.mS[0].ps[0].ci == 0)
			{
				SetClr(4);
				cout << Mp.mS[0].sym;
			}
			else
			{
				SetClr(14);
				cout << Mp.mS[0].sym;
			}
		}
	}
	//2P 
	if (Mp.mS[1].isDie == false)
	{
		for (int i = 0; i < Mp.mS[1].size; i++)
		{
			gotoRowCol(Mp.mS[1].ps[i].ri, Mp.mS[1].ps[i].ci);
			if (Mp.mS[1].ps[0].ri == 0 && Mp.mS[1].ps[0].ci == 0)
			{
				SetClr(4);
				cout << Mp.mS[1].sym;
			}
			else
			{
				SetClr(2);
				cout << Mp.mS[1].sym;
			}
		}
	}
	//3P
	if (Mp.mS[2].isDie == false)
	{
		for (int i = 0; i < Mp.mS[2].size; i++)
		{
			gotoRowCol(Mp.mS[2].ps[i].ri, Mp.mS[2].ps[i].ci);
			if (Mp.mS[2].ps[0].ri == 0 && Mp.mS[2].ps[0].ci == 0)
			{
				SetClr(4);
				cout << Mp.mS[2].sym;
			}
			else
			{
				SetClr(3);
				cout << Mp.mS[2].sym;
			}
		}
	}
}
void threePlayerEraseSnake(threePlayerSnake Mp)
{
	//1P
	if (Mp.mS[0].isDie == false)
	{
		for (int i = 0; i < Mp.mS[0].size; i++)
		{
			gotoRowCol(Mp.mS[0].ps[i].ri, Mp.mS[0].ps[i].ci);
			cout << ' ';
		}
	}
	//2P
	if (Mp.mS[1].isDie == false)
	{
		for (int i = 0; i < Mp.mS[1].size; i++)
		{
			gotoRowCol(Mp.mS[1].ps[i].ri, Mp.mS[1].ps[i].ci);
			cout << ' ';
		}
	}
	//3P
	if (Mp.mS[2].isDie == false)
	{
		for (int i = 0; i < Mp.mS[2].size; i++)
		{
			gotoRowCol(Mp.mS[2].ps[i].ri, Mp.mS[2].ps[i].ci);
			cout << ' ';
		}
	}
}
void threePlayerMoveSnake(threePlayerSnake& Mp)
{
	//1P
	if (Mp.mS[0].isDie == false)
	{
		for (int i = Mp.mS[0].size - 1; i > 0; i--)
		{
			Mp.mS[0].ps[i] = Mp.mS[0].ps[i - 1];
		}
		switch (Mp.mS[0].dir)
		{
		case UP:
			Mp.mS[0].ps[0].ri--;
			if (Mp.mS[0].ps[0].ri == 0)
			{
				Mp.mS[0].ps[0].ri = gameRows - 1;
			}
			break;
		case DOWN:
			Mp.mS[0].ps[0].ri++;
			if (Mp.mS[0].ps[0].ri == gameRows)
			{
				Mp.mS[0].ps[0].ri = 1;
			}
			break;
		case LEFT:
			Mp.mS[0].ps[0].ci--;
			if (Mp.mS[0].ps[0].ci == 0)
			{
				Mp.mS[0].ps[0].ci = gameCols - 1;
			}
			break;
		case RIGHT:
			Mp.mS[0].ps[0].ci++;
			if (Mp.mS[0].ps[0].ci == gameCols)
			{
				Mp.mS[0].ps[0].ci = 1;
			}
			break;
		}
	}
	//2P
	if (Mp.mS[1].isDie == false)
	{
		for (int i = Mp.mS[1].size - 1; i > 0; i--)
		{
			Mp.mS[1].ps[i] = Mp.mS[1].ps[i - 1];
		}
		switch (Mp.mS[1].dir)
		{
		case UP:
			Mp.mS[1].ps[0].ri--;
			if (Mp.mS[1].ps[0].ri == 0)
			{
				Mp.mS[1].ps[0].ri = gameRows - 1;
			}
			break;
		case DOWN:
			Mp.mS[1].ps[0].ri++;
			if (Mp.mS[1].ps[0].ri == gameRows)
			{
				Mp.mS[1].ps[0].ri = 1;
			}
			break;
		case LEFT:
			Mp.mS[1].ps[0].ci--;
			if (Mp.mS[1].ps[0].ci == 0)
			{
				Mp.mS[1].ps[0].ci = gameCols - 1;
			}
			break;
		case RIGHT:
			Mp.mS[1].ps[0].ci++;
			if (Mp.mS[1].ps[0].ci == gameCols)
			{
				Mp.mS[1].ps[0].ci = 1;
			}
			break;
		}

	}
	//3P
	if (Mp.mS[2].isDie == false)
	{
		for (int i = Mp.mS[2].size - 1; i > 0; i--)
		{
			Mp.mS[2].ps[i] = Mp.mS[2].ps[i - 1];
		}
		switch (Mp.mS[2].dir)
		{
		case UP:
			Mp.mS[2].ps[0].ri--;
			if (Mp.mS[2].ps[0].ri == 0)
			{
				Mp.mS[2].ps[0].ri = gameRows - 1;
			}
			break;
		case DOWN:
			Mp.mS[2].ps[0].ri++;
			if (Mp.mS[2].ps[0].ri == gameRows)
			{
				Mp.mS[2].ps[0].ri = 1;
			}
			break;
		case LEFT:
			Mp.mS[2].ps[0].ci--;
			if (Mp.mS[2].ps[0].ci == 0)
			{
				Mp.mS[2].ps[0].ci = gameCols - 1;
			}
			break;
		case RIGHT:
			Mp.mS[2].ps[0].ci++;
			if (Mp.mS[2].ps[0].ci == gameCols)
			{
				Mp.mS[2].ps[0].ci = 1;
			}
			break;
		}
	}
}
void threePlayerChangeDirection(threePlayerSnake& Mp, char key_pressed)
{
	//1P
	if (Mp.mS[0].isDie == false)
	{
		if (key_pressed == Mp.mS[0].UPKEY)
		{

			if (Mp.mS[0].dir != DOWN)
			{
				Mp.mS[0].dir = UP;
			}
		}
		if (key_pressed == Mp.mS[0].DOWNKEY)
		{
			if (Mp.mS[0].dir != UP)
			{
				Mp.mS[0].dir = DOWN;
			}
		}
		if (key_pressed == Mp.mS[0].LEFTKEY)
		{
			if (Mp.mS[0].dir != RIGHT)
			{
				Mp.mS[0].dir = LEFT;
			}
		}
		if (key_pressed == Mp.mS[0].RIGHTKEY)
		{
			if (Mp.mS[0].dir != LEFT)
			{
				Mp.mS[0].dir = RIGHT;
			}
		}
	}
	//2P
	if (Mp.mS[1].isDie == false)
	{
		if (key_pressed == Mp.mS[1].UPKEY)
		{

			if (Mp.mS[1].dir != DOWN)
			{
				Mp.mS[1].dir = UP;
			}
		}
		if (key_pressed == Mp.mS[1].DOWNKEY)
		{
			if (Mp.mS[1].dir != UP)
			{
				Mp.mS[1].dir = DOWN;
			}
		}
		if (key_pressed == Mp.mS[1].LEFTKEY)
		{
			if (Mp.mS[1].dir != RIGHT)
			{
				Mp.mS[1].dir = LEFT;
			}
		}
		if (key_pressed == Mp.mS[1].RIGHTKEY)
		{
			if (Mp.mS[1].dir != LEFT)
			{
				Mp.mS[1].dir = RIGHT;
			}
		}
	}
	//3P
	if (Mp.mS[2].isDie == false)
	{
		if (key_pressed == Mp.mS[2].UPKEY)
		{

			if (Mp.mS[2].dir != DOWN)
			{
				Mp.mS[2].dir = UP;
			}
		}
		if (key_pressed == Mp.mS[2].DOWNKEY)
		{
			if (Mp.mS[2].dir != UP)
			{
				Mp.mS[2].dir = DOWN;
			}
		}
		if (key_pressed == Mp.mS[2].LEFTKEY)
		{
			if (Mp.mS[2].dir != RIGHT)
			{
				Mp.mS[2].dir = LEFT;
			}
		}
		if (key_pressed == Mp.mS[2].RIGHTKEY)
		{
			if (Mp.mS[2].dir != LEFT)
			{
				Mp.mS[2].dir = RIGHT;
			}
		}
	}
}
void threePlayerGrowSnake(threePlayerSnake& Mp)
{
	//1P
	if (Mp.mS[0].isDie == false)
	{
		position* arr_1 = new position[Mp.mS[0].size + 1];
		for (int i = 0; i < Mp.mS[0].size; i++)
		{
			arr_1[i] = Mp.mS[0].ps[i];
		}
		arr_1[Mp.mS[0].size] = Mp.mS[0].ps[Mp.mS[0].size - 1];
		Mp.mS[0].size++;
		delete[] Mp.mS[0].ps;
		Mp.mS[0].ps = arr_1;
	}
	//2p
	if (Mp.mS[1].isDie == false)
	{
		position* arr_2 = new position[Mp.mS[1].size + 1];
		for (int i = 0; i < Mp.mS[1].size; i++)
		{
			arr_2[i] = Mp.mS[1].ps[i];
		}
		arr_2[Mp.mS[1].size] = Mp.mS[1].ps[Mp.mS[1].size - 1];
		Mp.mS[1].size++;
		delete[] Mp.mS[1].ps;
		Mp.mS[1].ps = arr_2;
	}
	//3P
	if (Mp.mS[2].isDie == false)
	{
		position* arr_3 = new position[Mp.mS[2].size + 1];
		for (int i = 0; i < Mp.mS[2].size; i++)
		{
			arr_3[i] = Mp.mS[2].ps[i];
		}
		arr_3[Mp.mS[2].size] = Mp.mS[2].ps[Mp.mS[2].size - 1];
		Mp.mS[2].size++;
		delete[] Mp.mS[2].ps;
		Mp.mS[2].ps = arr_3;
	}
}
bool threePlayerSelfDeath1stPlayer(threePlayerSnake Mp)
{
	if (Mp.mS[0].isDie == false)
	{
		for (int i = 1; i < Mp.mS[0].size; i++)
		{
			if (Mp.mS[0].ps[0].ri == Mp.mS[0].ps[i].ri && Mp.mS[0].ps[0].ci == Mp.mS[0].ps[i].ci)
				return true;

		}
	}
	return false;
}
bool threePlayerSelfDeath2ndPlayer(threePlayerSnake Mp)
{
	if (Mp.mS[1].isDie == false)
	{
		for (int i = 1; i < Mp.mS[1].size; i++)
		{
			if (Mp.mS[1].ps[0].ri == Mp.mS[1].ps[i].ri && Mp.mS[1].ps[0].ci == Mp.mS[1].ps[i].ci)
				return true;
		
		}
	}
	return false;
}
bool threePlayerSelfDeath3rdPlayer(threePlayerSnake Mp)
{
	if (Mp.mS[2].isDie = false)
	{
		for (int i = 1; i < Mp.mS[2].size; i++)
		{
			if (Mp.mS[2].ps[0].ri == Mp.mS[2].ps[i].ri && Mp.mS[2].ps[0].ci == Mp.mS[2].ps[i].ci)
				return true;
		}
	}
	return false;
}
bool threePlayer1stPlayerLoseCondition(threePlayerSnake Mp)
{
	if (Mp.mS[0].isDie == false && Mp.mS[1].isDie == false)
	{
		for (int i = 1; i < Mp.mS[1].size; i++)
		{
			if (Mp.mS[0].ps[0].ri == Mp.mS[1].ps[i].ri && Mp.mS[0].ps[0].ci == Mp.mS[1].ps[i].ci)
				return true;
			
		}
	}
	else if (Mp.mS[0].isDie == false && Mp.mS[2].isDie == false)
	{
		for (int i = 1; i < Mp.mS[2].size; i++)
		{
			if (Mp.mS[0].ps[0].ri == Mp.mS[2].ps[i].ri && Mp.mS[0].ps[0].ci == Mp.mS[2].ps[i].ci)
				return true;
		}
	}
	return false;
}
bool threePlayer2ndPlayerLoseCondition(threePlayerSnake Mp)
{
	if (Mp.mS[1].isDie == false && Mp.mS[0].isDie == false)
	{
		for (int i = 1; i < Mp.mS[0].size; i++)
		{
			if (Mp.mS[1].ps[0].ri == Mp.mS[0].ps[i].ri && Mp.mS[1].ps[0].ci == Mp.mS[0].ps[i].ci)
			{
				return true;
			}
		}
	}
	else if (Mp.mS[1].isDie == false && Mp.mS[2].isDie == false)
	{
		for (int i = 1; i < Mp.mS[2].size; i++)
		{
			if (Mp.mS[1].ps[0].ri == Mp.mS[2].ps[i].ri && Mp.mS[1].ps[0].ci == Mp.mS[2].ps[i].ci)
			{
				return true;
			}
		}
	
	}
	return false;
}
bool threePlayer3rdPlayerLoseCondition(threePlayerSnake Mp)
{
	if (Mp.mS[2].isDie == false && Mp.mS[0].isDie == false)
	{
		for (int i = 1; i < Mp.mS[0].size; i++)
		{
			if (Mp.mS[2].ps[0].ri == Mp.mS[0].ps[i].ri && Mp.mS[2].ps[0].ci == Mp.mS[0].ps[i].ci)
			{
				return true;
			}
		}
	}
	else if (Mp.mS[2].isDie == false && Mp.mS[1].isDie == false)
	{
		for (int i = 1; i < Mp.mS[1].size; i++)
		{
			if (Mp.mS[2].ps[0].ri == Mp.mS[1].ps[i].ri && Mp.mS[2].ps[0].ci == Mp.mS[1].ps[i].ci)
			{
				return true;
			}
		}
	}
	return false;
}
bool threePlayerDrawingCondition(threePlayerSnake &Mp)
{
	if (Mp.mS[0].isDie == false && Mp.mS[1].isDie == false)
	{
		if (Mp.mS[0].ps[0].ri == Mp.mS[1].ps[0].ri && Mp.mS[0].ps[0].ci == Mp.mS[1].ps[0].ci)
		{
			delete[]Mp.mS[0].ps;
			delete[]Mp.mS[1].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[1].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[1].size = 0;
			Mp.mS[0].isDie = true;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	if (Mp.mS[1].isDie == false && Mp.mS[0].isDie == false)
	{
		if (Mp.mS[1].ps[0].ri == Mp.mS[0].ps[0].ri && Mp.mS[1].ps[0].ci == Mp.mS[0].ps[0].ci)
		{
			delete[]Mp.mS[0].ps;
			delete[]Mp.mS[1].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[1].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[1].size = 0;
			Mp.mS[0].isDie = true;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	if (Mp.mS[0].isDie == false && Mp.mS[2].isDie == false)
	{
		if (Mp.mS[0].ps[0].ri == Mp.mS[2].ps[0].ri && Mp.mS[0].ps[0].ci == Mp.mS[2].ps[0].ci)
		{
			delete[]Mp.mS[0].ps;
			delete[]Mp.mS[2].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[2].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[2].size = 0;
			Mp.mS[0].isDie = true;
			Mp.mS[2].isDie = true;
			return true;
		}
	}
	if (Mp.mS[1].isDie == false && Mp.mS[2].isDie == false)
	{
		if (Mp.mS[1].ps[0].ri == Mp.mS[2].ps[0].ri && Mp.mS[1].ps[0].ci == Mp.mS[2].ps[0].ci)
		{
			delete[]Mp.mS[2].ps;
			delete[]Mp.mS[1].ps;
			Mp.mS[1].ps = nullptr;
			Mp.mS[2].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[1].size = 0;
			Mp.mS[2].isDie = true;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	if (Mp.mS[2].isDie == false && Mp.mS[0].isDie == false)
	{
		if (Mp.mS[2].ps[0].ri == Mp.mS[0].ps[0].ri && Mp.mS[2].ps[0].ci == Mp.mS[0].ps[0].ci)
		{
			delete[]Mp.mS[2].ps;
			delete[]Mp.mS[0].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[2].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[0].size = 0;
			Mp.mS[0].isDie = true;
			Mp.mS[2].isDie = true;
			return true;
		}
	}
	if (Mp.mS[2].isDie == false && Mp.mS[1].isDie == false)
	{
		if (Mp.mS[2].ps[0].ri == Mp.mS[1].ps[0].ri && Mp.mS[2].ps[0].ci == Mp.mS[1].ps[0].ci)
		{
			delete[]Mp.mS[2].ps;
			delete[]Mp.mS[1].ps;
			Mp.mS[1].ps = nullptr;
			Mp.mS[2].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[1].size = 0;
			Mp.mS[2].isDie = true;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	return false;
}
bool threePlayerDecision(threePlayerSnake &Mp)
{
	int p1=0, p2=0, p3=0;
	if (Mp.mS[0].isDie == false)
	{
		if (threePlayerSelfDeath1stPlayer(Mp) == true)
		{
			delete[]Mp.mS[0].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[0].isDie = true;
			return true;
		}
	}
	if (Mp.mS[0].isDie == false)
	{
		if (threePlayer1stPlayerLoseCondition(Mp) == true)
		{
			delete[]Mp.mS[0].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[0].isDie = true;
			return true;
		}
	}
	if (Mp.mS[1].isDie == false)
	{
		if (threePlayerSelfDeath2ndPlayer(Mp) == true)
		{
			delete[]Mp.mS[1].ps;
			Mp.mS[1].ps = nullptr;
			Mp.mS[1].size = 0;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	if (Mp.mS[2].isDie == false)
	{
		if (threePlayerSelfDeath3rdPlayer(Mp) == true)
		{
			delete[]Mp.mS[2].ps;
			Mp.mS[2].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[2].isDie = true;
			return true;
		}
	}
	if (Mp.mS[1].isDie == false)
	{
		if (threePlayer2ndPlayerLoseCondition(Mp) == true)
		{
			delete[]Mp.mS[1].ps;
			Mp.mS[1].ps = nullptr;
			Mp.mS[1].size = 0;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	if (Mp.mS[2].isDie == false)
	{
		if (threePlayer3rdPlayerLoseCondition(Mp) == true)
		{
			delete[]Mp.mS[2].ps;
			Mp.mS[2].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[2].isDie = true;
			return true;
		}
	}
	return false;
}
bool threePlayerIsLegalCord(position P, threePlayerSnake& Mp)
{
	bool check1 = true, check2 = true, check3 = true;
	if (Mp.mS[0].isDie == false)
	{
		for (int i = 0; i < Mp.mS[0].size; i++)
		{
			if ((Mp.mS[0].ps[i].ri == P.ri && Mp.mS[0].ps[i].ci == P.ci))/* &&
				((P.ri == 0 && P.ci <= gameCols) || (P.ri == gameRows && P.ci <= gameCols)) &&
				((P.ci == 0 && P.ri <= gameCols) || (P.ci == gameRows && P.ri <= gameCols)))*/
				check1 = false;
		}
	}
	if (Mp.mS[1].isDie == false)
	{
		for (int i = 0; i < Mp.mS[1].size; i++)
		{
			if ((Mp.mS[1].ps[i].ri == P.ri && Mp.mS[1].ps[i].ci == P.ci))/* &&
				((P.ri == 0 && P.ci <= gameCols) || (P.ri == gameRows && P.ci <= gameCols)) &&
				((P.ci == 0 && P.ri <= gameCols) || (P.ci == gameRows && P.ri <= gameCols)))*/
				check2 = false;
		}
	}
	if (Mp.mS[2].isDie == false)
	{
		for (int i = 0; i < Mp.mS[2].size; i++)
		{
			if ((Mp.mS[2].ps[i].ri == P.ri && Mp.mS[2].ps[i].ci == P.ci))/* &&
				((P.ri == 0 && P.ci <= gameCols) || (P.ri == gameRows && P.ci <= gameCols)) &&
				((P.ci == 0 && P.ri <= gameCols) || (P.ci == gameRows && P.ri <= gameCols)))*/
				check3 = false;
		}
	}
	if (check1 == false || check2 == false || check3 == false)
		return false;
	return true;
}
void threePlayerGenFood(food& f, threePlayerSnake& Mp)
{
	do {
		f.fp.ri = (rand() % (gameCols - 2) + 1);
		f.fp.ci = (rand() % (gameCols - 2) + 1);
	} while (threePlayerIsLegalCord(f.fp, Mp) == false);
	f.isAlive = true;
	f.store = 1;
}
void threePlayerDisplayFood(food f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	SetClr(9);
	cout << f.sym;
}
void threePlayerDisplayBaraFood(baraFood f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	SetClr(9);
	cout << f.sym;
}
void threePlayerEraseBaraFood(baraFood f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	cout << ' ';
}
void threePlayerGenBaraFood(baraFood& F, threePlayerSnake& Mp)
{
	do {
		F.fp.ri = (rand() % (gameCols - 2) + 1);
		F.fp.ci = (rand() % (gameCols - 2) + 1);
	} while (threePlayerIsLegalCord(F.fp, Mp) == false);
	F.isAlive = true;
	F.store = 6;
	F.isAvailable = true;
}
bool threePlayer1stPlayerEatFood(food f, threePlayerSnake& Mp)
{
	if (Mp.mS[0].isDie == false)
	{
		if (Mp.mS[0].ps[0].ri == f.fp.ri && Mp.mS[0].ps[0].ci == f.fp.ci)
		{
			position* arr_1 = new position[Mp.mS[0].size + 1];
			for (int i = 0; i < Mp.mS[0].size; i++)
			{
				arr_1[i] = Mp.mS[0].ps[i];
			}
			arr_1[Mp.mS[0].size] = Mp.mS[0].ps[Mp.mS[0].size - 1];
			Mp.mS[0].size++;
			delete[] Mp.mS[0].ps;
			Mp.mS[0].ps = arr_1;

			return true;
		}
	}
	return false;
}
bool threePlayer2ndPlayerEatFood(food f, threePlayerSnake& Mp)
{
	if (Mp.mS[1].isDie == false)
	{
		if (Mp.mS[1].ps[0].ri == f.fp.ri && Mp.mS[1].ps[0].ci == f.fp.ci)
		{
			position* arr_2 = new position[Mp.mS[1].size + 1];
			for (int i = 0; i < Mp.mS[1].size; i++)
			{
				arr_2[i] = Mp.mS[1].ps[i];
			}
			arr_2[Mp.mS[1].size] = Mp.mS[1].ps[Mp.mS[1].size - 1];
			Mp.mS[1].size++;
			delete[] Mp.mS[1].ps;
			Mp.mS[1].ps = arr_2;
			return true;
		}
	}
	return false;
}
bool threePlayer3rdPlayerEatFood(food f, threePlayerSnake& Mp)
{
	if (Mp.mS[2].isDie == false)
	{
		if (Mp.mS[2].ps[0].ri == f.fp.ri && Mp.mS[2].ps[0].ci == f.fp.ci)
		{
			position* arr_3 = new position[Mp.mS[2].size + 1];
			for (int i = 0; i < Mp.mS[2].size; i++)
			{
				arr_3[i] = Mp.mS[2].ps[i];
			}
			arr_3[Mp.mS[2].size] = Mp.mS[2].ps[Mp.mS[2].size - 1];
			Mp.mS[2].size++;
			delete[] Mp.mS[2].ps;
			Mp.mS[2].ps = arr_3;
			return true;
		}
	}
	return false;
}
bool threePlayer1stPlayerEatBaraFood(baraFood f, threePlayerSnake& Mp)
{
	if (Mp.mS[0].isDie == false)
	{
		if (Mp.mS[0].ps[0].ri == f.fp.ri && Mp.mS[0].ps[0].ci == f.fp.ci)
		{
			return true;
		}
	}
	return false;
}
bool threePlayer2ndPlayerEatBaraFood(baraFood f, threePlayerSnake& Mp)
{
	if (Mp.mS[1].isDie == false)
	{
		if (Mp.mS[1].ps[0].ri == f.fp.ri && Mp.mS[1].ps[0].ci == f.fp.ci)
		{
			return true;
		}
	}
	return false;
}
bool threePlayer3rdPlayerEatBaraFood(baraFood f, threePlayerSnake& Mp)
{
	if (Mp.mS[2].isDie == false)
	{
		if (Mp.mS[2].ps[0].ri == f.fp.ri && Mp.mS[2].ps[0].ci == f.fp.ci)
		{
			return true;
		}
	}
	return false;
}
//3Players End

// 4 Players
void fourPlayerGenBaraFood(baraFood& F, fourPlayerSnake& Mp);
void fourPlayerGenFood(food& f, fourPlayerSnake& Mp);
void fourPlayerInit(fourPlayerSnake& Mp, food& f, baraFood& F)
{
	// 1P snake
	Mp.mS[0].ps = new position[3];
	Mp.mS[0].ps[0].ri = gameRows / 2;
	Mp.mS[0].ps[0].ci = gameCols / 2;

	Mp.mS[0].ps[1].ri = gameRows / 2;
	Mp.mS[0].ps[1].ci = gameCols / 2 - 1;

	Mp.mS[0].ps[2].ri = gameRows / 2;
	Mp.mS[0].ps[2].ci = gameCols / 2 - 2;

	Mp.mS[0].size = 3;
	Mp.mS[0].score = 0;

	Mp.mS[0].p_Name = "Hamza";
	Mp.mS[0].sym = 'O';
	Mp.mS[0].dir = RIGHT;
	Mp.mS[0].RIGHTKEY = 77, Mp.mS[0].LEFTKEY = 75, Mp.mS[0].UPKEY = 72, Mp.mS[0].DOWNKEY = 80;
	Mp.mS[0].isDie = false;

	//2P smake
	Mp.mS[1].ps = new position[3];
	Mp.mS[1].ps[0].ri = gameRows / 2 - 1;
	Mp.mS[1].ps[0].ci = gameCols / 2;

	Mp.mS[1].ps[1].ri = gameRows / 2 - 1;
	Mp.mS[1].ps[1].ci = gameCols / 2 - 1;

	Mp.mS[1].ps[2].ri = gameRows / 2 - 1;
	Mp.mS[1].ps[2].ci = gameCols / 2 - 2;

	Mp.mS[1].size = 3;
	Mp.mS[1].score = 0;

	Mp.mS[1].p_Name = "Hamza";
	Mp.mS[1].sym = 'O';
	Mp.mS[1].dir = RIGHT;
	Mp.mS[1].RIGHTKEY = 100, Mp.mS[1].LEFTKEY = 97, Mp.mS[1].UPKEY = 119, Mp.mS[1].DOWNKEY = 115;
	Mp.mS[1].isDie = false;

	//3P smake
	Mp.mS[2].ps = new position[3];
	Mp.mS[2].ps[0].ri = gameRows / 2 - 2;
	Mp.mS[2].ps[0].ci = gameCols / 2;

	Mp.mS[2].ps[1].ri = gameRows / 2 - 2;
	Mp.mS[2].ps[1].ci = gameCols / 2 - 1;

	Mp.mS[2].ps[2].ri = gameRows / 2 - 2;
	Mp.mS[2].ps[2].ci = gameCols / 2 - 2;

	Mp.mS[2].size = 3;
	Mp.mS[2].score = 0;

	Mp.mS[2].p_Name = "Hamza";
	Mp.mS[2].sym = 'O';
	Mp.mS[2].dir = RIGHT;
	Mp.mS[2].RIGHTKEY = 104, Mp.mS[2].LEFTKEY = 102, Mp.mS[2].UPKEY = 116, Mp.mS[2].DOWNKEY = 103;
	Mp.mS[2].isDie = false;

	//4P smake
	Mp.mS[3].ps = new position[3];
	Mp.mS[3].ps[0].ri = gameRows / 2 - 3;
	Mp.mS[3].ps[0].ci = gameCols / 2;

	Mp.mS[3].ps[1].ri = gameRows / 2 - 3;
	Mp.mS[3].ps[1].ci = gameCols / 2 - 1;

	Mp.mS[3].ps[2].ri = gameRows / 2 - 3;
	Mp.mS[3].ps[2].ci = gameCols / 2 - 2;

	Mp.mS[3].size = 3;
	Mp.mS[3].score = 0;

	Mp.mS[3].p_Name = "Hamza";
	Mp.mS[3].sym = 'O';
	Mp.mS[3].dir = RIGHT;
	Mp.mS[3].RIGHTKEY = 108, Mp.mS[3].LEFTKEY = 106, Mp.mS[3].UPKEY = 105, Mp.mS[3].DOWNKEY = 107;
	Mp.mS[3].isDie = false;

	fourPlayerGenFood(f, Mp);
	fourPlayerGenBaraFood(F, Mp);
	f.sym = 'X';
	F.sym = 'K';
}
void fourPlayerDisplaySnake(fourPlayerSnake Mp)
{
	//1P
	if (Mp.mS[0].isDie == false)
	{
		for (int i = 0; i < Mp.mS[0].size; i++)
		{
			gotoRowCol(Mp.mS[0].ps[i].ri, Mp.mS[0].ps[i].ci);
			if (Mp.mS[0].ps[0].ri == 0 && Mp.mS[0].ps[0].ci == 0)
			{
				SetClr(4);
				cout << Mp.mS[0].sym;
			}
			else
			{
				SetClr(14);
				cout << Mp.mS[0].sym;
			}
		}
	}
	//2P 
	if (Mp.mS[1].isDie == false)
	{
		for (int i = 0; i < Mp.mS[1].size; i++)
		{
			gotoRowCol(Mp.mS[1].ps[i].ri, Mp.mS[1].ps[i].ci);
			if (Mp.mS[1].ps[0].ri == 0 && Mp.mS[1].ps[0].ci == 0)
			{
				SetClr(4);
				cout << Mp.mS[1].sym;
			}
			else
			{
				SetClr(2);
				cout << Mp.mS[1].sym;
			}
		}
	}
	//3P
	if (Mp.mS[2].isDie == false)
	{
		for (int i = 0; i < Mp.mS[2].size; i++)
		{
			gotoRowCol(Mp.mS[2].ps[i].ri, Mp.mS[2].ps[i].ci);
			if (Mp.mS[2].ps[0].ri == 0 && Mp.mS[2].ps[0].ci == 0)
			{
				SetClr(4);
				cout << Mp.mS[2].sym;
			}
			else
			{
				SetClr(3);
				cout << Mp.mS[2].sym;
			}
		}
	}
	//4P
	if (Mp.mS[3].isDie == false)
	{
		for (int i = 0; i < Mp.mS[3].size; i++)
		{
			gotoRowCol(Mp.mS[3].ps[i].ri, Mp.mS[3].ps[i].ci);
			if (Mp.mS[3].ps[0].ri == 0 && Mp.mS[3].ps[0].ci == 0)
			{
				SetClr(4);
				cout << Mp.mS[3].sym;
			}
			else
			{
				SetClr(11);
				cout << Mp.mS[3].sym;
			}
		}
	}
}
void fourPlayerEraseSnake(fourPlayerSnake Mp)
{
	//1P
	if (Mp.mS[0].isDie == false)
	{
		for (int i = 0; i < Mp.mS[0].size; i++)
		{
			gotoRowCol(Mp.mS[0].ps[i].ri, Mp.mS[0].ps[i].ci);
			cout << ' ';
		}
	}
	//2P
	if (Mp.mS[1].isDie == false)
	{
		for (int i = 0; i < Mp.mS[1].size; i++)
		{
			gotoRowCol(Mp.mS[1].ps[i].ri, Mp.mS[1].ps[i].ci);
			cout << ' ';
		}
	}
	//3P
	if (Mp.mS[2].isDie == false)
	{
		for (int i = 0; i < Mp.mS[2].size; i++)
		{
			gotoRowCol(Mp.mS[2].ps[i].ri, Mp.mS[2].ps[i].ci);
			cout << ' ';
		}
	}
	//4P
	if (Mp.mS[3].isDie == false)
	{
		for (int i = 0; i < Mp.mS[3].size; i++)
		{
			gotoRowCol(Mp.mS[3].ps[i].ri, Mp.mS[3].ps[i].ci);
			cout << ' ';
		}
	}
}
void fourPlayerMoveSnake(fourPlayerSnake& Mp)
{
	//1P
	if (Mp.mS[0].isDie == false)
	{
		for (int i = Mp.mS[0].size - 1; i > 0; i--)
		{
			Mp.mS[0].ps[i] = Mp.mS[0].ps[i - 1];
		}
		switch (Mp.mS[0].dir)
		{
		case UP:
			Mp.mS[0].ps[0].ri--;
			if (Mp.mS[0].ps[0].ri == 0)
			{
				Mp.mS[0].ps[0].ri = gameRows - 1;
			}
			break;
		case DOWN:
			Mp.mS[0].ps[0].ri++;
			if (Mp.mS[0].ps[0].ri == gameRows)
			{
				Mp.mS[0].ps[0].ri = 1;
			}
			break;
		case LEFT:
			Mp.mS[0].ps[0].ci--;
			if (Mp.mS[0].ps[0].ci == 0)
			{
				Mp.mS[0].ps[0].ci = gameCols - 1;
			}
			break;
		case RIGHT:
			Mp.mS[0].ps[0].ci++;
			if (Mp.mS[0].ps[0].ci == gameCols)
			{
				Mp.mS[0].ps[0].ci = 1;
			}
			break;
		}
	}
	//2P
	if (Mp.mS[1].isDie == false)
	{
		for (int i = Mp.mS[1].size - 1; i > 0; i--)
		{
			Mp.mS[1].ps[i] = Mp.mS[1].ps[i - 1];
		}
		switch (Mp.mS[1].dir)
		{
		case UP:
			Mp.mS[1].ps[0].ri--;
			if (Mp.mS[1].ps[0].ri == 0)
			{
				Mp.mS[1].ps[0].ri = gameRows - 1;
			}
			break;
		case DOWN:
			Mp.mS[1].ps[0].ri++;
			if (Mp.mS[1].ps[0].ri == gameRows)
			{
				Mp.mS[1].ps[0].ri = 1;
			}
			break;
		case LEFT:
			Mp.mS[1].ps[0].ci--;
			if (Mp.mS[1].ps[0].ci == 0)
			{
				Mp.mS[1].ps[0].ci = gameCols - 1;
			}
			break;
		case RIGHT:
			Mp.mS[1].ps[0].ci++;
			if (Mp.mS[1].ps[0].ci == gameCols)
			{
				Mp.mS[1].ps[0].ci = 1;
			}
			break;
		}

	}
	//3P
	if (Mp.mS[2].isDie == false)
	{
		for (int i = Mp.mS[2].size - 1; i > 0; i--)
		{
			Mp.mS[2].ps[i] = Mp.mS[2].ps[i - 1];
		}
		switch (Mp.mS[2].dir)
		{
		case UP:
			Mp.mS[2].ps[0].ri--;
			if (Mp.mS[2].ps[0].ri == 0)
			{
				Mp.mS[2].ps[0].ri = gameRows - 1;
			}
			break;
		case DOWN:
			Mp.mS[2].ps[0].ri++;
			if (Mp.mS[2].ps[0].ri == gameRows)
			{
				Mp.mS[2].ps[0].ri = 1;
			}
			break;
		case LEFT:
			Mp.mS[2].ps[0].ci--;
			if (Mp.mS[2].ps[0].ci == 0)
			{
				Mp.mS[2].ps[0].ci = gameCols - 1;
			}
			break;
		case RIGHT:
			Mp.mS[2].ps[0].ci++;
			if (Mp.mS[2].ps[0].ci == gameCols)
			{
				Mp.mS[2].ps[0].ci = 1;
			}
			break;
		}
	}


	//4P
	if (Mp.mS[3].isDie == false)
	{
		for (int i = Mp.mS[3].size - 1; i > 0; i--)
		{
			Mp.mS[3].ps[i] = Mp.mS[3].ps[i - 1];
		}
		switch (Mp.mS[3].dir)
		{
		case UP:
			Mp.mS[3].ps[0].ri--;
			if (Mp.mS[3].ps[0].ri == 0)
			{
				Mp.mS[3].ps[0].ri = gameRows - 1;
			}
			break;
		case DOWN:
			Mp.mS[3].ps[0].ri++;
			if (Mp.mS[3].ps[0].ri == gameRows)
			{
				Mp.mS[3].ps[0].ri = 1;
			}
			break;
		case LEFT:
			Mp.mS[3].ps[0].ci--;
			if (Mp.mS[3].ps[0].ci == 0)
			{
				Mp.mS[3].ps[0].ci = gameCols - 1;
			}
			break;
		case RIGHT:
			Mp.mS[3].ps[0].ci++;
			if (Mp.mS[3].ps[0].ci == gameCols)
			{
				Mp.mS[3].ps[0].ci = 1;
			}
			break;
		}
	}

}
void fourPlayerChangeDirection(fourPlayerSnake& Mp, char key_pressed)
{
	//1P
	if (Mp.mS[0].isDie == false)
	{
		if (key_pressed == Mp.mS[0].UPKEY)
		{

			if (Mp.mS[0].dir != DOWN)
			{
				Mp.mS[0].dir = UP;
			}
		}
		if (key_pressed == Mp.mS[0].DOWNKEY)
		{
			if (Mp.mS[0].dir != UP)
			{
				Mp.mS[0].dir = DOWN;
			}
		}
		if (key_pressed == Mp.mS[0].LEFTKEY)
		{
			if (Mp.mS[0].dir != RIGHT)
			{
				Mp.mS[0].dir = LEFT;
			}
		}
		if (key_pressed == Mp.mS[0].RIGHTKEY)
		{
			if (Mp.mS[0].dir != LEFT)
			{
				Mp.mS[0].dir = RIGHT;
			}
		}
	}
	//2P
	if (Mp.mS[1].isDie == false)
	{
		if (key_pressed == Mp.mS[1].UPKEY)
		{

			if (Mp.mS[1].dir != DOWN)
			{
				Mp.mS[1].dir = UP;
			}
		}
		if (key_pressed == Mp.mS[1].DOWNKEY)
		{
			if (Mp.mS[1].dir != UP)
			{
				Mp.mS[1].dir = DOWN;
			}
		}
		if (key_pressed == Mp.mS[1].LEFTKEY)
		{
			if (Mp.mS[1].dir != RIGHT)
			{
				Mp.mS[1].dir = LEFT;
			}
		}
		if (key_pressed == Mp.mS[1].RIGHTKEY)
		{
			if (Mp.mS[1].dir != LEFT)
			{
				Mp.mS[1].dir = RIGHT;
			}
		}
	}
	//3P
	if (Mp.mS[2].isDie == false)
	{
		if (key_pressed == Mp.mS[2].UPKEY)
		{

			if (Mp.mS[2].dir != DOWN)
			{
				Mp.mS[2].dir = UP;
			}
		}
		if (key_pressed == Mp.mS[2].DOWNKEY)
		{
			if (Mp.mS[2].dir != UP)
			{
				Mp.mS[2].dir = DOWN;
			}
		}
		if (key_pressed == Mp.mS[2].LEFTKEY)
		{
			if (Mp.mS[2].dir != RIGHT)
			{
				Mp.mS[2].dir = LEFT;
			}
		}
		if (key_pressed == Mp.mS[2].RIGHTKEY)
		{
			if (Mp.mS[2].dir != LEFT)
			{
				Mp.mS[2].dir = RIGHT;
			}
		}
	}
	//4P
	if (Mp.mS[3].isDie == false)
	{
		if (key_pressed == Mp.mS[3].UPKEY)
		{

			if (Mp.mS[3].dir != DOWN)
			{
				Mp.mS[3].dir = UP;
			}
		}
		if (key_pressed == Mp.mS[3].DOWNKEY)
		{
			if (Mp.mS[3].dir != UP)
			{
				Mp.mS[3].dir = DOWN;
			}
		}
		if (key_pressed == Mp.mS[3].LEFTKEY)
		{
			if (Mp.mS[3].dir != RIGHT)
			{
				Mp.mS[3].dir = LEFT;
			}
		}
		if (key_pressed == Mp.mS[3].RIGHTKEY)
		{
			if (Mp.mS[3].dir != LEFT)
			{
				Mp.mS[3].dir = RIGHT;
			}
		}
	}
}
void fourPlayerGrowSnake(fourPlayerSnake& Mp)
{
	//1P
	if (Mp.mS[0].isDie == false)
	{
		position* arr_1 = new position[Mp.mS[0].size + 1];
		for (int i = 0; i < Mp.mS[0].size; i++)
		{
			arr_1[i] = Mp.mS[0].ps[i];
		}
		arr_1[Mp.mS[0].size] = Mp.mS[0].ps[Mp.mS[0].size - 1];
		Mp.mS[0].size++;
		delete[] Mp.mS[0].ps;
		Mp.mS[0].ps = arr_1;
	}
	//2p
	if (Mp.mS[1].isDie == false)
	{
		position* arr_2 = new position[Mp.mS[1].size + 1];
		for (int i = 0; i < Mp.mS[1].size; i++)
		{
			arr_2[i] = Mp.mS[1].ps[i];
		}
		arr_2[Mp.mS[1].size] = Mp.mS[1].ps[Mp.mS[1].size - 1];
		Mp.mS[1].size++;
		delete[] Mp.mS[1].ps;
		Mp.mS[1].ps = arr_2;
	}
	//3P
	if (Mp.mS[2].isDie == false)
	{
		position* arr_3 = new position[Mp.mS[2].size + 1];
		for (int i = 0; i < Mp.mS[2].size; i++)
		{
			arr_3[i] = Mp.mS[2].ps[i];
		}
		arr_3[Mp.mS[2].size] = Mp.mS[2].ps[Mp.mS[2].size - 1];
		Mp.mS[2].size++;
		delete[] Mp.mS[2].ps;
		Mp.mS[2].ps = arr_3;
	}
}
bool fourPlayerSelfDeath1stPlayer(fourPlayerSnake Mp)
{
	if (Mp.mS[0].isDie == false)
	{
		for (int i = 1; i < Mp.mS[0].size; i++)
		{
			if (Mp.mS[0].ps[0].ri == Mp.mS[0].ps[i].ri && Mp.mS[0].ps[0].ci == Mp.mS[0].ps[i].ci)
				return true;

		}
	}
	return false;
}
bool fourPlayerSelfDeath2ndPlayer(fourPlayerSnake Mp)
{
	if (Mp.mS[1].isDie == false)
	{
		for (int i = 1; i < Mp.mS[1].size; i++)
		{
			if (Mp.mS[1].ps[0].ri == Mp.mS[1].ps[i].ri && Mp.mS[1].ps[0].ci == Mp.mS[1].ps[i].ci)
				return true;

		}
	}
	return false;
}
bool fourPlayerSelfDeath3rdPlayer(fourPlayerSnake Mp)
{
	if (Mp.mS[2].isDie = false)
	{
		for (int i = 1; i < Mp.mS[2].size; i++)
		{
			if (Mp.mS[2].ps[0].ri == Mp.mS[2].ps[i].ri && Mp.mS[2].ps[0].ci == Mp.mS[2].ps[i].ci)
				return true;
		}
	}
	return false;
}
bool fourPlayerSelfDeath4thPlayer(fourPlayerSnake Mp)
{
	if (Mp.mS[3].isDie == false)
	{
		for (int i = 1; i < Mp.mS[3].size; i++)
		{
			if (Mp.mS[3].ps[0].ri == Mp.mS[3].ps[i].ri && Mp.mS[3].ps[0].ci == Mp.mS[3].ps[i].ci)
				return true;

		}
	}
	return false;
}
bool fourPlayer1stPlayerLoseCondition(fourPlayerSnake Mp)
{
	if (Mp.mS[0].isDie == false && Mp.mS[1].isDie == false)
	{
		for (int i = 1; i < Mp.mS[1].size; i++)
		{
			if (Mp.mS[0].ps[0].ri == Mp.mS[1].ps[i].ri && Mp.mS[0].ps[0].ci == Mp.mS[1].ps[i].ci)
				return true;

		}
	}
	else if (Mp.mS[0].isDie == false && Mp.mS[2].isDie == false)
	{
		for (int i = 1; i < Mp.mS[2].size; i++)
		{
			if (Mp.mS[0].ps[0].ri == Mp.mS[2].ps[i].ri && Mp.mS[0].ps[0].ci == Mp.mS[2].ps[i].ci)
				return true;
		}
	}
	else if (Mp.mS[0].isDie == false && Mp.mS[3].isDie == false)
	{
		for (int i = 1; i < Mp.mS[3].size; i++)
		{
			if (Mp.mS[0].ps[0].ri == Mp.mS[3].ps[i].ri && Mp.mS[0].ps[0].ci == Mp.mS[3].ps[i].ci)
				return true;
		}
	}
	return false;
}
bool fourPlayer2ndPlayerLoseCondition(fourPlayerSnake Mp)
{
	if (Mp.mS[1].isDie == false && Mp.mS[0].isDie == false)
	{
		for (int i = 1; i < Mp.mS[0].size; i++)
		{
			if (Mp.mS[1].ps[0].ri == Mp.mS[0].ps[i].ri && Mp.mS[1].ps[0].ci == Mp.mS[0].ps[i].ci)
			{
				return true;
			}
		}
	}
	else if (Mp.mS[1].isDie == false && Mp.mS[2].isDie == false)
	{
		for (int i = 1; i < Mp.mS[2].size; i++)
		{
			if (Mp.mS[1].ps[0].ri == Mp.mS[2].ps[i].ri && Mp.mS[1].ps[0].ci == Mp.mS[2].ps[i].ci)
			{
				return true;
			}
		}
	}
	else if (Mp.mS[1].isDie == false && Mp.mS[3].isDie == false)
	{
		for (int i = 1; i < Mp.mS[3].size; i++)
		{
			if (Mp.mS[1].ps[0].ri == Mp.mS[3].ps[i].ri && Mp.mS[1].ps[0].ci == Mp.mS[3].ps[i].ci)
			{
				return true;
			}
		}
	}
	return false;
}
bool fourPlayer3rdPlayerLoseCondition(fourPlayerSnake Mp)
{
	if (Mp.mS[2].isDie == false && Mp.mS[0].isDie == false)
	{
		for (int i = 1; i < Mp.mS[0].size; i++)
		{
			if (Mp.mS[2].ps[0].ri == Mp.mS[0].ps[i].ri && Mp.mS[2].ps[0].ci == Mp.mS[0].ps[i].ci)
			{
				return true;
			}
		}
	}
	else if (Mp.mS[2].isDie == false && Mp.mS[1].isDie == false)
	{
		for (int i = 1; i < Mp.mS[1].size; i++)
		{
			if (Mp.mS[2].ps[0].ri == Mp.mS[1].ps[i].ri && Mp.mS[2].ps[0].ci == Mp.mS[1].ps[i].ci)
			{
				return true;
			}
		}
	}
	else if (Mp.mS[2].isDie == false && Mp.mS[3].isDie == false)
	{
		for (int i = 1; i < Mp.mS[3].size; i++)
		{
			if (Mp.mS[2].ps[0].ri == Mp.mS[3].ps[i].ri && Mp.mS[2].ps[0].ci == Mp.mS[3].ps[i].ci)
			{
				return true;
			}
		}
	}
	return false;
}
bool fourPlayer4thPlayerLoseCondition(fourPlayerSnake Mp)
{
	if (Mp.mS[3].isDie == false && Mp.mS[0].isDie == false)
	{
		for (int i = 1; i < Mp.mS[0].size; i++)
		{
			if (Mp.mS[3].ps[0].ri == Mp.mS[0].ps[i].ri && Mp.mS[3].ps[0].ci == Mp.mS[0].ps[i].ci)
			{
				return true;
			}
		}
	}
	else if (Mp.mS[3].isDie == false && Mp.mS[1].isDie == false)
	{
		for (int i = 1; i < Mp.mS[1].size; i++)
		{
			if (Mp.mS[3].ps[0].ri == Mp.mS[1].ps[i].ri && Mp.mS[3].ps[0].ci == Mp.mS[1].ps[i].ci)
			{
				return true;
			}
		}
	}
	else if (Mp.mS[3].isDie == false && Mp.mS[2].isDie == false)
	{
		for (int i = 1; i < Mp.mS[2].size; i++)
		{
			if (Mp.mS[3].ps[0].ri == Mp.mS[2].ps[i].ri && Mp.mS[3].ps[0].ci == Mp.mS[2].ps[i].ci)
			{
				return true;
			}
		}
	}
	return false;
}
bool fourPlayerDrawingCondition(fourPlayerSnake& Mp)
{
	// 1-->2
	if (Mp.mS[0].isDie == false && Mp.mS[1].isDie == false)
	{
		if (Mp.mS[0].ps[0].ri == Mp.mS[1].ps[0].ri && Mp.mS[0].ps[0].ci == Mp.mS[1].ps[0].ci)
		{
			delete[]Mp.mS[0].ps;
			delete[]Mp.mS[1].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[1].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[1].size = 0;
			Mp.mS[0].isDie = true;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	// 1-->3
	if (Mp.mS[0].isDie == false && Mp.mS[2].isDie == false)
	{
		if (Mp.mS[0].ps[0].ri == Mp.mS[2].ps[0].ri && Mp.mS[0].ps[0].ci == Mp.mS[2].ps[0].ci)
		{
			delete[]Mp.mS[0].ps;
			delete[]Mp.mS[2].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[2].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[2].size = 0;
			Mp.mS[0].isDie = true;
			Mp.mS[2].isDie = true;
			return true;
		}
	}
	// 1-->4
	if (Mp.mS[0].isDie == false && Mp.mS[3].isDie == false)
	{
		if (Mp.mS[0].ps[0].ri == Mp.mS[3].ps[0].ri && Mp.mS[0].ps[0].ci == Mp.mS[3].ps[0].ci)
		{
			delete[]Mp.mS[0].ps;
			delete[]Mp.mS[3].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[3].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[3].size = 0;
			Mp.mS[0].isDie = true;
			Mp.mS[3].isDie = true;
			return true;
		}
	}
	// 2-->1
	if (Mp.mS[1].isDie == false && Mp.mS[0].isDie == false)
	{
		if (Mp.mS[1].ps[0].ri == Mp.mS[0].ps[0].ri && Mp.mS[1].ps[0].ci == Mp.mS[0].ps[0].ci)
		{
			delete[]Mp.mS[0].ps;
			delete[]Mp.mS[1].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[1].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[1].size = 0;
			Mp.mS[0].isDie = true;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	// 2-->3
	if (Mp.mS[1].isDie == false && Mp.mS[2].isDie == false)
	{
		if (Mp.mS[1].ps[0].ri == Mp.mS[2].ps[0].ri && Mp.mS[1].ps[0].ci == Mp.mS[2].ps[0].ci)
		{
			delete[]Mp.mS[2].ps;
			delete[]Mp.mS[1].ps;
			Mp.mS[1].ps = nullptr;
			Mp.mS[2].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[1].size = 0;
			Mp.mS[2].isDie = true;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	// 2-->4
	if (Mp.mS[1].isDie == false && Mp.mS[3].isDie == false)
	{
		if (Mp.mS[1].ps[0].ri == Mp.mS[3].ps[0].ri && Mp.mS[1].ps[0].ci == Mp.mS[3].ps[0].ci)
		{
			delete[]Mp.mS[3].ps;
			delete[]Mp.mS[1].ps;
			Mp.mS[1].ps = nullptr;
			Mp.mS[3].ps = nullptr;
			Mp.mS[3].size = 0;
			Mp.mS[1].size = 0;
			Mp.mS[3].isDie = true;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	// 3-->1
	if (Mp.mS[2].isDie == false && Mp.mS[0].isDie == false)
	{
		if (Mp.mS[2].ps[0].ri == Mp.mS[0].ps[0].ri && Mp.mS[2].ps[0].ci == Mp.mS[0].ps[0].ci)
		{
			delete[]Mp.mS[2].ps;
			delete[]Mp.mS[0].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[2].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[0].size = 0;
			Mp.mS[0].isDie = true;
			Mp.mS[2].isDie = true;
			return true;
		}
	}
	// 3-->2
	if (Mp.mS[2].isDie == false && Mp.mS[1].isDie == false)
	{
		if (Mp.mS[2].ps[0].ri == Mp.mS[1].ps[0].ri && Mp.mS[2].ps[0].ci == Mp.mS[1].ps[0].ci)
		{
			delete[]Mp.mS[2].ps;
			delete[]Mp.mS[1].ps;
			Mp.mS[1].ps = nullptr;
			Mp.mS[2].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[1].size = 0;
			Mp.mS[2].isDie = true;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	// 3-->4
	if (Mp.mS[2].isDie == false && Mp.mS[3].isDie == false)
	{
		if (Mp.mS[2].ps[0].ri == Mp.mS[3].ps[0].ri && Mp.mS[2].ps[0].ci == Mp.mS[3].ps[0].ci)
		{
			delete[]Mp.mS[2].ps;
			delete[]Mp.mS[3].ps;
			Mp.mS[3].ps = nullptr;
			Mp.mS[2].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[3].size = 0;
			Mp.mS[2].isDie = true;
			Mp.mS[3].isDie = true;
			return true;
		}
	}
	// 4-->1
	if (Mp.mS[3].isDie == false && Mp.mS[0].isDie == false)
	{
		if (Mp.mS[3].ps[0].ri == Mp.mS[0].ps[0].ri && Mp.mS[3].ps[0].ci == Mp.mS[0].ps[0].ci)
		{
			delete[]Mp.mS[0].ps;
			delete[]Mp.mS[3].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[3].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[3].size = 0;
			Mp.mS[0].isDie = true;
			Mp.mS[3].isDie = true;
			return true;
		}
	}
	// 4-->2
	if (Mp.mS[3].isDie == false && Mp.mS[1].isDie == false)
	{
		if (Mp.mS[3].ps[0].ri == Mp.mS[1].ps[0].ri && Mp.mS[3].ps[0].ci == Mp.mS[1].ps[0].ci)
		{
			delete[]Mp.mS[1].ps;
			delete[]Mp.mS[3].ps;
			Mp.mS[1].ps = nullptr;
			Mp.mS[3].ps = nullptr;
			Mp.mS[1].size = 0;
			Mp.mS[3].size = 0;
			Mp.mS[1].isDie = true;
			Mp.mS[3].isDie = true;
			return true;
		}
	}
	// 4-->3
	if (Mp.mS[3].isDie == false && Mp.mS[2].isDie == false)
	{
		if (Mp.mS[3].ps[0].ri == Mp.mS[2].ps[0].ri && Mp.mS[3].ps[0].ci == Mp.mS[2].ps[0].ci)
		{
			delete[]Mp.mS[2].ps;
			delete[]Mp.mS[3].ps;
			Mp.mS[2].ps = nullptr;
			Mp.mS[3].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[3].size = 0;
			Mp.mS[2].isDie = true;
			Mp.mS[3].isDie = true;
			return true;
		}
	}
	return false;
}
bool fourPlayerDecision(fourPlayerSnake& Mp)
{
	int p1 = 0, p2 = 0, p3 = 0;
	if (Mp.mS[0].isDie == false)
	{
		if (fourPlayerSelfDeath1stPlayer(Mp) == true)
		{
			delete[]Mp.mS[0].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[0].isDie = true;
			return true;
		}
	}
	if (Mp.mS[0].isDie == false)
	{
		if (fourPlayer1stPlayerLoseCondition(Mp) == true)
		{
			delete[]Mp.mS[0].ps;
			Mp.mS[0].ps = nullptr;
			Mp.mS[0].size = 0;
			Mp.mS[0].isDie = true;
			return true;
		}
	}
	if (Mp.mS[1].isDie == false)
	{
		if (fourPlayerSelfDeath2ndPlayer(Mp) == true)
		{
			delete[]Mp.mS[1].ps;
			Mp.mS[1].ps = nullptr;
			Mp.mS[1].size = 0;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	if (Mp.mS[2].isDie == false)
	{
		if (fourPlayerSelfDeath3rdPlayer(Mp) == true)
		{
			delete[]Mp.mS[2].ps;
			Mp.mS[2].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[2].isDie = true;
			return true;
		}
	}
	if (Mp.mS[3].isDie == false)
	{
		if (fourPlayerSelfDeath4thPlayer(Mp) == true)
		{
			delete[]Mp.mS[3].ps;
			Mp.mS[3].ps = nullptr;
			Mp.mS[3].size = 0;
			Mp.mS[3].isDie = true;
			return true;
		}
	}

	if (Mp.mS[1].isDie == false)
	{
		if (fourPlayer2ndPlayerLoseCondition(Mp) == true)
		{
			delete[]Mp.mS[1].ps;
			Mp.mS[1].ps = nullptr;
			Mp.mS[1].size = 0;
			Mp.mS[1].isDie = true;
			return true;
		}
	}
	if (Mp.mS[2].isDie == false)
	{
		if (fourPlayer3rdPlayerLoseCondition(Mp) == true)
		{
			delete[]Mp.mS[2].ps;
			Mp.mS[2].ps = nullptr;
			Mp.mS[2].size = 0;
			Mp.mS[2].isDie = true;
			return true;
		}
	}
	if (Mp.mS[3].isDie == false)
	{
		if (fourPlayer4thPlayerLoseCondition(Mp) == true)
		{
			delete[]Mp.mS[3].ps;
			Mp.mS[3].ps = nullptr;
			Mp.mS[3].size = 0;
			Mp.mS[3].isDie = true;
			return true;
		}
	}
	return false;
}
bool fourPlayerIsLegalCord(position P, fourPlayerSnake& Mp)
{
	bool check1 = true, check2 = true, check3 = true, check4 = true;
	if (Mp.mS[0].isDie == false)
	{
		for (int i = 0; i < Mp.mS[0].size; i++)
		{
			if ((Mp.mS[0].ps[i].ri == P.ri && Mp.mS[0].ps[i].ci == P.ci))/* &&
				((P.ri == 0 && P.ci <= gameCols) || (P.ri == gameRows && P.ci <= gameCols)) &&
				((P.ci == 0 && P.ri <= gameCols) || (P.ci == gameRows && P.ri <= gameCols)))*/
				check1 = false;
		}
	}
	if (Mp.mS[1].isDie == false)
	{
		for (int i = 0; i < Mp.mS[1].size; i++)
		{
			if ((Mp.mS[1].ps[i].ri == P.ri && Mp.mS[1].ps[i].ci == P.ci))/* &&
				((P.ri == 0 && P.ci <= gameCols) || (P.ri == gameRows && P.ci <= gameCols)) &&
				((P.ci == 0 && P.ri <= gameCols) || (P.ci == gameRows && P.ri <= gameCols)))*/
					check2= false;
		}
	}
	if (Mp.mS[2].isDie == false)
	{
		for (int i = 0; i < Mp.mS[2].size; i++)
		{
			if ((Mp.mS[2].ps[i].ri == P.ri && Mp.mS[2].ps[i].ci == P.ci ))/* &&
				((P.ri == 0 && P.ci <= gameCols) || (P.ri == gameRows && P.ci <= gameCols)) &&
				((P.ci == 0 && P.ri <= gameCols) || (P.ci == gameRows && P.ri <= gameCols)))*/
					check3=false;
		}
	}
	if (Mp.mS[3].isDie == false)
	{
		for (int i = 0; i < Mp.mS[3].size; i++)
		{
			if ((Mp.mS[3].ps[i].ri == P.ri && Mp.mS[3].ps[i].ci == P.ci))/* &&
				((P.ri == 0 && P.ci <= gameCols) || (P.ri == gameRows && P.ci <= gameCols)) &&
				((P.ci == 0 && P.ri <= gameCols) || (P.ci == gameRows && P.ri <= gameCols)))*/
					check4=false;
		}
	}
	if (check1 == false || check2 == false || check3 == false || check4 == false)
		return false;
	return true;
}
void fourPlayerGenFood(food& f, fourPlayerSnake& Mp)
{
	do {
		f.fp.ri = (rand() % (gameCols - 2) + 1);
		f.fp.ci = (rand() % (gameCols - 2) + 1);
	} while (fourPlayerIsLegalCord(f.fp, Mp) == false);
	
	f.isAlive = true;
	f.store = 1;
}
void fourPlayerGenBaraFood(baraFood& F, fourPlayerSnake& Mp)
{
	do {
		F.fp.ri = (rand() % (gameCols - 2) + 1);
		F.fp.ci = (rand() % (gameCols - 2) + 1);
	} while (fourPlayerIsLegalCord(F.fp, Mp) == false);
	F.isAlive = true;
	F.store = 6;
	F.isAvailable = true;
}
void fourPlayerDisplayFood(food f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	SetClr(9);
	cout << f.sym;
}
void fourPlayerDisplayBaraFood(baraFood f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	SetClr(9);
	cout << f.sym;
}
void fourPlayerEraseBaraFood(baraFood f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	cout << ' ';
}
bool fourPlayer1stPlayerEatFood(food f, fourPlayerSnake& Mp)
{
	if (Mp.mS[0].isDie == false)
	{
		if (Mp.mS[0].ps[0].ri == f.fp.ri && Mp.mS[0].ps[0].ci == f.fp.ci)
		{
			position* arr_1 = new position[Mp.mS[0].size + 1];
			for (int i = 0; i < Mp.mS[0].size; i++)
			{
				arr_1[i] = Mp.mS[0].ps[i];
			}
			arr_1[Mp.mS[0].size] = Mp.mS[0].ps[Mp.mS[0].size - 1];
			Mp.mS[0].size++;
			delete[] Mp.mS[0].ps;
			Mp.mS[0].ps = arr_1;

			return true;
		}
	}
	return false;
}
bool fourPlayer2ndPlayerEatFood(food f, fourPlayerSnake& Mp)
{
	if (Mp.mS[1].isDie == false)
	{
		if (Mp.mS[1].ps[0].ri == f.fp.ri && Mp.mS[1].ps[0].ci == f.fp.ci)
		{
			position* arr_2 = new position[Mp.mS[1].size + 1];
			for (int i = 0; i < Mp.mS[1].size; i++)
			{
				arr_2[i] = Mp.mS[1].ps[i];
			}
			arr_2[Mp.mS[1].size] = Mp.mS[1].ps[Mp.mS[1].size - 1];
			Mp.mS[1].size++;
			delete[] Mp.mS[1].ps;
			Mp.mS[1].ps = arr_2;
			return true;
		}
	}
	return false;
}
bool fourPlayer3rdPlayerEatFood(food f, fourPlayerSnake& Mp)
{
	if (Mp.mS[2].isDie == false)
	{
		if (Mp.mS[2].ps[0].ri == f.fp.ri && Mp.mS[2].ps[0].ci == f.fp.ci)
		{
			position* arr_3 = new position[Mp.mS[2].size + 1];
			for (int i = 0; i < Mp.mS[2].size; i++)
			{
				arr_3[i] = Mp.mS[2].ps[i];
			}
			arr_3[Mp.mS[2].size] = Mp.mS[2].ps[Mp.mS[2].size - 1];
			Mp.mS[2].size++;
			delete[] Mp.mS[2].ps;
			Mp.mS[2].ps = arr_3;
			return true;
		}
	}
	return false;
}
bool fourPlayer4thPlayerEatFood(food f, fourPlayerSnake& Mp)
{
	if (Mp.mS[3].isDie == false)
	{
		if (Mp.mS[3].ps[0].ri == f.fp.ri && Mp.mS[3].ps[0].ci == f.fp.ci)
		{
			position* arr_3 = new position[Mp.mS[3].size + 1];
			for (int i = 0; i < Mp.mS[3].size; i++)
			{
				arr_3[i] = Mp.mS[3].ps[i];
			}
			arr_3[Mp.mS[3].size] = Mp.mS[3].ps[Mp.mS[3].size - 1];
			Mp.mS[3].size++;
			delete[] Mp.mS[3].ps;
			Mp.mS[3].ps = arr_3;
			return true;
		}
	}
	return false;
}
bool fourPlayer1stPlayerEatBaraFood(baraFood f, fourPlayerSnake& Mp)
{
	if (Mp.mS[0].isDie == false)
	{
		if (Mp.mS[0].ps[0].ri == f.fp.ri && Mp.mS[0].ps[0].ci == f.fp.ci)
		{
			return true;
		}
	}
	return false;
}
bool fourPlayer2ndPlayerEatBaraFood(baraFood f, fourPlayerSnake& Mp)
{
	if (Mp.mS[1].isDie == false)
	{
		if (Mp.mS[1].ps[0].ri == f.fp.ri && Mp.mS[1].ps[0].ci == f.fp.ci)
		{
			return true;
		}
	}
	return false;
}
bool fourPlayer3rdPlayerEatBaraFood(baraFood f, fourPlayerSnake& Mp)
{
	if (Mp.mS[2].isDie == false)
	{
		if (Mp.mS[2].ps[0].ri == f.fp.ri && Mp.mS[2].ps[0].ci == f.fp.ci)
		{
			return true;
		}
	}
	return false;
}
bool fourPlayer4thPlayerEatBaraFood(baraFood f, fourPlayerSnake& Mp)
{
	if (Mp.mS[3].isDie == false)
	{
		if (Mp.mS[3].ps[0].ri == f.fp.ri && Mp.mS[3].ps[0].ci == f.fp.ci)
		{
			return true;
		}
	}
	return false;
}


int one()
{
	system("cls");
	srand(time(0));
	snake S;
	food f;
	baraFood F;
	char ch;
	int count = 0;
	init(S,f);
	 
	singleGenBaraFood(F, S);
	consoleScrnInfinity();
	displayFood(f);

	while (true)
	{
		gotoRowCol(0, 81);
		cout << "Score: " << S.score;
		if (_kbhit())
		{
			ch = _getch();
			if (ch == -32)
			{
				ch = _getch();
				changeDirection(S, ch);
			}
		}
		displaySnake(S);
		Sleep(100);
		removeSnake(S);
		moveSnake(S);
		if (eatFood(f, S) == true)
		{
			 
			S.score++;
			genFood(f, S);
			displayFood(f);
			snakeExpand(S);
		}
		if (count == 30)
		{
			singleGenBaraFood(F, S);
			twoPlayerDisplayBaraFood(F);
		}
		if (eatBaraFood(F, S) == true)
		{
			S.score += 6;
		}
		if (count == 80)
		{
			twoPlayerEraseBaraFood(F);
			count = 0;
		}
		count++;
		if (selfDeath(S) == true)
			break;
	}
	system("cls");
	gotoRowCol(gameRows / 2, gameCols / 2);
	cout << "Game Over........";
	 
	_getch();
	return 0;
}
int two()
{
	system("cls");
	srand(time(0));
	int count = 1;
	food f;
	baraFood F;
	twoPlayerSnake Mp;
	char chr;
	twoPlayerInit(Mp,f);
	 
	consoleScrnInfinity();
	twoPlayerGenBaraFood(F, Mp);
	twoPlayerGenFood(f, Mp);
	twoPlayerDisplayFood(f);
	while (true)
	{
		gotoRowCol(20, 85);
		cout << "Player 1 score: " << Mp.mS[0].score;
		gotoRowCol(21, 85);
		cout << "Player 2 score: " << Mp.mS[1].score;
		if (_kbhit())
		{

			chr = _getch();
			if (chr == -32)
			{
				chr = _getch();
				twoPlayerChangeDirection(Mp, chr);
			}
			twoPlayerChangeDirection(Mp, chr);
		}
		twoPlayerDisplaySnake(Mp);
		Sleep(100);
		twoPlayerEraseSnake(Mp);
		twoPlayerMoveSnake(Mp);
		if (twoPlayerEat1stPlayerFood(f, Mp) == true)
		{
			 
			twoPlayerGenFood(f, Mp);
			twoPlayerDisplayFood(f);
			Mp.mS[0].score+=f.store;
		}
		if (twoPlayerEat2ndPlayerFood(f, Mp) == true)
		{
			 
			twoPlayerGenFood(f, Mp);
			twoPlayerDisplayFood(f);
			Mp.mS[1].score+=f.store;
		}
		if (count == 50)
		{
			twoPlayerGenBaraFood(F, Mp);
			twoPlayerDisplayBaraFood(F);
		}
		if (twoPlayer1stPlayerEatBaraFood(F, Mp) == true)
		{
			Mp.mS[0].score += 6;
		}
		if (twoPlayer2ndPlayerEatBaraFood(F, Mp) == true)
		{
			Mp.mS[1].score += 6;
		}
		if (count == 150)
		{
			twoPlayerEraseBaraFood(F);
			count = 0;
		}
		if (twoPlayerDecision(Mp) == true)
			break;

		count++;
	}
	_getch();
	return 0;
}
int three()
{
	system("cls");
	srand(time(0));
	food f;
	int count = 1, winCount = 0, draw = 0;
	threePlayerSnake Mp;
	baraFood F;
	char chr;
	threePlayerInit(Mp,f);
	consoleScrnInfinity();
	 
	threePlayerGenBaraFood(F, Mp);
	threePlayerDisplayFood(f);
	while (true)
	{
		gotoRowCol(20, 85);
		cout << "Player 1 score: " << Mp.mS[0].score;
		gotoRowCol(21, 85);
		cout << "Player 2 score: " << Mp.mS[1].score;
		gotoRowCol(22, 85);
		cout << "Player 3 score: " << Mp.mS[2].score;
		if (_kbhit())
		{

			chr = _getch();
			if (chr == -32)
			{
				chr = _getch();
				threePlayerChangeDirection(Mp, chr);
			}
			threePlayerChangeDirection(Mp, chr);
		}
		threePlayerDisplaySnake(Mp);
		Sleep(100);
		threePlayerEraseSnake(Mp);
		threePlayerMoveSnake(Mp);
		// Food Conditions
		// vvvvvvvvvvvvvvv
		if (threePlayer1stPlayerEatFood(f, Mp) == true)
		{
			 
			threePlayerGenFood(f, Mp);
			threePlayerDisplayFood(f);
			Mp.mS[0].score++;
		}
		if (threePlayer2ndPlayerEatFood(f, Mp) == true)
		{
			 
			threePlayerGenFood(f, Mp);
			threePlayerDisplayFood(f);
			Mp.mS[1].score++;
		}
		if (threePlayer3rdPlayerEatFood(f, Mp) == true)
		{
			 
			threePlayerGenFood(f, Mp);
			threePlayerDisplayFood(f);
			Mp.mS[2].score++;
		}
		// ^^^^^^^^^^^^^^
		// Decision Making
		// vvvvvvvvvvvvvvv
		if (threePlayerDecision(Mp) == true)
		{
			winCount++;
			if (winCount == 2)
				break;
		}
		if (threePlayerDrawingCondition(Mp) == true)
			draw++;
		if (draw == 1)
			break;
		if (count == 50)
		{
			threePlayerGenBaraFood(F, Mp);
			threePlayerDisplayBaraFood(F);
		}
		if (threePlayer1stPlayerEatBaraFood(F, Mp) == true)
		{
			 
			Mp.mS[0].score += F.store;
		}
		if (threePlayer2ndPlayerEatBaraFood(F, Mp) == true)
		{
			 
			Mp.mS[1].score += F.store;
		}
		if (threePlayer3rdPlayerEatBaraFood(F, Mp) == true)
		{
			 
			Mp.mS[2].score += F.store;
		}
		if (count == 150)
		{
			threePlayerEraseBaraFood(F);
			count = 0;
		}
		// ^^^^^^^^^^^^^^^^
		count++;
	}
	if (Mp.mS[0].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 1 has Won the game--------------------\n\n";
		cout << "-------------------------------------------\n\n";
	}
	else if (Mp.mS[1].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 2 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	else if (Mp.mS[2].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 3 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	_getch();
	return 0;
}
int four()
{
	system("cls");
	srand(time(0));

	food f;
	baraFood F;
	int count = 0, winCount = 0, draw = 0;
	fourPlayerSnake Mp;
	char chr;
	 
	fourPlayerGenBaraFood(F, Mp);
	fourPlayerInit(Mp, f,F);
	consoleScrnInfinity();
	
	fourPlayerDisplayFood(f);
	while (true)
	{
		gotoRowCol(20, 85);
		cout << "Player 1 score: " << Mp.mS[0].score;
		gotoRowCol(21, 85);
		cout << "Player 2 score: " << Mp.mS[1].score;
		gotoRowCol(22, 85);
		cout << "Player 3 score: " << Mp.mS[2].score;
		gotoRowCol(23, 85);
		cout << "Player 4 score: " << Mp.mS[3].score;
		if (_kbhit())
		{

			chr = _getch();
			if (chr == -32)
			{
				chr = _getch();
				fourPlayerChangeDirection(Mp, chr);
			}
			fourPlayerChangeDirection(Mp, chr);
		}
		fourPlayerDisplaySnake(Mp);
		Sleep(100);
		fourPlayerEraseSnake(Mp);
		fourPlayerMoveSnake(Mp);
		// Food Conditions
		// vvvvvvvvvvvvvvv
		if (fourPlayer1stPlayerEatFood(f, Mp) == true)
		{
			 
			fourPlayerGenFood(f, Mp);
			fourPlayerDisplayFood(f);
			Mp.mS[0].score++;
		}
		if (fourPlayer2ndPlayerEatFood(f, Mp) == true)
		{
			 
			fourPlayerGenFood(f, Mp);
			fourPlayerDisplayFood(f);
			Mp.mS[1].score++;
		}
		if (fourPlayer3rdPlayerEatFood(f, Mp) == true)
		{
			 
			fourPlayerGenFood(f, Mp);
			fourPlayerDisplayFood(f);
			Mp.mS[2].score++;
		}
		if (fourPlayer4thPlayerEatFood(f, Mp) == true)
		{
			 
			fourPlayerGenFood(f, Mp);
			fourPlayerDisplayFood(f);
			Mp.mS[3].score++;
		}
		if (count == 50)
		{
			fourPlayerGenBaraFood(F, Mp);
			fourPlayerDisplayBaraFood(F);
		}
			if (fourPlayer1stPlayerEatBaraFood(F, Mp) == true)
			{
				 
				Mp.mS[0].score += F.store;
				F.isAvailable = false;
			}
			if (fourPlayer2ndPlayerEatBaraFood(F, Mp) == true)
			{
				 
				Mp.mS[1].score += F.store;
				F.isAvailable = false;
			}
				if (fourPlayer3rdPlayerEatBaraFood(F, Mp) == true)
				{
					 
					Mp.mS[2].score += F.store;
					F.isAvailable = false;
				}
				if (fourPlayer4thPlayerEatBaraFood(F, Mp) == true)
				{
					 
					Mp.mS[3].score += F.store;
					F.isAvailable = false;
				}
			
		
		if (count == 150)
		{
			fourPlayerEraseBaraFood(F);
			count = 0;
		}
		// ^^^^^^^^^^^^^^
		// Decision Making
		// vvvvvvvvvvvvvvv
		if (fourPlayerDecision(Mp) == true)
		{
			winCount++;
			if (winCount == 3)
				break;
		}
		if (fourPlayerDrawingCondition(Mp) == true)
			draw++;
		if (draw == 2)
			break;
		// ^^^^^^^^^^^^^^^^
		count++;
	}
	if (Mp.mS[0].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 1 has Won the game--------------------\n\n";
		cout << "-------------------------------------------\n\n";
	}
	else if (Mp.mS[1].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 2 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	else if (Mp.mS[2].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 3 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	else if (Mp.mS[3].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 4 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	else if (draw == 2)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Draw--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	_getch();
	return 0;

}
int oneTimed()
{
	system("cls");
	srand(time(0));
	snake S;
	food f;
	baraFood F;
	char ch;
	int count = 0;
	init(S, f);
	 
	singleGenBaraFood(F, S);
	consoleScrnInfinity();
	displayFood(f);

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < 10; k++)
			{
				gotoRowCol(0, 81);
				cout << "Time: " << i;
				gotoRowCol(1, 81);
				cout << "Score: " << S.score;
				if (_kbhit())
				{
					ch = _getch();
					if (ch == -32)
					{
						ch = _getch();
						changeDirection(S, ch);
					}
				}
				displaySnake(S);
				Sleep(100);
				removeSnake(S);
				moveSnake(S);
				if (eatFood(f, S) == true)
				{
					 
					S.score++;
					genFood(f, S);
					displayFood(f);
					snakeExpand(S);
				}
				if (selfDeath(S) == true)
					break;
				if (count == 30)
				{
					singleGenBaraFood(F, S);
					twoPlayerDisplayBaraFood(F);
				}
				if (eatBaraFood(F, S) == true)
				{
					 
					S.score += 6;
				}
				if (count == 80)
				{
					twoPlayerEraseBaraFood(F);
					count = 0;
				}
				count++;
			}
		}
	}
	system("cls");
	gotoRowCol(gameRows / 2, gameCols / 2);
	cout << "Game Over........";

	_getch();
	return 0;
}
int twoTime()
{
	system("cls");
	srand(time(0));
	int count = 1;
	food f;
	baraFood F;
	twoPlayerSnake Mp;
	char chr;
	 
	twoPlayerGenBaraFood(F, Mp);
	twoPlayerInit(Mp, f);
	consoleScrnInfinity();
	twoPlayerGenFood(f, Mp);
	
	//mciSendString(TEXT("bgmusic.wav"), NULL, 0, NULL);
	twoPlayerDisplayFood(f);
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < 10; k++)
			{
				gotoRowCol(20, 85);
				cout << "Player 1 score: " << Mp.mS[0].score;
				gotoRowCol(21, 85);
				cout << "Player 2 score: " << Mp.mS[1].score;
				gotoRowCol(22, 85);
				cout << "Time: " << i;
				if (_kbhit())
				{

					chr = _getch();
					if (chr == -32)
					{
						chr = _getch();
						twoPlayerChangeDirection(Mp, chr);
					}
					twoPlayerChangeDirection(Mp, chr);
				}
				twoPlayerDisplaySnake(Mp);
				Sleep(100);
				twoPlayerEraseSnake(Mp);
				twoPlayerMoveSnake(Mp);
				if (twoPlayerEat1stPlayerFood(f, Mp) == true)
				{
					 
					twoPlayerGenFood(f, Mp);
					twoPlayerDisplayFood(f);
					Mp.mS[0].score += f.store;
				}
				if (twoPlayerEat2ndPlayerFood(f, Mp) == true)
				{
					 
					twoPlayerGenFood(f, Mp);
					twoPlayerDisplayFood(f);
					Mp.mS[1].score += f.store;
				}
				if (count == 50)
				{
					twoPlayerGenBaraFood(F, Mp);
					twoPlayerDisplayBaraFood(F);
				}
				if (twoPlayer1stPlayerEatBaraFood(F, Mp) == true)
				{
					Mp.mS[0].score +=6;
				}
				if (twoPlayer2ndPlayerEatBaraFood(F, Mp) == true)
				{
					Mp.mS[1].score +=6;
				}
				if (count == 150)
				{
					twoPlayerEraseBaraFood(F);
					count = 0;
				}
				if (twoPlayerDecision(Mp) == true)
					break;

				count++;
			}
		}
	}
	if (Mp.mS[0].score < Mp.mS[1].score)
	{
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 2 has Won the game--------------------\n\n";
		cout << "-------------------------------------\n\n";
	}
	else
	{
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 1 has Won the game--------------------\n\n";
		cout << "-------------------------------------\n\n";
	}
	_getch();
	return 0;
}
int threeTime()
{
	system("cls");
	srand(time(0));
	food f;
	int count = 1, winCount = 0, draw = 0;
	threePlayerSnake Mp;
	baraFood F;
	char chr;
	 
	threePlayerInit(Mp, f);
	consoleScrnInfinity();
	threePlayerGenBaraFood(F, Mp);
	threePlayerDisplayFood(f);
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < 10; k++)
			{
				gotoRowCol(20, 85);
				cout << "Player 1 score: " << Mp.mS[0].score;
				gotoRowCol(21, 85);
				cout << "Player 2 score: " << Mp.mS[1].score;
				gotoRowCol(22, 85);
				cout << "Player 3 score: " << Mp.mS[2].score;
				gotoRowCol(23, 85);
				cout << "Time: " << i;
				if (_kbhit())
				{

					chr = _getch();
					if (chr == -32)
					{
						chr = _getch();
						threePlayerChangeDirection(Mp, chr);
					}
					threePlayerChangeDirection(Mp, chr);
				}
				threePlayerDisplaySnake(Mp);
				Sleep(100);
				threePlayerEraseSnake(Mp);
				threePlayerMoveSnake(Mp);
				// Food Conditions
				// vvvvvvvvvvvvvvv
				if (threePlayer1stPlayerEatFood(f, Mp) == true)
				{
					 
					threePlayerGenFood(f, Mp);
					threePlayerDisplayFood(f);
					Mp.mS[0].score++;
				}
				if (threePlayer2ndPlayerEatFood(f, Mp) == true)
				{
					 
					threePlayerGenFood(f, Mp);
					threePlayerDisplayFood(f);
					Mp.mS[1].score++;
				}
				if (threePlayer3rdPlayerEatFood(f, Mp) == true)
				{
					 
					threePlayerGenFood(f, Mp);
					threePlayerDisplayFood(f);
					Mp.mS[2].score++;
				}
				// ^^^^^^^^^^^^^^
				// Decision Making
				// vvvvvvvvvvvvvvv
				if (threePlayerDecision(Mp) == true)
				{
					winCount++;
					if (winCount == 2)
						break;
				}
				if (threePlayerDrawingCondition(Mp) == true)
					draw++;
				if (draw == 1)
					break;
				if (count == 50)
				{
					threePlayerGenBaraFood(F, Mp);
					threePlayerDisplayBaraFood(F);
				}
				if (threePlayer1stPlayerEatBaraFood(F, Mp) == true)
				{
					 
					Mp.mS[0].score += F.store;
				}
				if (threePlayer2ndPlayerEatBaraFood(F, Mp) == true)
				{
					 
					Mp.mS[1].score += F.store;
				}
				if (threePlayer3rdPlayerEatBaraFood(F, Mp) == true)
				{
					 
					Mp.mS[2].score += F.store;
				}
				if (count == 150)
				{
					 
					threePlayerEraseBaraFood(F);
					count = 0;
				}
				// ^^^^^^^^^^^^^^^^
				count++;
			}
			if (winCount == 2)
				break;
		}
		if (winCount == 2)
			break;
	}
	if (Mp.mS[0].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 1 has Won the game--------------------\n\n";
		cout << "-------------------------------------------\n\n";
	}
	else if (Mp.mS[1].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 2 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	else if (Mp.mS[2].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 3 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	_getch();
	return 0;
}

int fourTimed()
{
	system("cls");
	srand(time(0));

	food f;
	baraFood F;
	int count = 0, winCount = 0, draw = 0;
	fourPlayerSnake Mp;
	char chr;
	 
	fourPlayerGenBaraFood(F, Mp);
	fourPlayerInit(Mp, f, F);
	consoleScrnInfinity();
	fourPlayerDisplayFood(f);
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < 10; k++)
			{
				gotoRowCol(20, 85);
				cout << "Player 1 score: " << Mp.mS[0].score;
				gotoRowCol(21, 85);
				cout << "Player 2 score: " << Mp.mS[1].score;
				gotoRowCol(22, 85);
				cout << "Player 3 score: " << Mp.mS[2].score;
				gotoRowCol(23, 85);
				cout << "Player 4 score: " << Mp.mS[3].score;
				gotoRowCol(24, 85);
				cout << "Time: " << i;
				if (_kbhit())
				{

					chr = _getch();
					if (chr == -32)
					{
						chr = _getch();
						fourPlayerChangeDirection(Mp, chr);
					}
					fourPlayerChangeDirection(Mp, chr);
				}
				fourPlayerDisplaySnake(Mp);
				Sleep(100);
				fourPlayerEraseSnake(Mp);
				fourPlayerMoveSnake(Mp);
				// Food Conditions
				// vvvvvvvvvvvvvvv
				if (fourPlayer1stPlayerEatFood(f, Mp) == true)
				{
					 
					fourPlayerGenFood(f, Mp);
					fourPlayerDisplayFood(f);
					Mp.mS[0].score++;
				}
				if (fourPlayer2ndPlayerEatFood(f, Mp) == true)
				{
					 
					fourPlayerGenFood(f, Mp);
					fourPlayerDisplayFood(f);
					Mp.mS[1].score++;
				}
				if (fourPlayer3rdPlayerEatFood(f, Mp) == true)
				{
					 
					fourPlayerGenFood(f, Mp);
					fourPlayerDisplayFood(f);
					Mp.mS[2].score++;
				}
				if (fourPlayer4thPlayerEatFood(f, Mp) == true)
				{
					 
					fourPlayerGenFood(f, Mp);
					fourPlayerDisplayFood(f);
					Mp.mS[3].score++;
				}
				if (count == 50)
				{
					fourPlayerGenBaraFood(F, Mp);
					fourPlayerDisplayBaraFood(F);
				}
				if (fourPlayer1stPlayerEatBaraFood(F, Mp) == true)
				{
					 
					Mp.mS[0].score += F.store;
					F.isAvailable = false;
				}
				if (fourPlayer2ndPlayerEatBaraFood(F, Mp) == true)
				{
					 
					Mp.mS[1].score += F.store;
					F.isAvailable = false;
				}
				if (fourPlayer3rdPlayerEatBaraFood(F, Mp) == true)
				{
					 
					Mp.mS[2].score += F.store;
					F.isAvailable = false;
				}
				if (fourPlayer4thPlayerEatBaraFood(F, Mp) == true)
				{
					 
					Mp.mS[3].score += F.store;
					F.isAvailable = false;
				}


				if (count == 150)
				{
					fourPlayerEraseBaraFood(F);
					count = 0;
				}
				// ^^^^^^^^^^^^^^
				// Decision Making
				// vvvvvvvvvvvvvvv
				if (fourPlayerDecision(Mp) == true)
				{
					winCount++;
					if (winCount == 3)
						break;
				}
				if (fourPlayerDrawingCondition(Mp) == true)
					draw++;
				if (draw == 2)
					break;
				// ^^^^^^^^^^^^^^^^
				count++;
			}
			if (winCount == 3)
				break;
		}
		if (winCount == 3)
			break;
	}
	if (winCount != 3)
	{
		if (Mp.mS[0].score > Mp.mS[1].score && Mp.mS[0].score > Mp.mS[2].score && Mp.mS[0].score > Mp.mS[3].score)
		{
			system("cls");
			gotoRowCol(20, 20);
			cout << "--------------------------------------\n\n";
			cout << "--------------Player 1 has Won the game--------------------\n\n";
			cout << "-------------------------------------------\n\n";
		}
		else if (Mp.mS[1].score > Mp.mS[0].score && Mp.mS[1].score > Mp.mS[2].score && Mp.mS[1].score > Mp.mS[3].score)
		{
			system("cls");
			gotoRowCol(20, 20);
			cout << "--------------------------------------\n\n";
			cout << "--------------Player 2 has Won the game--------------------\n\n";
			cout << "-------------------------------------------\n\n";
		}
		else if (Mp.mS[2].score > Mp.mS[1].score && Mp.mS[2].score > Mp.mS[0].score && Mp.mS[2].score > Mp.mS[3].score)
		{
			system("cls");
			gotoRowCol(20, 20);
			cout << "--------------------------------------\n\n";
			cout << "--------------Player 3 has Won the game--------------------\n\n";
			cout << "-------------------------------------------\n\n";
		}
		else if (Mp.mS[3].score > Mp.mS[1].score && Mp.mS[3].score > Mp.mS[2].score && Mp.mS[3].score > Mp.mS[0].score)
		{
			system("cls");
			gotoRowCol(20, 20);
			cout << "--------------------------------------\n\n";
			cout << "--------------Player 4 has Won the game--------------------\n\n";
			cout << "-------------------------------------------\n\n";
		}
	}
	if (Mp.mS[0].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 1 has Won the game--------------------\n\n";
		cout << "-------------------------------------------\n\n";
	}
	else if (Mp.mS[1].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 2 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	else if (Mp.mS[2].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 3 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	else if (Mp.mS[3].size != 0)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Player 4 has Won the game--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	else if (draw == 2)
	{
		system("cls");
		gotoRowCol(20, 20);
		cout << "--------------------------------------\n\n";
		cout << "--------------Draw--------------------\n\n";
		cout << "--------------------------------------\n\n";
	}
	_getch();
	return 0;
}

int main()
{
	gotoRowCol(5, 10);
	cout << "I dont know how to continue the music for whole game how ever you will see hear some sounds";
	 
	do {
		system("cls");
		mainMenu();
		if (checkOnePlayer() == true)
		{
			 
			selectMode();
			if (checkOnePlayer() == true)
			{
				 
				oneTimed();
			}
			else if (checkTwoPlayer() == true)
			{
				 
				one();
			}
		}
		else if (checkTwoPlayer() == true)
		{
			selectMode();
			 
			if (checkOnePlayer() == true)
			{
				 
				twoTime();
			}
			else if (checkTwoPlayer() == true)
			{
				 
				two();
			}
		}
		else if (checkThreePlayer() == true)
		{
			selectMode();
			 
			if (checkOnePlayer() == true)
			{
				 
				threeTime();
			}
			else if (checkTwoPlayer() == true)
			{
				 
				three();
			}
		}
		else if (checkFourPlayer() == true)
		{
			selectMode();
			 
			if (checkOnePlayer() == true)
			{
				 
				fourTimed();
			}
			else if (checkTwoPlayer() == true)
			{
				 
				four();
			}
		}
		 
	} while (true);
	return 0;
}
