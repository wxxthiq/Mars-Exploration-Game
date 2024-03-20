#include "Mars.h"
#include<vector>
#include<string>

using namespace std;

#pragma once

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
        void losing();
        void winning();
        void quit();
        // bool checkQuit();
};
