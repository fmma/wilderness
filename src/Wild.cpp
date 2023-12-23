#include "MenuControl.h"
#include "GameControl.h"
#include "View.h"
#include "Model.h"
#include "Keyboard.h"

#include <stdio.h>

class Wild {
    public:
    int run() {
        View view{160, 49};
        Keyboard keyboard;
        MenuControl menu{&view, &keyboard};
        while(1) {
            MenuAction menuAction = menu.getMenuAction();
            switch(menuAction) {
                case ExitGame:
                    return 0;
                case NewGame:
                    Model *model = menu.getModel();
                    if(!model)
                        continue;
                    GameControl control{&view, model, &keyboard};
                    int result = control.mainLoop();
                    delete model;
                    if(result != 0)
                        return result;
            }
        }
        return 0;
    }
};


int main() {
#ifdef GEN_STUFF
    for(int i5 = 0; i5 < 2; ++i5)
    for(int i0 = 0; i0 < 2; ++i0) 
    {
    printf("/*\n");
    for(int i6 = 0; i6 < 2; ++i6)
    for(int i4 = 0; i4 < 2; ++i4)
    for(int i3 = 0; i3 < 2; ++i3)
    for(int i1 = 0; i1 < 2; ++i1)
    {
        printf("  %d%d%d%d  ", i1,i1,i3,i3);
    }
    printf("\n");
    for(int i6 = 0; i6 < 2; ++i6)
    for(int i4 = 0; i4 < 2; ++i4)
    for(int i3 = 0; i3 < 2; ++i3)    
    for(int i1 = 0; i1 < 2; ++i1)
    {
        printf(" %d%d11%d%d ", i0,i0,i5,i5);
    }
    printf("\n");
    
    for(int i6 = 0; i6 < 2; ++i6)
    for(int i4 = 0; i4 < 2; ++i4)
    for(int i3 = 0; i3 < 2; ++i3)
    for(int i1 = 0; i1 < 2; ++i1)
    {
        printf("  %d%d%d%d  ", i4, i4, i6,i6);
    }
    printf("*/\n");
    for(int i6 = 0; i6 < 2; ++i6)
    for(int i4 = 0; i4 < 2; ++i4)
    for(int i3 = 0; i3 < 2; ++i3)
    for(int i1 = 0; i1 < 2; ++i1)
    {
        printf("  \"??\"  ");
    }
    printf("\n");
    }
    exit(0);
#endif
    Wild wild;
    return wild.run();
}
