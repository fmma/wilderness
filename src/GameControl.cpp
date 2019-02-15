#include "GameControl.h"

#include <stdio.h>
#include <time.h>

double clockToMilliseconds(clock_t ticks){
    // units/(units/time) => time (seconds) * 1000 = milliseconds
    return (ticks/(double)CLOCKS_PER_SEC)*1000.0;
}

GameControl::GameControl(View *view, Model *model, Keyboard *keyboard) : view(view), model(model), keyboard(keyboard) {

}

int GameControl::mainLoop() {
    struct timespec req{0, 10 * 1000000};
    time_t t, t0 = time(NULL);
    int frames = 0;
    char buf[50];
    view->clear();
    bool showFps = 1;

    while(1) {

        model->renderMap(view);

        if(showFps) {
            frames++;
            if(!(frames & 7)) {
                t = time(NULL);
                sprintf(buf, "FPS: %f", frames / difftime(t, t0));
            }
            view->writeString(0,0, buf);
        }

        view->render();
        nanosleep(&req, NULL);

        handleKeys();
    }
    return 0;
}

void GameControl::handleKeys() {
    this->keyboard->pollKeys();
    if(keyboard->keyClicked(KEY_LEFT)) {
        model->player.x--;
    }
    if(keyboard->keyClicked(KEY_RIGHT)) {
        model->player.x++;
    }
    if(keyboard->keyClicked(KEY_UP)) {
        model->player.y--;
    }
    if(keyboard->keyClicked(KEY_DOWN)) {
        model->player.y++;
    }
}