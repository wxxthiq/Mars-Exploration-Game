#include <iostream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <windows.h>
#include <vector>
#include <conio.h>
using namespace std;

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

const char RIGHT = char(16);
const char LEFT = char(17);
const char UP = char(30);
const char DOWN = char(31);

class Map
{
    private:
        char **map;
        int dimY, dimX, gold;

    public:
        
        Map(){};
        Map(int, int);
        
        ~Map()
        {
            for (int i = 0; i < dimX; i++)
                delete[] map[i];
            delete[] map;
        }
        void resizeMap();
        void initMap(char[]);
        void initMap();
        void display();
        void firstLine();
        void middleLine();
        void lastLine();
        void setObject(int, int, char);
        char getObject(int, int);
        void setDimY(int);
        void setDimX(int);
        void setGold();
        void goldScatter();
        bool roverStuck(int, int);
        bool goldNotBlocked(int, int);
        bool isHill(int, int);
        bool isTrap(int, int);
        bool isRock(int, int);
        bool isRepair(int, int);
        bool isHole(int, int);
        bool isEmpty(int, int);
        bool isGold(int, int);
        bool isInsideMap(int , int);
        bool roverPosition(int, int);
        bool validPosition(int, int);

        void colorManager(int, int);
        
        int getGold();
        int getDimY();
        int getDimX();
};

