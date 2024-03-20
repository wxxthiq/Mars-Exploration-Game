// map.h

#pragma once 

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




