#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <random>

// 방향키
#define UP				72
#define DOWN			80
#define LEFT			75
#define RIGHT			77

#define MAX_HP			3
#define START_POS_X		5
#define START_POS_Y		22

#define HUDDLE_START_X	104
#define ROCK_START_Y	29
#define TREE_START_Y	25
#define BIRD_Y0			6
#define BIRD_Y1			11
#define BIRD_Y2			16
#define CLOUD_START_Y	9
#define BULLET_START_X	START_POS_X + 18

#define ITEM_START_X	106
#define ITEM_UPDATE		15
#define SPEED_UPDATE	35
#define STAR_MAINTAIN	10
#define MONSTER_UPDATE	30

// modify
#define SKY_START_X		106
#define MONSTER_START_X	95
#define MONSTER_START_Y 3

using namespace std;
typedef unsigned int uint;

/*----------자료형 영역----------*/
enum {
	BLACK, DARK_BLUE, DARK_GREEN, DARK_SKYBLUE, DARK_RED, DARK_VOILET, DARK_YELLOW,
	GRAY, DARK_GRAY, BLUE, GREEN, SKYBLUE, RED, VIOLET, YELLOW, WHITE,
};

typedef struct _position {
	int x;
	int y;
}P;

class Huddle {
private:
	uint type; // rock, tree, bird
	P pos;
public:
	Huddle() : type(0) { pos.x = HUDDLE_START_X; pos.y = 0; }
	Huddle(uint t) : type(t) { pos.x = HUDDLE_START_X; pos.y = 0; }
	uint get_type() { return type; }
	int get_Xpos() { return pos.x; }
	int get_Ypos() { return pos.y; }
	void set_type(uint t) { type = t; }
	void set_Xpos(int val) { pos.x = val; }
	void set_Ypos(int val) { pos.y = val; }
	void reset() {
		// type = t;
		pos.x = HUDDLE_START_X;
		pos.y = 0;
	}
};

class Player {
private:
	uint score;
	uint hp;
	P pos;
public:
	Player() : score(0), hp(3) { pos.x = START_POS_X; pos.y = START_POS_Y; }
	uint get_score() { return score; }
	uint get_hp() { return hp; }
	int get_Xpos() { return pos.x; }
	int get_Ypos() { return pos.y; }
	void set_score(uint s) { score = s; }
	void set_hp(uint val) { hp = val; }
	void set_Xpos(int val) { pos.x = val; }
	void set_Ypos(int val) { pos.y = val; }
	friend void Character();
	friend class Bullet;
	void reset() {
		// score = 0;
		hp = MAX_HP;
		pos.x = START_POS_X;
		pos.y = START_POS_Y;
	}
};

class Item {
private:
	uint type;	// heart, star, blank
	P pos;
public:
	Item() : type(0) { pos.x = ITEM_START_X; pos.y = 0; }
	//Item(uint t) : type(t) { pos.x = ITEM_START_X; pos.y = 0; }
	uint get_type() { return type; }
	int get_Xpos() { return pos.x; }
	int get_Ypos() { return pos.y; }
	void set_type(uint t) { type = t; }
	void set_Xpos(int val) { pos.x = val; }
	void set_Ypos(int val) { pos.y = val; }
	void reset() {
		type = 2;
		pos.x = ITEM_START_X;
		pos.y = 13;
	}
};

// modify
class Sky {
private:
	uint type;	// cloud, bird
	P pos;
public:
	Sky() : type(0) { pos.x = SKY_START_X; pos.y = 0; }
	int get_Xpos() { return pos.x; }
	int get_Ypos() { return pos.y; }
	void set_Xpos(int val) { pos.x = val; }
	void set_Ypos(int val) { pos.y = val; }
	void reset() {
		// type = t;
		pos.x = SKY_START_X;
		pos.y = 0;
	}
};

class Monster {
private:
	bool isDead;
	P pos;
public:
	Monster() : isDead(FALSE) { pos.x = MONSTER_START_X; pos.y = MONSTER_START_Y; }
	bool get_dead() { return isDead; }
	int get_Xpos() { return pos.x; }
	int get_Ypos() { return pos.y; }
	void set_dead(bool val) { isDead = val; }
	void set_Xpos(int val) { pos.x = val; }
	void set_Ypos(int val) { pos.y = val; }
	void reset() {
		isDead = FALSE;
		pos.x = MONSTER_START_X;
		pos.y = MONSTER_START_Y;
	}
};