Map::Map(int Y, int X)
{
    this->dimY = Y;
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

int Map::getGold()
{
    return gold;
}

void Map::setDimX(int X)
{
    this->dimX = X;
}

class Rover
{
    private:
        int posX, posY;
        char roverHeading;
        int health, fuel, moves = 0, score = 0;
        int goldCollected = 0;
        int numOfCommands = 0;
        int numOfCommandSequence = 0;
        int goldTarget;

        bool execution;
        vector<char> fuelBar;
        vector<char> healthBar;
        string command;
        Map *p_mars;
        Map marsHidden;
    
    public:
        bool endGame = false;
        bool win;
        Rover(Map *ptr);
        void setRoverFuel();
        void setRoverHealth();
        void placeRoverRandomly();
        void setFuelBar();
        void setHealthBar();
        void showRoverFuel();
        void showRoverHealth();
        //char getHead() {return roverHeading;}
        void controlHealth(int, int);
        void controlFuel(int, int);
        void land();
        void move();
        void turnLeft();
        void turnRight();
        void setCommand();
        void takeAction();
        void executeCommand(char);
        void commandExecutable(char);
        void loseMessage();
        void threeDisappear();
        void goldScore();
        void displayScore();
        void quit();
        void userInstructions();
        // bool checkQuit();
};

void Rover:: userInstructions()
{
    showRoverHealth();
    showRoverFuel();
    
    cout << BOLDWHITE << "Mission: Get all the golds!! Do not get trapped!!\n" << RESET << endl;
    cout << "L => Turn Left | << R => Turn Right |"
         << " M => Move |" << " Q => Quit |" <<endl;
    cout << "=============================================================" << endl;
    cout << setw(2) << MAGENTA << char(178) << RESET << setw(11) << "=> HILL |" << setw(2) << BOLDRED << '#'
         << RESET << setw(11) << "=> TRAP |" << setw(2) << YELLOW << char(4) << RESET << setw(11) << "=> GOLD |" << endl 
         << setw(2) << RED << char(237) << RESET << setw(11) << "=> ROCK |" << setw(2) << GREEN << char(15) 
         << RESET << setw(11) << "=> Fixer |" << setw(2) << BOLDMAGENTA << char(233) << RESET << setw(11) << "=> Hole |"
         << endl << endl;
}

Rover::Rover(Map *ptr)
{
    p_mars = ptr;

    int Y = p_mars->getDimY();
    int X = p_mars->getDimX();

    goldTarget = p_mars->getGold();
    //cout<<"PMARS GET GOLD"<<p_mars->getGold();
    marsHidden.setDimY(Y);
    marsHidden.setDimX(X);
    marsHidden.resizeMap();
    marsHidden.initMap();
    setRoverHealth();
    setRoverFuel();
}

void Rover::placeRoverRandomly()
{
    int Y;
    int X;

    do
    {
        Y = rand()% p_mars->getDimY() + 1;
        X = rand()% p_mars->getDimX() + 1;

    } while ( p_mars->roverStuck(Y, X) || !p_mars->isEmpty(X,Y));

    posX = X;
    posY = Y;
    marsHidden.setObject(X,Y,roverHeading);
}

void Rover::controlHealth(int Y, int X)
{
    char F = 15;
    char R = 237;

    char ch = p_mars->getObject(Y,X);
    
    if ( ch == F && health != p_mars->getDimY() )
        health++;
    
    else if ( ch == R )
        health--;
}

void Rover::controlFuel(int Y, int X)
{
    char F = 15;

    char ch = p_mars->getObject(Y,X);
    
    if ( ch == F && fuel != p_mars->getDimY() )
        fuel = p_mars->getDimY();
    
    else if ( moves == p_mars->getDimY() - 1 )
    {
        moves = 0;
        fuel--;
    }
}

void Rover::setRoverFuel()
{
    fuel = p_mars->getDimY() * 1.5;
}

void Rover::setRoverHealth()
{
    health = p_mars->getDimY();
}

void Rover::setFuelBar()
{
    char ful = 254; 

    for (int i = 0; i < fuel; i++)
        fuelBar.push_back(ful);
}

void Rover::setHealthBar()
{
    char hth = 254; 

    for (int i = 0; i < health; i++)
        healthBar.push_back(hth);
}

void Rover::showRoverHealth()
{
    int healtRemain = float(health)/p_mars->getDimY() * 100;

    cout << "Health [";
    for (int i = 0; i < p_mars->getDimY(); i++)
        if (i < health ){
            if ( healtRemain <= 100 && healtRemain > 70)
                cout << GREEN << healthBar[i] << healthBar[i] << RESET;
            else if ( healtRemain <= 70 && healtRemain > 40 )
                cout << YELLOW << healthBar[i] << healthBar[i] << RESET;
            else
                cout << RED << healthBar[i] << healthBar[i] << RESET;
        }
        else
            cout << ' ' << ' ';
    
    cout << "] " << healtRemain << " %" << endl;
    
}

void Rover::showRoverFuel()
{
    int fuelRemain = float(fuel) / ( int(p_mars->getDimY() * 1.5) ) * 100;
    int fuelMax = p_mars->getDimY() * 1.5;

    cout << "Fuel [";
    for (int i = 0; i < fuelMax; i++)
        if (i < fuel ){
            if ( fuelRemain <= 100 && fuelRemain > 70)
                cout << GREEN << fuelBar[i] << fuelBar[i] << RESET;
            else if ( fuelRemain <= 70 && fuelRemain > 40 )
                cout << YELLOW << fuelBar[i] << fuelBar[i] << RESET;
            else
                cout << RED << fuelBar[i] << fuelBar[i] << RESET;
        }
        else
            cout << ' ' << ' ';
    
    cout << "] " << fuelRemain << " %" << endl << endl;    
}

void Rover::loseMessage()
{
    cout<<"GAME OVER,YOU HAVE QUIT"<<endl;
}

void Rover::setCommand()
{
    cout << "Enter the command sequence => ";
    cin >> command;

    numOfCommandSequence +=1;
    numOfCommands += command.length();
}

void Rover::quit()
{
    endGame=true;
    win = false;
}



void Rover:: commandExecutable(char ch)
{
    // showRoverHealth();
    // showRoverFuel();

    if (execution)
        cout << "Command: Executed (" << ch << ")" << endl;
    else
        cout << "Command: Failed to execute (" << ch << ")" << endl;

    Sleep(1200);
    system("cls");
    marsHidden.display();    
}

void Rover::takeAction()
{
    for (int i = 0; i < command.length();i++)
    {
        executeCommand(command[i]);
        system("cls");
        marsHidden.display();
        showRoverHealth();
        showRoverFuel();

        cout << "Command: Executing (" << command[i] << ")..." << endl;

        Sleep(1200);
        system("cls");
        marsHidden.display();
        showRoverHealth();
        showRoverFuel();
        commandExecutable(command[i]);
    }
}

void Rover:: executeCommand(char c)
{
    if (c == 'M')
        move();

    else
        if (c == 'R'){
            execution = true; 
            turnRight();
        }
        else 
            if (c == 'L'){
                execution = true; 
                turnLeft();
            }
            else
                if ( c == 'Q')
                    quit();
                else
                    execution = false;

    threeDisappear();
}

void Rover::land()
{
    char possibleHeading[] = {RIGHT, LEFT, UP, DOWN};

    posX = round(p_mars->getDimX()/(float)2);
    posY = round(p_mars->getDimY()/(float)2);
    
    roverHeading = possibleHeading[ rand() % 4 ];
    marsHidden.setObject(posX,posY,roverHeading);
    threeDisappear();
    marsHidden.display();
    setHealthBar();
    setFuelBar();
}
void Rover:: threeDisappear()
{
    if ( roverHeading == RIGHT ) //16 right 
    {
        if (p_mars->isInsideMap( posX+1,posY+1))
        {
            char object = p_mars->getObject(posY+1,posX+1); //REVERSING POS X AND POSY WILL GIVE INVERSE INVERSE BECAUSE FIRST MAP REFLECTS AT X=Y
            marsHidden.setObject(posX+1,posY+1,object);
        }
        if ( p_mars->isInsideMap(posX+1,posY))
        {
            char object = p_mars->getObject(posY,posX+1);
            marsHidden.setObject(posX+1,posY,object);
        }

        if ( p_mars->isInsideMap(posX+1,posY-1))
        {
            char object = p_mars->getObject(posY-1,posX+1);
            marsHidden.setObject(posX+1,posY-1,object);
        }
    } 

    else if (roverHeading == LEFT ) // 17 LEFT
    {
        if (p_mars->isInsideMap(posX-1,posY+1))
        {
            char object = p_mars->getObject(posY+1,posX-1);
            marsHidden.setObject(posX-1,posY+1,object);
        }
        if ( p_mars->isInsideMap(posX-1,posY))
        {
            char object = p_mars->getObject(posY,posX-1);
            marsHidden.setObject(posX-1,posY,object);
        }

        if ( p_mars->isInsideMap(posX-1, posY-1))
        {
            char object = p_mars->getObject(posY-1,posX-1);
            marsHidden.setObject(posX-1,posY-1,object);
        }
    }

    else if (roverHeading == UP ) //UP
    {
        if (p_mars->isInsideMap(posX,posY+1))
        {
            char object = p_mars->getObject(posY+1, posX);
            marsHidden.setObject(posX,posY+1,object);
        }
        if ( p_mars->isInsideMap(posX-1,posY+1))
        {
            char object = p_mars->getObject(posY+1, posX-1);
            marsHidden.setObject(posX-1,posY+1,object);
        }
        if ( p_mars->isInsideMap(posX+1,posY+1))
        {
            char object = p_mars->getObject(posY+1, posX+1);
            marsHidden.setObject(posX+1,posY+1,object);
        }
    }

    else if (roverHeading == DOWN ) //DOWN
    {
        if (p_mars->isInsideMap(posX+1,posY-1))
        {
            char object = p_mars->getObject(posY-1,posX+1);
            marsHidden.setObject(posX+1,posY-1,object);
        }
        if ( p_mars->isInsideMap(posX,posY-1))
        {
            char object = p_mars->getObject(posY-1,posX );
            marsHidden.setObject(posX,posY-1,object);
        }

        if ( p_mars->isInsideMap(posX-1, posY-1))
        {
            char object = p_mars->getObject(posY-1,posX-1);
            marsHidden.setObject(posX-1,posY-1,object);
        }
    }
}

void Rover::move()
{
    moves++;

    if ( roverHeading == RIGHT)
    {       
        if( p_mars->isInsideMap(posX+1, posY) )
        {
            if ( !p_mars->isHill(posX+1,posY) )  
            {
                marsHidden.setObject(posX, posY,' ');
                posX += 1;
                execution = true; 
            }
            else
                execution = false; // "Command Execution failed" << endl;
        }
        else
            execution = false; // "CANNOT GO OUT OF PLANET MARS!";
    }

    else if ( roverHeading == LEFT )
    {
        
        if ( p_mars->isInsideMap(posX-1, posY) )
        {
            if ( !p_mars->isHill(posX-1,posY) )
            {        
                marsHidden.setObject( posX, posY, ' ' );
                posX -= 1;
                execution = true; 
            }
            else
                execution = false; // "Command Execution failed" << endl;
        }
        else
            execution = false; // "CANNOT GO OUT OF PLANET MARS!";
    }

    else if (roverHeading == UP )
    {    
        if( p_mars->isInsideMap(posX, posY+1) )
        {
            if ( !p_mars->isHill(posX,posY+1) )
            {
                marsHidden.setObject(posX, posY,' ');
                posY += 1;
                execution = true; 
            }
            else
                execution = false; // "Command Execution failed" << endl;
        }
        else
            execution = false; // "CANNOT GO OUT OF MARS!";
    }

    else if ( roverHeading == DOWN )
    {
        if( p_mars->isInsideMap(posX, posY-1) )
        {
            if ( !p_mars->isHill(posX,posY-1) && !p_mars->isTrap(posX,posY-1))
            {
                marsHidden.setObject(posX, posY,' ');
                posY -= 1;
                execution = true; 
            }
            else
                execution = false; // "Command Execution failed" << endl;
        }
        else
            execution = false; // "CANNOT GO OUT OF MARS!";
    }

    if ( p_mars->isGold(posX, posY) )
        goldScore();
    
    if ( p_mars->isTrap(posX,posY) )
    {
        cout<<"TRAP.MISSION FAILED. HEADING BACK TO PLANET EARTH"<<endl;
        exit(EXIT_FAILURE);
    }

    else if ( p_mars->isHole(posX,posY) )
    {
        marsHidden.setObject(posX, posY, p_mars->getObject(posY, posX));
        placeRoverRandomly();
    }
    
    else
    {
        controlHealth(posY, posX);
        p_mars->setObject(posX, posY, ' ');
        marsHidden.setObject(posX, posY, roverHeading);
    }

    controlFuel(posY, posX);
}

void Rover::turnRight()
{
    if ( roverHeading == RIGHT )
        roverHeading = DOWN;
    else
        if ( roverHeading == LEFT )
            roverHeading = UP;
        else
            if ( roverHeading == UP )
                roverHeading = RIGHT;
            else
                roverHeading = LEFT;
    
    marsHidden.setObject(posX, posY, roverHeading);
}

void Rover::turnLeft()
{
    if ( roverHeading == RIGHT )
        roverHeading = UP;
    else
        if ( roverHeading == LEFT )
           roverHeading = DOWN;
        else
            if ( roverHeading == UP )
                roverHeading = LEFT;
            else
                roverHeading = RIGHT;

    marsHidden.setObject(posX, posY, roverHeading);
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

void Rover::goldScore()
{
    goldCollected += 1;
    if ( goldCollected == goldTarget )
    {
        endGame= true;
        win = true;
    }
        
}

void Rover::displayScore()
{
    // goldTarget not intialized
    cout << "Total Commands Sequence: " << numOfCommandSequence << " [S]" << endl;
    cout << "Total Commands: " << numOfCommands<< " [C]" <<endl;
    cout << "Total Golds: " << goldCollected << " [G] out of " << goldTarget << endl;
    
    score = (goldCollected*50)-(numOfCommandSequence*5)-(numOfCommandSequence*1);
    
    cout<<"Total Score: [G] x 50 - [S] x 5 - [C] x 1 = "<<score<<endl;
    cout<<"\n"<<endl;
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

int Map::getDimY()
{
    return dimY;
}

int Map::getDimX()
{
    return dimX;
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

    if ( map[dimY-Y][X-1] == char(178)) 
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

void showMap(Map);
void gameOn(Map, Rover);

void initGame()
{
    int dimX, dimY;
    char L = 178, // Hill
    R = 237, // Rock
    H = 233, // Hole 
    F = 15,  // Fixer / Repair Station
    T = 35;  // Trap

    char objectList[] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                            ' ',' ',' ',' ',' ', T , T , T , T , L ,
                             L , L , L , L , R , R , R , R , F , H };
    
    cout << "Enter Y Dimensions: ";
    cin >> dimY;
    cout << "Enter X Dimensions: ";
    cin >> dimX;

    Map mars( dimY, dimX );
    

    mars.resizeMap();
    mars.initMap(objectList);
    mars.setGold();
    mars.goldScatter();
    mars.display();
    Rover marsRover(&mars);
    marsRover.land();
    marsRover.userInstructions();
    marsRover.displayScore(); //displays score before initialized goldTarget

    gameOn(mars, marsRover);
}


void winning()
{
    cout << BOLDYELLOW << "*   *   oo   +  +   @   @@   @   00   $   $" << endl;
    cout << BOLDGREEN  << " ***   o  o  +  +    @@@  @@@   0  0  $ $ $" << endl;
    cout << BOLDRED    << "  *    o  o  +  +     @    @    0  0  $   $" << endl;
    cout << BOLDCYAN   << "  *     oo    ++      @    @     00   $   $" << RESET << endl;
}

void losing()
{
    cout << BOLDYELLOW << "*   *   oo   +  +   @     00    SSS  TTT " << endl;
    cout << BOLDGREEN  << " ***   o  o  +  +   @    0  0  SS     T  " << endl;
    cout << BOLDRED    << "  *    o  o  +  +   @    0  0     SS  T  " << endl;
    cout << BOLDCYAN   << "  *     oo    ++    @@@@  00    SSS   T  "<< RESET << endl;
}

void gameOn(Map mars, Rover marsRover)
{
    bool quit = marsRover.endGame;
    bool win = marsRover.win;
    while(!quit)
    {
        marsRover.setCommand();
        marsRover.takeAction();
        marsRover.userInstructions();
        marsRover.displayScore();
        quit = marsRover.endGame;
    }

    if (win)
        winning();
    else
        losing();

    // if statement for either winning or losing
    showMap(mars);
}

void playAgain()
{    
    char choice;
    bool playOn = true;
    do
    {
        cout << "Would You Like To Play Again => " << endl;
        choice = getch();
        
        if( choice == 'y' || choice == 'Y' )
            initGame();
        
        else if ( choice == 'n' || choice == 'N' )
            playOn = false;
    
    } while (playOn);

    exit(EXIT_SUCCESS);
}

void showMap(Map mars)
{
    char choice;
    bool valid = false;
    
    do
    {
        cout << "Would You Like To See Mars Map => " << endl;
        choice = getch();
        
        if( choice == 'y' || choice == 'Y' ){
            valid = true;
            mars.display();
        }
        else if( choice == 'n' || choice == 'N' ){
            valid = true;
            break;
        }

    } while(!valid); 
    
    playAgain();
}

int main()
{
    srand ( time(NULL) );

    initGame();

    return 0;
}
