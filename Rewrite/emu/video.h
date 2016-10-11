#ifndef VIDEO_H
#define VIDEO_H

#include "../classList.h"
#include <emuPart.h>

class video : public emuPart
{
    public:
        video();
        virtual ~video();
        string partName();

        void saveConfigLines(fstream* fs);
        void loadConfigVal(string hdr, string value);

        void startGame();
        void endGame();
    protected:

    private:
        SDL_Window* displayWindow;
        SDL_GLContext glcontext;
};

#endif // VIDEO_H