class Bullet {
private:
	P pos;
public:
	Bullet() { pos.x = BULLET_START_X; pos.y = 0; }
	int get_Xpos() { return pos.x; }
	int get_Ypos() { return pos.y; }
	void set_Xpos(int val) { pos.x = val; }
	void set_Ypos(int val) { pos.y = val; }
	void reset() {
		pos.x = BULLET_START_X;
		pos.y = 0;
	}
};

class map {
private:
	uint time;
};

/*---------전역 변수 초기화---------*/
int playerNum;
Player playerInfo[3];
Huddle huddleInfo[3];
Item itemInfo[3];
Sky skyInfo[3];
Monster monsterInfo;
Bullet bulletInfo;

/*----------함수 영역 시작----------*/
void init_class()
{
	for (int i = 0; i < 3; i++) {
		playerInfo[i].reset();
		huddleInfo[i].reset();
		itemInfo[i].reset();
		skyInfo[i].reset();
	}
	monsterInfo.reset();
	bulletInfo.reset();
}

void get_scores_file()
{
	uint num[3] = { 0 };
	ifstream in("score.txt");
	if (!in.is_open()) return;
	in >> num[0] >> num[1] >> num[2];
	for (int i = 0; i < 3; i++)
		playerInfo[i].set_score(num[i]);
	in.close();
	return;
}

void set_scores_file()
{
	ofstream out("score.txt");
	for (int i = 0; i < 3; i++)
		out << playerInfo[i].get_score() << " ";
	out.close();
	return;
}

template <typename T>
inline T randomNum(T min, T max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<T> dist(min, max);

	return dist(gen);
}

void cursor_pos(int x, int y)
{
	COORD p;
	p.X = x;
	p.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

int input_key_dir()
{
	char c;
	if (_kbhit())
	{
		c = _getch();
		if (c == -32)
			return _getch();
	}
	return 0;
}

int MainMenu()
{
	int x = 44, y = 10;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);

	system("cls");
	cursor_pos(x, y);
	printf("=================================");
	cursor_pos(x + 4, y + 3);
	printf("▶  G A M E S T A R T");
	cursor_pos(x + 8, y + 6);
	printf("S C O R E");
	cursor_pos(x + 8, y + 9);
	printf("E X I T");
	cursor_pos(x, y + 12);
	printf("=================================");
	printf("\n\n\n\n\n\n\n\n\n\n\n");

	int menu = 0;
	bool isSelect = FALSE;

	while (1) {
		int key = input_key_dir();
		if (key == DOWN) menu++;
		else if (key == UP) menu += 2;
		else if (key == RIGHT) isSelect = TRUE;
		else if (key == 0) continue;

		menu %= 3;

		if (menu == 0) {
			system("cls");
			cursor_pos(x, y);
			printf("=================================");
			cursor_pos(x + 4, y + 3);
			printf("▶  G A M E S T A R T");
			cursor_pos(x + 8, y + 6);
			printf("S C O R E");
			cursor_pos(x + 8, y + 9);
			printf("E X I T");
			cursor_pos(x, y + 12);
			printf("=================================");
		}
		else if (menu == 1) {
			system("cls");
			cursor_pos(x, y);
			printf("=================================");
			cursor_pos(x + 8, y + 3);
			printf("G A M E S T A R T");
			cursor_pos(x + 4, y + 6);
			printf("▶  S C O R E");
			cursor_pos(x + 8, y + 9);
			printf("E X I T");
			cursor_pos(x, y + 12);
			printf("=================================");
		}
		else if (menu == 2) {
			system("cls");
			cursor_pos(x, y);
			printf("=================================");
			cursor_pos(x + 8, y + 3);
			printf("G A M E S T A R T");
			cursor_pos(x + 8, y + 6);
			printf("S C O R E");
			cursor_pos(x + 4, y + 9);
			printf("▶  E X I T");
			cursor_pos(x, y + 12);
			printf("=================================");
		}
		printf("\n\n\n\n\n\n\n\n\n\n\n");

		if (isSelect) break;
	}

	return menu;
}

