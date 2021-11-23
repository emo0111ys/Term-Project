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

#define START_POS_X		4
#define START_POS_Y		22

#define HUDDLE_START_X	100
#define ROCK_START_Y	30
#define TREE_START_Y	25
#define BIRD_Y0			7
#define BIRD_Y1			10
#define BIRD_Y2			13

#define MAX_HP			3

#define ITEM_START_X	100

#define ITEM_UPDATE		15
#define SPEED_UPDATE	20
#define STAR_MAINTAIN	10

using namespace std;
typedef unsigned int uint;

/*----------자료형 영역----------*/
enum {
	BLACK, DARK_BLUE, DARK_GREEN, DARK_SKYBLUE, DARK_RED, DARK_VOILET, DAKR_YELLOW,
	GRAY, DARK_GRAY, BLUE, GREEN, SKYBLUE, RED, VIOLET, YELLOW, WHITE,
};

typedef struct _position {
	int x;
	int y;
}P;

class Huddle {
private:
	uint type; // rock, tree, bird, cloud
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
	void reset(int t) {
		type = t;
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
	Item(uint t) : type(t) { pos.x = ITEM_START_X; pos.y = 0; }
	uint get_type() { return type; }
	int get_Xpos() { return pos.x; }
	int get_Ypos() { return pos.y; }
	void set_type(uint t) { type = t; }
	void set_Xpos(int val) { pos.x = val; }
	void set_Ypos(int val) { pos.y = val; }
	void reset(int t) {
		type = t;
		pos.x = ITEM_START_X;
		pos.y = 13;
	}
};

/*
class Cloud {
private:
	P pos;
public:
	Cloud() : { pos.x = CLOUD_START_X; pos.y = 0; }
	int get_Xpos() { return pos.x; }
	int get_Ypos() { return pos.y; }
	void set_Xpos(int val) { pos.x = val; }
	void set_Ypos(int val) { pos.y = val; }
	void reset() {
		pos.x = CLOUD_START_X;
		pos.y = 0;
	}
};
*/

class bullet {
private:
	P pos;
};

class map {
private:
	uint time;
};

int playerNum;
Player playerInfo[3];
Huddle huddleInfo[3];
Item itemInfo[3];
// Cloud cloudInfo[1];

/*----------함수 영역 시작----------*/
void init_class()
{
	for (int i = 0; i < 3; i++) {
		playerInfo[i].reset();
		huddleInfo[i].reset(i);
		itemInfo[i].reset(i);
	}
	// cloudInfo[0].reset();
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

void cursor_pos(int x, int y)
{
	COORD p;
	p.X = x;
	p.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

int input_key()
{
	if (_kbhit())
		return _getch();
	return 0;
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

void GameStart()
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

	bool isSelect = FALSE;

	while (1) {
		int key = input_key_dir();
		if (key == DOWN) playerNum++;
		else if (key == UP) playerNum += 2;
		else if (key == RIGHT) isSelect = TRUE;
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

		if (isSelect) break;
	}
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
	int x = 45, y = 10;

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

void Character(Player myplay)
{
	int x = myplay.get_Xpos(), y = myplay.get_Ypos();
	//system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	cursor_pos(x + 6, y++);
	printf("■■■");
	cursor_pos(x + 4, y++);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x + 2, y++);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x + 2, y++);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■■");
	cursor_pos(x, y++);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_SKYBLUE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x, y++);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■■");
	cursor_pos(x, y++);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x, y++);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■■■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x + 2, y++);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x + 2, y++);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■  ■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_BLUE);
	printf("■");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DARK_GRAY);
	printf("■");
	cursor_pos(x + 3, y++);
	printf("■■    ■■");
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
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DAKR_YELLOW);
	cursor_pos(x + 2, y++);
	printf("■■");
	cursor_pos(x + 2, y++);
	printf("■■");
	cursor_pos(x + 2, y++);
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
	printf("\n\n\n\n\n\n\n\n\n\n\n");
	system("pause");
}

void ShowScore(uint score)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	cursor_pos(0, 0);
	printf("SCORE = %u", score);
}

