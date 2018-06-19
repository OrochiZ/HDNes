#ifndef GAMETILE_H
#define GAMETILE_H

#include "common.h"
#include "classList.h"
#include "condition.h"
#include "tileReference.h"

struct replacement{
    int img;
    int x;
    int y;
    double brightness;
};

class gameTile
{
    public:
        gameTile();
        virtual ~gameTile();

        tileReference id;

        int objCoordX;
        int objCoordY;
        bool hFlip;
        bool vFlip;
        bool markForDelete;
        bool isUnique;

        bool hasReplacement;
        bool isDefault;
        vector<condition> conditions;
        vector<bool> conSigns;

        vector<replacement> aniFrames;

        void readLine(string s);
        string writeLine();
        string writeConditionNames();
        bool compareEqual(gameTile& t);
        void load(fstream& file);
        void save(fstream& file);
    protected:

    private:
};

#endif // GAMETILE_H
