#ifndef __GAME_CONTROL_HEADER__
#define __GAME_CONTROL_HEADER__

#include "View.h"
#include "Model.h"
#include "Keyboard.h"

class GameControl {
    public:
        GameControl(View *view, Model *model, Keyboard *keyboard);
        int mainLoop();
    private:
        void handleKeys();
        View *view;
        Model *model; 
        Keyboard *keyboard;
};

#endif