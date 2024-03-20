#include"Mars.h"
#include<iostream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <windows.h>
#include <vector>
#include <conio.h>
#include"colors.h"
using namespace std;

Map::Map(int Y, int X)
{
    this->dimY = Y;
    this->dimX = X;
}
int Map::getDimY()
{
    return dimY;
}

int Map::getDimX()
{
    return dimX;
}

int Map::getGold()
{
    return gold;
}

void Map::setDimX(int X)
{
    this->dimX = X;
}

void Map::setDimY(int Y)
{
    this->dimY = Y;
}

char Map::getObject(int Y, int X)
{
    char ch;
    ch = map[dimY-Y][X-1];
    
    return ch;
}

void Map::setObject (int X,int Y, char ch)
{
    map[dimY-Y][X-1] = ch;
}

void Map::resizeMap()
{
    map = new char*[dimY];

    for (int i = 0; i < dimY; i++)
        map[i] = new char[dimX];

    for (int i = 1; i <= dimY; i++)
    {
        for (int j = 1; j <= dimX; j++)
            setObject(j, i, ' ');
    }
}

void Map::initMap(char charlist[])
{
    int y = round(dimY/2.0);
    int x = round(dimX/2.0);

    for (int i = dimY; i >= 1; i--)
    {
        for(int j = 1; j <= dimX; j++)
        {
            int r = rand() % 30;
            char ch = charlist[r];

            if ( roverPosition(i, j) || !validPosition(i, j))
                continue;
            else
            {
                if( roverStuck(y,x) )
                    continue;
                else
                    setObject(j, i, ch);
            }
        }
    }
}

bool Map::roverStuck(int y, int x)
{
    bool roverBlocked = false;

    if ( !(isEmpty(x, y-1) || isEmpty(x, y+1) ||
           isEmpty(x-1, y) || isEmpty(x+1, y)) )
           roverBlocked = true;
    
    return roverBlocked;
}

bool Map::validPosition(int Y, int X)
{
    bool valid = false;

    if ( dimY - Y == 0 )
        valid = true;

    else if ( X == 1 ){
        if( isEmpty(X, Y+1) && isEmpty(X+1, Y+1) )
            valid = true;
    }

    else if ( X == dimX ){
        if( isEmpty(X, Y+1) && isEmpty(X-1, Y+1) )
            valid = true;
    }

    else if ( isEmpty(X, Y+1) && isEmpty(X-1, Y+1) && isEmpty(X-1, Y) )
        valid = true;

    return valid;
}

bool Map::goldNotBlocked(int x, int y)
{
    char H = 240;
    bool goldReachable = false;

    if ( isEmpty(x, y-1) || isEmpty(x, y+1) ||
         isEmpty(x-1, y) || isEmpty(x+1, y) )
         goldReachable = true; 

    return goldReachable;
}

void Map::setGold()
{
    cout << "Enter the amount of gold: ";
    cin >> gold;
}

void Map::goldScatter()
{
    int goldCount = 0;
    char G = 4;
    
    while (goldCount != gold)
    {    
        int y = rand()% dimY + 1;
        int x = rand()% dimX + 1;
        
        if (isEmpty(x,y) && !roverPosition(y,x) && goldNotBlocked(x,y))
        {
            setObject(x, y, G);
            goldCount++;
        }
    }
}



void Map::firstLine()
{
    cout << "  ";
    cout << CYAN << char(201) << RESET;
    for (int i = 0; i < dimX; i++)
    {
        if (i < dimX-1)
            cout << CYAN << char(205) << char(205) << char(205) << char(203) << RESET;
        else
            cout << CYAN << char(205) << char(205) << char(205) << char(187) << RESET;
    }
    cout << endl;
}

void Map::middleLine()
{
    cout << "  ";
    cout << CYAN << char(204) << RESET;
    for (int i = 0; i < dimX; i++)
    {
        if (i < dimX-1)
            cout << CYAN << char(205) << char(205) << char(205) << char(206) << RESET;
        else
            cout << CYAN << char(205) << char(205) << char(205) << char(185) << RESET;
    }

    cout << endl;
}

