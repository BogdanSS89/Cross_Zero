#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>   
using namespace std;


enum Cell {
    CROSS='X',
    ZERO='0',
    EMPTY='_'
};

enum Progress {
    IN_GAME,
    WON_PLAYER,
    WON_COMP,
    DRAW
};

struct Field {
    Cell** ppField = nullptr;
    const size_t size = 3;
    size_t turn = 0;
    Cell  player = EMPTY;
    Cell comp = EMPTY;
    Progress progress = IN_GAME;
};

struct Coord {
    size_t y;
    size_t x;
};

int32_t getRandomNum(int32_t min, int32_t max)
{
    const static auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937_64 generator(seed);
    uniform_int_distribution<int32_t> dis(min, max);
    return dis(generator);
}



//==========================================================================================================
void ClearScr()
{
    system("cls");
}
void WellDone(Field& f)
{
    if (f.progress == WON_PLAYER)
    {
        cout << " Player won! Well done!" << endl;
    }
    else if (f.progress == WON_COMP)
    {
        cout << " Computer won!" << endl;
    }
    else if (f.progress == DRAW)
    {
        cout << " Draw!" << endl;
    }
}

void InitGame(Field& f)
{
    f.ppField = new Cell * [f.size];
    for (size_t y = 0; y < f.size; y++)
    {
        f.ppField[y] = new Cell[f.size];
    }

    for (size_t y = 0; y < f.size; y++)
    {
        for (size_t x = 0; x < f.size; x++)
        {
            f.ppField[y][x] = EMPTY;
        }
    }

    if (getRandomNum(0, 1000) > 500)
    {
        f.player = CROSS;
        f.comp = ZERO;
        f.turn = 0;

    }
    else
    {
        f.player = ZERO;
        f.comp = CROSS;
        f.turn = 1;
    }
}

void DeInitGame(Field& f)
{
    for (size_t y = 0; y < f.size; y++)
    {
        delete[] f.ppField[y];
    }

    delete[] f.ppField; 
}

void DrawField(const Field& f)
{
    cout <<endl<< "       ";
    for (size_t x = 0; x < f.size; x++)
    {
        cout << x + 1 << "      ";
    }
    cout << endl;

    for (size_t y = 0; y < f.size; y++)
    {
        cout << " " << y + 1 << "  |  ";
        for (size_t x = 0; x < f.size; x++)
        {
            cout <<(char) f.ppField[y][x] << "  |  ";

        }
        cout << endl;
    }

    cout << endl << " Player: " << (char)f.player << endl << " Computer: " << (char)f.comp << endl;
}

Coord GetPlayerCoord( Field & f)
{
    Coord c{ 0 };

    cout << endl;

    do
    {
        cout << " Enter coord X(1..3): ";
        cin >> c.x;
        cout << " Enter coord Y(1..3): ";
        cin >> c.y;

    } while (c.x<=0||c.y<=0||c.x>3||c.y>3||f.ppField[c.y-1][c.x-1]!=EMPTY);

    c.x--;
    c.y--;

    return c;
}

Progress IsWon(const Field& f)
{
    for (size_t y = 0; y < f.size; y++)
    {
        if (f.ppField[y][0]==f.ppField[y][1] && f.ppField[y][0]==f.ppField[y][2])
        {
            if (f.ppField[y][0]==f.player)
            {
                return WON_PLAYER;
            }
            else if (f.ppField[y][0]==f.comp)
            {
                return WON_COMP;

            }
        }
    }

    for (size_t x = 0; x < f.size; x++)
    {
        if (f.ppField[0][x] == f.ppField[1][x] && f.ppField[0][x] == f.ppField[2][x])
        {
            if (f.ppField[0][x] == f.player)
            {
                return WON_PLAYER;
            }
            else if (f.ppField[0][x] == f.comp)
            {
                return WON_COMP;

            }
        }
    }


    if (f.ppField[2][0]==f.ppField[1][1] && f.ppField[2][0] == f.ppField[0][2])
    {
        if (f.ppField[1][1]==f.player)
        {
            return WON_PLAYER;
        }
        else if (f.ppField[1][1] == f.comp)
        {
            return WON_COMP;
        }
    }


    if (f.ppField[0][0] == f.ppField[1][1] && f.ppField[0][0] == f.ppField[2][2])
    {
        if (f.ppField[1][1] == f.player)
        {
            return WON_PLAYER;
        }
        else if (f.ppField[1][1] == f.comp)
        {
            return WON_COMP;
        }
    }
    bool draw = true;
    for (size_t y = 0; y < f.size; y++)
    {
        for (size_t x = 0; x < f.size; x++)
        {
            if (f.ppField[y][x] == EMPTY)
            {
                draw = false;
                    break;
           }
        }
        if (!draw)
        {
            break;
        }
    }
    if (draw)
    {
        return DRAW;
    }

    return IN_GAME;
}

Coord GetCompCoord(Field& f)
{
    Coord c{ 0 };

    for (size_t y = 0; y < f.size; y++)
    {

        for (size_t x = 0; x < f.size; x++)
        {
            if (f.ppField[y][x] == EMPTY)
            {
                f.ppField[y][x] = f.comp;
                if (IsWon(f)==WON_COMP)
                {
                    f.ppField[y][x] = EMPTY;
                    return{ y,x };
                }
                f.ppField[y][x] = EMPTY;
            }
        }
    }

    for (size_t y = 0; y < f.size; y++)
    {

        for (size_t x = 0; x < f.size; x++)
        {
            if (f.ppField[y][x] == EMPTY)
            {
                f.ppField[y][x] = f.player;
                if (IsWon(f) == WON_PLAYER)
                {
                    f.ppField[y][x] = EMPTY;
                    return{ y,x };
                }
                f.ppField[y][x] = EMPTY;
            }
        }
    }

    if (f.ppField[1][1] == EMPTY)
    {
        
         return{ 1,1 };

    }

    Coord buf[4];
    size_t num = 0;

    if (f.ppField[0][0]==EMPTY)
    {
        buf[num] = { 0,0 };
        num++;
    }
    if (f.ppField[0][0] == EMPTY)
    {
        buf[num] = { 2,2 };
        num++;
    }
    if (f.ppField[0][0] == EMPTY)
    {
        buf[num] = { 2,0 };
        num++;
    }
    if (f.ppField[0][0] == EMPTY)
    {
        buf[num] = { 0,2 };
        num++;
    }



    if (num!=0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }


    
    num = 0;
    if (f.ppField[0][1] == EMPTY)
    {
        buf[num] = { 0,1 };
        num++;
    }
    if (f.ppField[2][1] == EMPTY)
    {
        buf[num] = { 2,1 };
        num++;
    }
    if (f.ppField[1][0] == EMPTY)
    {
        buf[num] = { 1,0 };
        num++;
    }
    if (f.ppField[1][2] == EMPTY)
    {
        buf[num] = { 1,2 };
        num++;
    }


    if (num != 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }


    return c;
}



//===================================================================================================
int main()
{
    Field f;
    InitGame(f);
    ClearScr();
    DrawField(f);

    do {

        if (f.turn % 2 == 0)
        {
            Coord c = GetPlayerCoord(f);
            f.ppField[c.y][c.x] = f.player;
        }
        else
        {
            Coord c = GetCompCoord(f);
            f.ppField[c.y][c.x] = f.comp;
        }

        f.turn++;
        ClearScr();
        DrawField(f);

        f.progress = IsWon(f);

    } while (f.progress == IN_GAME);

    cout << endl;
   
    WellDone(f);
    DeInitGame(f);
    return 0;
}