int GameStart()
{
	int x = 44, y = 10;
	system("cls");
	cursor_pos(x, y);
	printf("=================================");
	cursor_pos(x + 4, y + 3);
	printf("▶  P L A Y E R  1");
	cursor_pos(x + 8, y + 6);
	printf("P L A Y E R  2");
	cursor_pos(x + 8, y + 9);
	printf("P L A Y E R  3");
	cursor_pos(x, y + 12);
	printf("=================================");
	printf("\n\n\n\n\n\n\n\n\n\n\n");

	int select = 4;

	while (1) {
		int key = input_key_dir();
		if (key == DOWN) playerNum++;
		else if (key == UP) playerNum += 2;
		else if (key == RIGHT) select = playerNum;
		else if (key == LEFT) select = 3;
		else if (key == 0) continue;

		playerNum %= 3;

		if (playerNum == 0) {
			system("cls");
			cursor_pos(x, y);
			printf("=================================");
			cursor_pos(x + 4, y + 3);
			printf("▶  P L A Y E R  1");
			cursor_pos(x + 8, y + 6);
			printf("P L A Y E R  2");
			cursor_pos(x + 8, y + 9);
			printf("P L A Y E R  3");
			cursor_pos(x, y + 12);
			printf("=================================");
		}
		else if (playerNum == 1) {
			system("cls");
			cursor_pos(x, y);
			printf("=================================");
			cursor_pos(x + 8, y + 3);
			printf("P L A Y E R  1");
			cursor_pos(x + 4, y + 6);
			printf("▶  P L A Y E R  2");
			cursor_pos(x + 8, y + 9);
			printf("P L A Y E R  3");
			cursor_pos(x, y + 12);
			printf("=================================");
		}
		else if (playerNum == 2) {
			system("cls");
			cursor_pos(x, y);
			printf("=================================");
			cursor_pos(x + 8, y + 3);
			printf("P L A Y E R  1");
			cursor_pos(x + 8, y + 6);
			printf("P L A Y E R  2");
			cursor_pos(x + 4, y + 9);
			printf("▶  P L A Y E R  3");
			cursor_pos(x, y + 12);
			printf("=================================");
		}
		printf("\n\n\n\n\n\n\n\n\n\n\n");

		if (select < 4) break;
	}
	return select;
}