int check_fail(uint player, uint huddle, bool status)
{
	if (status == TRUE) return 0;

	if (huddle == 0) {
		// rock
		int x = huddleInfo[huddle].get_Xpos(), y = ROCK_START_Y;
		for (int i = playerInfo[player].get_Xpos(); i < playerInfo[player].get_Xpos() + 9; i++) {
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
		for (int i = playerInfo[player].get_Xpos(); i < playerInfo[player].get_Xpos() + 9; i++) {
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
		for (int i = playerInfo[player].get_Xpos(); i < playerInfo[player].get_Xpos() + 9; i++) {
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
		for (int i = playerInfo[player].get_Xpos(); i < playerInfo[player].get_Xpos() + 9; i++) {
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
		for (int i = playerInfo[player].get_Xpos(); i < playerInfo[player].get_Xpos() + 9; i++) {
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

int Playing()
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

	// 장애물 종류 선정을 위한 난수 생성
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis1(0, 2);

	// 아이템 종류 선정을 위한 난수 생성
	uniform_int_distribution<int> dis2(0, 1);

	// 아이템 높이 선정을 위한 난수 생성
	uniform_int_distribution<int> dis3(11, 25);

	// 장애물 관련 변수
	int huddle_speed = 2;
	int huddle_type = 0;
	bool huddle_check = TRUE;
	int bird_height = 0;

	// 아이템 관련 변수
	bool isStar = FALSE;
	int item_type = 0;
	int item_speed = 2;
	bool item_check = TRUE;
	int item_once = 0;

	while (1) {
		system("cls");

		// 10초마다 장애물 속도 증가
		middle = time(NULL);
		time(&middle);
		if (((int)difftime(middle, start)) > (huddle_speed - 1) * SPEED_UPDATE) huddle_speed++;

		// 2단 점프
		int key = input_key_dir();
		if (key == UP) {
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
					Character(playerInfo[playerNum]);
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

		// 장애물 위치 선정
		uint huddle_curx = huddleInfo[huddle_type].get_Xpos();
		huddleInfo[huddle_type].set_Xpos(huddle_curx - huddle_speed);

		// 장애물 타입 교체
		if (huddleInfo[huddle_type].get_Xpos() < 0)
		{
			huddle_type = dis1(gen);
			huddle_check = TRUE;
			huddleInfo[huddle_type].set_Xpos(HUDDLE_START_X);
			if (huddle_type == 2) {
				bird_height = dis1(gen);
				switch (bird_height) {
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

		// 아이템 위치 선정
		uint item_curx = itemInfo[item_type].get_Xpos();
		itemInfo[item_type].set_Xpos(item_curx - item_speed);

		// 아이템 타입 교체
		if (((int)difftime(middle, start)) % ITEM_UPDATE == 0) item_once++;
		if (itemInfo[item_type].get_Xpos() < 0 && (item_once == 1))
		{
			item_type = dis2(gen);
			start_star = time(NULL);
			time(&start_star);
			item_check = TRUE;
			itemInfo[item_type].set_Xpos(ITEM_START_X);
			itemInfo[item_type].set_Ypos(dis3(gen));
		}
		if (((int)difftime(middle, start)) % (ITEM_UPDATE + 1) == 0) item_once = 0;

		// 게임 오버 판정
		if (check_fail(playerNum, huddle_type, isStar) && huddle_check) {
			huddle_check = FALSE;
			playerInfo[playerNum].set_hp(playerInfo[playerNum].get_hp() - 1);
			if (playerInfo[playerNum].get_hp() == 0) break;
		}

		// 아이템 얻었는지 판정
		if (check_success(playerNum, item_type) && item_check) {
			item_check = FALSE;
			if ((item_type == 0) && (playerInfo[playerNum].get_hp() < 3)) {
				playerInfo[playerNum].set_hp(playerInfo[playerNum].get_hp() + 1);
			}
			else if (item_type == 1) {
				isStar = TRUE;
			}
			item_type = 2;
		}
		if (((int)difftime(middle, start_star)) > (item_speed - 1) * STAR_MAINTAIN)
			isStar = FALSE;

		// 현재 점수 출력
		ShowScore((int)difftime(middle, start));

		// 지우기
		// 현재 체력, 현재 무적인지 아닌지 출력
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
		cursor_pos(0, 1);
		printf("HEART = %u, STAR = %d", playerInfo[playerNum].get_hp(), isStar);

		// 캐릭터 그리기
		Character(playerInfo[playerNum]);

		// 장애물 그리기
		switch (huddle_type) {
		case 0:
			Rock(huddleInfo[huddle_type].get_Xpos());
			break;
		case 1:
			Tree(huddleInfo[huddle_type].get_Xpos());
			break;
		case 2:
			Bird(huddleInfo[huddle_type].get_Xpos(), huddleInfo[huddle_type].get_Ypos());
			break;
		}

		// 아이템 그리기
		switch (item_type) {
		case 0:
			Heart(itemInfo[item_type].get_Xpos(), itemInfo[item_type].get_Ypos());
			break;
		case 1:
			Star(itemInfo[item_type].get_Xpos(), itemInfo[item_type].get_Ypos());
			break;
		}

		// 구름 그리기
		// Cloud(cloudInfo[1].get_Xpos()); break;
	}

	// 점수 설정
	finish = time(NULL);
	time(&finish);
	int flowTime = (int)difftime(finish, start);
	int prev = playerInfo[playerNum].get_score();

	playerInfo[playerNum].set_score(flowTime > prev ? flowTime : prev);

	return playerInfo[playerNum].get_score();
}

int main()
{
	system("mode con:cols=120 lines=35"); //가로, 세로
	//SetConsoleTitle("Team10  Google Dinosaurs Game Plus Version");
	get_scores_file();

	while (1)
	{
		int gameScore = 0;
		playerNum = 0;
		init_class();

		switch (MainMenu()) {
		case 0: GameStart(); Count3sec(); gameScore = Playing(); GameOver(); break;
		case 1: Score(); break;
		case 2: system("cls"); set_scores_file(); return 0;
		}
	}

	return 0;
}
