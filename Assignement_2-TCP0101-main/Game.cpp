#include <iostream>
#include <iomanip>
#include <time.h>
#include <cmath>
#include <windows.h>
#include <vector>
#include <conio.h>
#include"Mars.h"
#include"Rover.h"
#include"colors.h"

using namespace std;

void userInstructions()
{
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
    Rover marsRover(&mars);

    mars.resizeMap();
    mars.initMap(objectList);
    mars.setGold();
    mars.goldScatter();
    mars.display();
    marsRover.land();
    userInstructions();

    gameOn(mars, marsRover);
}

void gameOn(Map mars, Rover marsRover)
{
    bool quit = marsRover.endGame;
    
    while(!quit)
    {
        marsRover.setCommand();
        marsRover.takeAction();
        userInstructions();
        quit = marsRover.endGame;
    }

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