void Count3sec()
{
	int x = 54, y = 11;
	system("cls");
	cursor_pos(x, y++);
	printf("■■■■■■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("■■■■■■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("■■■■■■");
	printf("\n\n\n\n\n\n\n\n\n\n\n");
	Sleep(1000);

	system("cls");
	y = 11;
	cursor_pos(x, y++);
	printf("■■■■■■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("          ■");
	cursor_pos(x, y++);
	printf("■■■■■■");
	cursor_pos(x, y++);
	printf("■");
	cursor_pos(x, y++);
	printf("■");
	cursor_pos(x, y++);
	printf("■");
	cursor_pos(x, y++);
	printf("■");
	cursor_pos(x, y++);
	printf("■■■■■■");
	printf("\n\n\n\n\n\n\n\n\n\n\n");
	Sleep(1000);

	system("cls");
	x = 55, y = 11;
	cursor_pos(x, y++);
	printf("  ■■");
	cursor_pos(x, y++);
	printf("    ■");
	cursor_pos(x, y++);
	printf("    ■");
	cursor_pos(x, y++);
	printf("    ■");
	cursor_pos(x, y++);
	printf("    ■");
	cursor_pos(x, y++);
	printf("    ■");
	cursor_pos(x, y++);
	printf("    ■");
	cursor_pos(x, y++);
	printf("    ■");
	cursor_pos(x, y++);
	printf("    ■");
	cursor_pos(x, y++);
	printf("    ■");
	cursor_pos(x, y++);
	printf("■■■■■");
	printf("\n\n\n\n\n\n\n\n\n\n\n");
	Sleep(1000);
}

void Score()
{
	string name[] = { "Player1", "Player2", "Player3" };
	int ranking[] = { 0, 1, 2 };
	int x = 44, y = 10;

	//sort(); ranking을 sort

	system("cls");
	cursor_pos(x, y);
	printf("=================================");
	cursor_pos(x + 5, y + 3);
	cout << "1st    " << name[ranking[0]] << "   " << playerInfo[ranking[0]].get_score();
	cursor_pos(x + 5, y + 6);
	cout << "2nd    " << name[ranking[1]] << "   " << playerInfo[ranking[1]].get_score();
	cursor_pos(x + 5, y + 9);
	cout << "3rd    " << name[ranking[2]] << "   " << playerInfo[ranking[2]].get_score();
	cursor_pos(x, y + 12);
	printf("=================================");
	printf("\n\n\n\n\n\n\n\n\n\n\n");

	while (1) {
		int key = input_key_dir();
		if (key == 0) continue;
		else break;
	}
}

void Character(Player myplay, int isStar)
{
	int x = myplay.get_Xpos(), y = myplay.get_Ypos();
	//system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	cursor_pos(x + 6, y++);
	printf("■■■");
	cursor_pos(x + 4, y++);
	printf("■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	printf("■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x + 2, y++);
	printf("■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	printf("■■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x + 2, y++);
	printf("■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	printf("■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■■");
	cursor_pos(x, y++);
	printf("■■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	printf("■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_SKYBLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SKYBLUE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x, y++);
	printf("■■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	printf("■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■■");
	cursor_pos(x, y++);
	printf("■■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	printf("■■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x, y++);
	printf("■■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	printf("■■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x + 2, y++);
	printf("■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x + 2, y++);
	printf("■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■  ■");
	if (!isStar)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x + 3, y++);
	printf("■■    ■■");
	//system("pause");
}

void Dot(int posX) {
	int x = posX, y = bulletInfo.get_Ypos();
	//system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_RED);
	cursor_pos(x, y);
	printf("■▶");
	//system("pause");
}

void Rock(int posX)
{
	int x = posX, y = ROCK_START_Y;
	//system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	cursor_pos(x + 2, y++);
	printf("■■");
	cursor_pos(x, y++);
	printf("■■■■");
	cursor_pos(x, y++);
	printf("■■■■");
	cursor_pos(x, y++);
	printf("■■■■");
	//system("pause");
}

void Tree(int posX)
{
	int x = posX, y = TREE_START_Y;
	//system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GREEN);
	cursor_pos(x + 3, y++);
	printf("■■");
	cursor_pos(x + 2, y++);
	printf("■■■");
	cursor_pos(x + 1, y++);
	printf("■■■■");
	cursor_pos(x, y++);
	printf("■■■■■");
	cursor_pos(x, y++);
	printf("■■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_YELLOW);
	cursor_pos(x + 3, y++);
	printf("■■");
	cursor_pos(x + 3, y++);
	printf("■■");
	cursor_pos(x + 3, y++);
	printf("■■");
	//system("pause");
}

void Bird(int posX, int posY)
{
	int x = posX, y = posY;
	//system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GRAY);
	cursor_pos(x + 2, y++);
	printf("■        ■");
	cursor_pos(x, y++);
	printf("■  ■    ■  ■");
	cursor_pos(x + 6, y++);
	printf("■■");
	cursor_pos(x + 6, y++);
	printf("■■");
	//system("pause");
}

void Cloud(int posX, int posY)
{
	int x = posX, y = posY;
	//system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SKYBLUE);
	cursor_pos(x + 2, y++);
	printf("■■  ■■");
	cursor_pos(x, y++);
	printf("■■■■■■■");
	cursor_pos(x, y++);
	printf("■■■■■■■■");
	cursor_pos(x, y++);
	printf("■■■■■■■");
	cursor_pos(x + 2, y++);
	printf("■■■■");
	//system("pause");
}

void Ghost(int posX, int posY)
{
	int x = posX, y = posY;
	//system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VIOLET);
	cursor_pos(x + 8, y++);
	printf("■■■■");
	cursor_pos(x + 4, y++);
	printf("■■■■■■■■");
	cursor_pos(x + 2, y++);
	printf("■■■■■■■■■■");
	cursor_pos(x, y++);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VIOLET);
	printf("■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VIOLET);
	printf("■■■");
	cursor_pos(x, y++);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VIOLET);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VIOLET);
	printf("■■");
	cursor_pos(x, y++);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VIOLET);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VIOLET);
	printf("■■");
	cursor_pos(x, y++);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VIOLET);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VIOLET);
	printf("■■");
	cursor_pos(x, y++);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VIOLET);
	printf("■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), VIOLET);
	printf("■■■");
	cursor_pos(x, y++);
	printf("■■■■■■■■■■■■");
	cursor_pos(x, y++);
	printf("■■■■■■■■■■■■");
	cursor_pos(x, y++);
	printf("■■■  ■■■■  ■■■");
	cursor_pos(x, y++);
	printf("■■      ■■      ■■");
	//system("pause");
}