void Map::lastLine()
{
    cout << "  ";
    cout << CYAN << char(200) << RESET;
    for (int i = 0; i < dimX; i++)
    {
        if (i < dimX-1)
            cout << CYAN << char(205) << char(205) << char(205) << char(202) << RESET;
        else
            cout << CYAN << char(205) << char(205) << char(205) << char(188) << RESET;
    }

    cout << endl;
}

void Map::initMap()
{
    char ch = '?';
    
    for (int i = 1; i <= dimY; i++)
    {
        for(int j = 1; j <= dimX; j++)
            if ( roverPosition(i, j) )
                setObject(j, i, ' ');
            else
                setObject(j, i, ch);
    }
}

void Map::colorManager(int i, int j)
{
    if ( map[i][j] == char(4) ) // Gold
        cout << YELLOW << setw(2) << map[i][j] << RESET;
    
    else if ( map[i][j] == '#' ) // Trap
        cout << BOLDRED << setw(2) << map[i][j] << RESET;
    
    else if ( map[i][j] == char(237) ) // Rock
        cout << RED << setw(2) << map[i][j] << RESET;
    
    else if ( map[i][j] == char(15) ) // Fixer
        cout << GREEN << setw(2) << map[i][j] << RESET;
    
    else if ( map[i][j] == char(178) ) // Hill
        cout << MAGENTA << setw(2) << map[i][j] << RESET;

    else if ( map[i][j] == char(233) ) // Hole
        cout << BOLDMAGENTA << setw(2) << map[i][j] << RESET;
    
    else if ( map[i][j] == '?' ) // Hidden
        cout << BLUE << setw(2) << map[i][j] << RESET;

    else if ( map[i][j] == char(16) || map[i][j] == char(17) ||
              map[i][j] == char(30) || map[i][j] == char(31) ) // Rover
        cout << BOLDGREEN << setw(2) << map[i][j] << RESET;
    else
        cout << setw(2) << map[i][j];
}

void Map::display()
{
    firstLine();
    for (int i = 0; i < dimY; i++)
    {
        cout << setw(2) << (dimY-i) << CYAN << char(186) << RESET;
        for (int j = 0; j < dimX; j++){
            colorManager(i, j);   
            cout << CYAN << setw(2) << char(186) << RESET;  
        }
        cout << endl;
        if (i < dimY - 1)
            middleLine();
        else
            lastLine(); 
    }

    if ( dimX >= 10 )
    {
        cout << " ";
        for (int i = 1; i <= dimX; i++)
        {
            int num = i/10;
            if ( num == 0)
                cout << setw(4) << " ";
            else
                cout << setw(4) << num;
        }
        cout << endl;
    }

    cout << " ";
    for (int i = 1; i <= dimX; i++)
        cout << setw(4) << i%10;

    cout << endl << endl;
}

bool Map::isEmpty(int X, int Y)
{
    bool empty = false;
    
    if (isInsideMap(X, Y))
        if ( map[dimY-Y][X-1] == ' ')
            empty = true;

    return empty;
}

bool Map::isHole(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == char(233) )
        flag = true;
    
    return flag;
}

bool Map::isTrap(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == '#')
        flag = true;
    
    return flag;
}

bool Map::isGold(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == char(4))
        flag =  true;

    return flag;
}

bool Map::isHill(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == char(240))
        flag =  true;

    return flag;
}

bool Map::isRock(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == char(237))
        flag =  true;

    return flag;
}

bool Map::isRepair(int X, int Y)
{
    bool flag = false;

    if ( map[dimY-Y][X-1] == char(15))
        flag =  true;

    return flag;
}

bool Map::isInsideMap(int X, int Y)
{
    bool inside = true;

    if ( !(( Y <= dimY && Y > 0 ) && ( X <= dimX && X > 0 )) )
        inside = false;

    return inside;
}

bool Map::roverPosition(int Y, int X)
{
    int posX = round(dimX/2.0);
    int posY = round(dimY/2.0);
    bool isRoverPosition = false;

    if ( ( X == posX ) && ( Y == posY ) )
        isRoverPosition = true;
    
    return isRoverPosition;
}

