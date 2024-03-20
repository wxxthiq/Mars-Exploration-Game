#include"Rover.h"
#include<iostream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <windows.h>
#include <vector>
#include <conio.h>
#include"colors.h"
using namespace std;


const char RIGHT = char(16);
const char LEFT = char(17);
const char UP = char(30);
const char DOWN = char(31);

Rover::Rover(Map *ptr)
{
    p_mars = ptr;

    int Y = p_mars->getDimY();
    int X = p_mars->getDimX();

    goldTarget = p_mars->getGold();
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
    losing();
}

void Rover::winning()
{
    cout << BOLDYELLOW << "*   *   oo   +  +   @   @@   @   00   $   $" << endl;
    cout << BOLDGREEN  << " ***   o  o  +  +    @@@  @@@   0  0  $ $ $" << endl;
    cout << BOLDRED    << "  *    o  o  +  +     @    @    0  0  $   $" << endl;
    cout << BOLDCYAN   << "  *     oo    ++      @    @     00   $   $" << RESET << endl;

    endGame = true;
}

void Rover::losing()
{
    cout << BOLDYELLOW << "*   *   oo   +  +   @     00    SSS  TTT " << endl;
    cout << BOLDGREEN  << " ***   o  o  +  +   @    0  0  SS     T  " << endl;
    cout << BOLDRED    << "  *    o  o  +  +   @    0  0     SS  T  " << endl;
    cout << BOLDCYAN   << "  *     oo    ++    @@@@  00    SSS   T  "<< RESET << endl;

    endGame = true;
}


void Rover:: commandExecutable(char ch)
{
    showRoverHealth();
    showRoverFuel();

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
        cout << "Command: Executing (" << command[i] << ")..." << endl;
        Sleep(1200);
        system("cls");
        marsHidden.display();
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
    showRoverHealth();
    setFuelBar();
    showRoverFuel();
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

void Rover::goldScore()
{
    goldCollected += 1;
    if ( goldCollected == goldTarget )
        winning();
}

void Rover::displayScore()
{
    cout << "Total Commands Sequence: " << numOfCommandSequence << " [S]" << endl;
    cout << "Total Commands: " << numOfCommands<< " [C]" <<endl;
    cout << "Total Golds: " << goldCollected << " [G] out of " << goldTarget << endl;
    
    score = (goldCollected*50)-(numOfCommandSequence*5)-(numOfCommandSequence*1);
    
    cout<<"Total Score: [G] x 50 - [S] x 5 - [C] x 1 = "<<score<<endl;
}