void Heart(int posX, int posY)
{
	int x = posX, y = posY;
	//system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
	cursor_pos(x + 2, y++);
	printf("■■  ■■");
	cursor_pos(x, y++);
	printf("■■■■■■■");
	cursor_pos(x + 2, y++);
	printf("■■■■■");
	cursor_pos(x + 4, y++);
	printf("■■■");
	cursor_pos(x + 6, y++);
	printf("■");
	//system("pause");
}

void Star(int posX, int posY)
{
	int x = posX, y = posY;
	//system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	cursor_pos(x + 4, y++);
	printf("■");
	cursor_pos(x + 4, y++);
	printf("■");
	cursor_pos(x, y++);
	printf("■■■■■");
	cursor_pos(x + 3, y++);
	printf("■■");
	cursor_pos(x + 1, y++);
	printf("■    ■");
	//system("pause");
}

void GameOver()
{
	int x = 37, y = 10;
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);

	//GAME
	cursor_pos(x, y++);
	printf("■■■■      ■■      ■      ■    ■■■■");
	cursor_pos(x, y++);
	printf("■          ■    ■    ■■  ■■    ■");
	cursor_pos(x, y++);
	printf("■  ■■    ■■■■    ■  ■  ■    ■■■■");
	cursor_pos(x, y++);
	printf("■    ■    ■    ■    ■      ■    ■");
	cursor_pos(x, y++);
	printf("■■■■    ■    ■    ■      ■    ■■■■");

	//OVER
	y += 2;
	cursor_pos(x, y++);
	printf("■■■■    ■      ■    ■■■■    ■■■");
	cursor_pos(x, y++);
	printf("■    ■     ■    ■     ■          ■    ■");
	cursor_pos(x, y++);
	printf("■    ■      ■  ■      ■■■■    ■■■");
	cursor_pos(x, y++);
	printf("■    ■       ■■       ■          ■   ■");
	cursor_pos(x, y++);
	printf("■■■■        ■        ■■■■    ■    ■");
	//printf("\n\n\n\n\n\n\n\n\n\n\n");
	cursor_pos(x + 5, y + 5);
	system("pause");
}

void ShowScore(uint score)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	cursor_pos(2, 1);
	printf("SCORE  %u", score);
	cursor_pos(102, 1);
	printf("HEART  ");
	for (int i = 0; i < playerInfo[playerNum].get_hp(); i++)
		printf("♥ ");
}

int check_attack()
{
	int mx = monsterInfo.get_Xpos(), my = monsterInfo.get_Ypos();
	int bx = bulletInfo.get_Xpos(), by = bulletInfo.get_Ypos();
	for (int i = bx; i < bx + 2; i++) {
		for (int nx = mx; nx < mx + 12; nx++) {
			for (int ny = my; ny < my + 12; ny++) {
				if (i == nx && by == ny) return 1;
			}
		}
	}
	return 0;
}

int check_hunt(bool status)
{
	if (status) return 0;

	int mx = monsterInfo.get_Xpos(), my = monsterInfo.get_Ypos();
	int px = playerInfo[playerNum].get_Xpos(), py = playerInfo[playerNum].get_Ypos();
	for (int i = px; i < px + 8; i++) {
		for (int j = py; j < py + 11; j++) {
			for (int nx = mx; nx < mx + 12; nx++) {
				for (int ny = my; ny < my + 12; ny++) {
					if (i == nx && j == ny) return 1;
				}
			}
		}
	}
	return 0;
}

