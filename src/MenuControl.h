#ifndef __MENU_CONTROL_HEADER__
#define __MENU_CONTROL_HEADER__

#include "View.h"
#include "Model.h"
#include "Keyboard.h"

enum MenuAction {
    ExitGame, NewGame
};

class MenuControl {
    public:
        MenuControl(View *view, Keyboard *keyboard);
        MenuAction getMenuAction();
        Model *getModel();
    private:
        View *view;
        Keyboard *keyboard;
};

#endif