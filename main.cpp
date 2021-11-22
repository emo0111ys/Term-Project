#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>
#include <conio.h>
#include <time.h>

// 방향키
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

#define START_POS_X 0
#define START_POS_Y 0
#define MAX_HP 3

using namespace std;
typedef unsigned int uint;


/*----------자료형 영역----------*/
typedef struct _position {
    int x;
    int y;
}P;

class Huddle {
private:
    uint type; //rock, tree, bird, cloud
    P pos;
public:
    Huddle(uint t) : type(t) { pos.x = 0; pos.y = 0; }
    uint get_type() { return type; }
    int get_Xpos() { return pos.x; }
    int get_Ypos() { return pos.y; }
    void set_Xpos(int val) { pos.x = val; }
    void set_Ypos(int val) { pos.y = val; }
};

class Player {
private:
    uint score;
    uint hp;
    P pos;
public:
    Player() : score(0), hp(3) { pos.x = 0; pos.y = START_POS_Y; }
    uint get_score() { return score; }
    uint get_hp() { return hp; }
    int get_Xpos() { return pos.x; }
    int get_Ypos() { return pos.y; }
    void set_score(uint s) { score = s; }
    void set_hp(uint val) { hp = val; }
    void set_Xpos(int val) { pos.x = val; }
    void set_Ypos(int val) { pos.y = val; }

    void reset() {
        score = 0;
        hp = MAX_HP;
        pos.x = START_POS_X;
        pos.y = START_POS_Y;
    }
};

class bullet {
private:
    P pos;
};

class map {
private:
    uint time;
};

Player playerInfo[3];
int playerNum;
int playerScore[3];


/*----------함수 영역 시작----------*/
void cursor_pos(int x, int y)
{
    COORD p;
    p.X = x;
    p.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}
/*
int input_key()
{
    if (_kbhit())
        return _getch();
    return 0;
}
*/
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
    Sleep(1000);
}

void Score()
{
    string ranking[] = { "Player1", "Player2", "Player3" };
    int sorting[] = { 0, 1, 2 };
    int x = 45, y = 10;

    //sort();

    system("cls");
    cursor_pos(x, y);
    printf("=============================");
    cursor_pos(x + 5, y + 3);
    cout << "1st    " << ranking[sorting[0]] << "   " << playerScore[sorting[0]];
    cursor_pos(x + 5, y + 6);
    cout << "2nd    " << ranking[sorting[1]] << "   " << playerScore[sorting[1]];
    cursor_pos(x + 5, y + 9);
    cout << "3th    " << ranking[sorting[2]] << "   " << playerScore[sorting[2]];
    cursor_pos(x, y + 12);
    printf("=============================");

    while (1) {
        int key = input_key_dir();
        if (key == 0) continue;
        else break;
    }
}

void GameOver(int score)
{
    return;
}

int Playing()
{
    int score = 0;

    return score;
}

int main()
{
    system("mode con:cols=120 lines=35");                       // 콘솔 창 가로, 세로
    system("title Team10 Google Dinosaurs Game Plus Version");  // 콘손 창 제목

    while (1)
    {
        int gameScore = 0;

        switch (MainMenu()) {
        case 0: GameStart(); Count3sec(); gameScore = Playing(); break;
        case 1: Score(); break;
        case 2: system("cls"); return 0;
        }

        GameOver(gameScore);
    }

    return 0;
}