int check_fail(uint player, uint huddle, bool status)
{
	if (status) return 0;

	if (huddle == 0) {
		// rock
		int x = huddleInfo[huddle].get_Xpos(), y = ROCK_START_Y;
		for (int i = playerInfo[player].get_Xpos(); i < playerInfo[player].get_Xpos() + 8; i++) {
			for (int j = playerInfo[player].get_Ypos(); j < playerInfo[player].get_Ypos() + 11; j++) {
				for (int nx = x; nx < x + 4; nx++) {
					for (int ny = y; ny < y + 4; ny++) {
						if (i == nx && j == ny) return 1;
					}
				}
			}
		}
	}
	else if (huddle == 1) {
		// tree
		int x = huddleInfo[huddle].get_Xpos(), y = TREE_START_Y;
		for (int i = playerInfo[player].get_Xpos(); i < playerInfo[player].get_Xpos() + 8; i++) {
			for (int j = playerInfo[player].get_Ypos(); j < playerInfo[player].get_Ypos() + 11; j++) {
				for (int nx = x; nx < x + 5; nx++) {
					for (int ny = y; ny < y + 8; ny++) {
						if (i == nx && j == ny) return 1;
					}
				}
			}
		}

	}
	else if (huddle == 2) {
		// bird
		int x = huddleInfo[huddle].get_Xpos(), y = huddleInfo[huddle].get_Ypos();
		for (int i = playerInfo[player].get_Xpos(); i < playerInfo[player].get_Xpos() + 8; i++) {
			for (int j = playerInfo[player].get_Ypos(); j < playerInfo[player].get_Ypos() + 11; j++) {
				for (int nx = x; nx < x + 12; nx++) {
					for (int ny = y; ny < y + 4; ny++) {
						if (i == nx && j == ny) return 1;
					}
				}
			}
		}
	}
	return 0;
}

int check_success(uint player, uint item)
{
	if (item == 0) {
		// heart
		int x = itemInfo[item].get_Xpos(), y = itemInfo[item].get_Ypos();
		for (int i = playerInfo[player].get_Xpos(); i < playerInfo[player].get_Xpos() + 8; i++) {
			for (int j = playerInfo[player].get_Ypos(); j < playerInfo[player].get_Ypos() + 11; j++) {
				for (int nx = x; nx < x + 7; nx++) {
					for (int ny = y; ny < y + 5; ny++) {
						if (i == nx && j == ny) return 1;
					}
				}
			}
		}
	}
	else if (item == 1) {
		// star
		int x = itemInfo[item].get_Xpos(), y = itemInfo[item].get_Ypos();
		for (int i = playerInfo[player].get_Xpos(); i < playerInfo[player].get_Xpos() + 8; i++) {
			for (int j = playerInfo[player].get_Ypos(); j < playerInfo[player].get_Ypos() + 11; j++) {
				for (int nx = x; nx < x + 5; nx++) {
					for (int ny = y; ny < y + 5; ny++) {
						if (i == nx && j == ny) return 1;
					}
				}
			}
		}
	}
	return 0;
}

