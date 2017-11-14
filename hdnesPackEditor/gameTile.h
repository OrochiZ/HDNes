#ifndef GAMETILE_H
#define GAMETILE_H

#include "common.h"
#include "classList.h"
#include "condition.h"

class gameTile
{
    public:
        gameTile();
        virtual ~gameTile();

        int id;
        Uint8 rawData[16];
        Uint8 palette[4];

        int objCoordX;
        int objCoordY;
        bool hFlip;
        bool vFlip;
        bool markForDelete;

        bool hasReplacement;
        bool isDefault;
        vector<condition> conditions;
        int img;
        int x;
        int y;
        double brightness;

        void readLine(string s);

    protected:

    private:
};

#endif // GAMETILE_H