void Playing()
{
	// 시간 관련 변수
	time_t start = time(NULL), middle, finish;
	time_t start_star = time(NULL);
	time(&start);
	time(&start_star);

	// 점프 관련 변수
	char jump_type = 0; //no jump state
	int jump_size = 8, jump_cnt = 0;
	bool down = false;

	// 장애물 관련 변수
	int huddle_speed = 2;
	int huddle_type = 0;
	bool huddle_check = TRUE;

	// 아이템 관련 변수
	bool isStar = FALSE;
	int item_type = 0;
	int item_speed = 2;
	bool item_check = TRUE;
	int item_once = 0;

	/*
	// 하늘 관련 변수
	int sky_type = 0;
	int sky_speed = 2;
	bool sky_check = TRUE;
	*/

	// 몬스터 관련 변수
	int monster_type = 0;
	int monster_speed = 2;
	int monster_direction = 1;
	int monster_once = 0;

	// 총알 관련 변수
	bool bullet_check = FALSE;

	while (1) {

		// 10초마다 장애물 속도 증가
		middle = time(NULL);
		time(&middle);
		if (((int)difftime(middle, start)) > (huddle_speed - 1) * SPEED_UPDATE) huddle_speed++;

		// 2단 점프
		int key_dir = input_key_dir();

		if (key_dir == UP) {
			if (jump_type == 0)
				jump_type = 1;
			else if (jump_type == 1) {
				jump_type = 2;
				jump_cnt = 0;
				down = false;
			}
		}

		if (jump_type) {
			jump_cnt++;

			if (down)
				playerInfo[playerNum].set_Ypos(playerInfo[playerNum].get_Ypos() + 1);
			else
				playerInfo[playerNum].set_Ypos(playerInfo[playerNum].get_Ypos() - 1);

			if (jump_cnt == jump_size) {
				if (!down) {
					//Character(playerInfo[playerNum]);
					down = true;
					jump_cnt = 0;
				}
			}

			if (playerInfo[playerNum].get_Ypos() == START_POS_Y) {
				jump_type = 0;
				jump_cnt = 0;
				down = false;
			}
		}

		// 총알 위치 선정
		if (key_dir == RIGHT && !bullet_check) {
			bulletInfo.set_Xpos(BULLET_START_X);
			bulletInfo.set_Ypos(playerInfo[playerNum].get_Ypos() + 4);
			bullet_check = TRUE;
		}
		if (bullet_check) {
			uint bullet_curx = bulletInfo.get_Xpos();
			bulletInfo.set_Xpos(bullet_curx + 2);
			if ((int)difftime(middle, start) > MONSTER_UPDATE && (!monsterInfo.get_dead() && check_attack())) {
				bullet_check = FALSE;
				monsterInfo.set_dead(TRUE);
			}
		}
		if (bulletInfo.get_Xpos() > 116) {
			bullet_check = FALSE;
		}

		/*
		// 구름 위치 선정
		uint cloud_curx = skyInfo[sky_type].get_Xpos();
		skyInfo[sky_type].set_Xpos(cloud_curx - sky_speed);

		// 구름 타입 교체
		if (skyInfo[sky_type].get_Xpos() < 0) {

		}
		*/

		// 장애물 위치 선정
		uint huddle_curx = huddleInfo[huddle_type].get_Xpos();
		huddleInfo[huddle_type].set_Xpos(huddle_curx - huddle_speed);

		// 장애물 타입 교체
		if (huddleInfo[huddle_type].get_Xpos() < 0) {
			huddle_type = randomNum(0, 2);
			huddle_check = TRUE;
			huddleInfo[huddle_type].set_Xpos(HUDDLE_START_X);
			if (huddle_type == 2) {
				switch (randomNum(0, 2)) {
				case 0:
					huddleInfo[huddle_type].set_Ypos(BIRD_Y0);
					break;
				case 1:
					huddleInfo[huddle_type].set_Ypos(BIRD_Y1);
					break;
				case 2:
					huddleInfo[huddle_type].set_Ypos(BIRD_Y2);
					break;
				}
			}
		}

		// 몬스터 위치 선정
		if ((int)difftime(middle, start) > MONSTER_UPDATE) {
			int monster_curx = monsterInfo.get_Xpos();
			int monster_cury = monsterInfo.get_Ypos();
			monsterInfo.set_Xpos(monster_curx - monster_speed);

			if (monster_cury <= 3)
				monster_direction = 1;
			else if (monster_cury >= 18)
				monster_direction = -1;
			monsterInfo.set_Ypos(monster_cury + monster_direction);

			if (((int)difftime(middle, start)) % MONSTER_UPDATE == 0) monster_once++;
			if (monsterInfo.get_Xpos() < 0 && monster_once == 1) {
				monsterInfo.set_Xpos(MONSTER_START_X);
				monsterInfo.set_Ypos(randomNum(2, 18));
				monsterInfo.set_dead(FALSE);
			}
			if (((int)difftime(middle, start)) % (MONSTER_UPDATE + 1) == 0) monster_once = 0;
		}

		if ((int)difftime(middle, start) > MONSTER_UPDATE && (!monsterInfo.get_dead() && check_hunt(isStar))) {
			break;
		}

		// 아이템 위치 선정
		if ((int)difftime(middle, start) > ITEM_UPDATE) {
			int item_curx = itemInfo[item_type].get_Xpos();
			itemInfo[item_type].set_Xpos(item_curx - item_speed);

			// 아이템 타입 교체
			if (((int)difftime(middle, start)) % ITEM_UPDATE == 0) item_once++;
			if (itemInfo[item_type].get_Xpos() < 0 && item_once == 1) {
				item_type = randomNum(0, 1);
				item_check = TRUE;
				itemInfo[item_type].set_Xpos(ITEM_START_X);
				itemInfo[item_type].set_Ypos(randomNum(8, 25));
				if (item_type == 1) {
					start_star = time(NULL);
					time(&start_star);
				}
			}
			if (((int)difftime(middle, start)) % (ITEM_UPDATE + 1) == 0) item_once = 0;
		}

		// 아이템 얻었는지 판정
		if (check_success(playerNum, item_type) && item_check) {
			item_check = FALSE;
			if ((item_type == 0) && (playerInfo[playerNum].get_hp() < 3))
				playerInfo[playerNum].set_hp(playerInfo[playerNum].get_hp() + 1);
			else if (item_type == 1)
				isStar = TRUE;
			item_type = 2;
		}

		// 무적 상태 10초 유지
		if (((int)difftime(middle, start_star)) > (item_speed - 1) * STAR_MAINTAIN)
			isStar = FALSE;

		// 게임 오버 판정
		if (check_fail(playerNum, huddle_type, isStar) && huddle_check) {
			huddle_check = FALSE;
			playerInfo[playerNum].set_hp(playerInfo[playerNum].get_hp() - 1);
			if (playerInfo[playerNum].get_hp() == 0) break;
		}

		// 화면 출력
		system("cls");

		// 현재 점수, HP
		ShowScore((int)difftime(middle, start));

		// 캐릭터 그리기
		Character(playerInfo[playerNum], isStar);

		// 아이템 그리기
		if ((int)difftime(middle, start) > ITEM_UPDATE) {
			if (itemInfo[item_type].get_Xpos() > 0) {
				switch (item_type) {
				case 0:
					Heart(itemInfo[item_type].get_Xpos(), itemInfo[item_type].get_Ypos()); break;
				case 1:
					Star(itemInfo[item_type].get_Xpos(), itemInfo[item_type].get_Ypos()); break;
				}
			}
		}

		// 몬스터 그리기
		if (((int)difftime(middle, start) > MONSTER_UPDATE) && (!monsterInfo.get_dead())) {
			if (monsterInfo.get_Xpos() > 0) {
				Ghost(monsterInfo.get_Xpos(), monsterInfo.get_Ypos());
			}
		}

		// 총알 그리기
		if (bullet_check)
			Dot(bulletInfo.get_Xpos());

		// 장애물 그리기
		switch (huddle_type) {
		case 0:
			Rock(huddleInfo[huddle_type].get_Xpos()); break;
		case 1:
			Tree(huddleInfo[huddle_type].get_Xpos()); break;
		case 2:
			Bird(huddleInfo[huddle_type].get_Xpos(), huddleInfo[huddle_type].get_Ypos()); break;
		}
	}

	// 점수 설정
	finish = time(NULL);
	time(&finish);
	int flowTime = (int)difftime(finish, start);
	int prev = playerInfo[playerNum].get_score();

	playerInfo[playerNum].set_score(flowTime > prev ? flowTime : prev);

	return;
}

int main()
{
	system("mode con:cols=120 lines=35"); // 가로, 세로
	get_scores_file();

	while (1)
	{
		playerNum = 0;
		init_class();
		int next_GameStart = 4, next_Score = 4;

		switch (MainMenu()) {
		case 0: {
			next_GameStart = GameStart();		// player 선택
			if (next_GameStart == 3) break;		// 첫 화면으로 이동
			else if (next_GameStart < 4) {
				// Count3sec();					// 게임 시작 전 3초 count
				Playing();
				GameOver();
				break;
			}
		}
		case 1: Score(); break;
		case 2: system("cls"); set_scores_file(); return 0;
		}
	}

	return 0;
}