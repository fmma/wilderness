#include "MenuControl.h"
#include "Model.h"

#include <time.h>
#include <stdio.h>
#include <string.h>

#define MAX_PAGE_OPTIONS (10)

enum OptionType {
    Label, ExitOption, StringOption, RangeOption, EnumOption
};

struct OptionItem {
    const char *name;
    OptionType type;

    OptionItem() {
        name = "";
        type = Label;
    }

    OptionItem(const char *name, OptionType type) : name(name), type(type) {
    }

    union Values {
        struct StringOption {
            int maxResultSize;
            char *result;
        } stringOption;
        struct RangeOption {
            int min;
            int max;
            int result;
        } rangeOption;
        struct EnumOption {
            const char** enums;
            int nEnums;
            int result;
        } enumOption;
    } values;
};

struct MenuPage {
    Keyboard *keyboard;
    View *view;
    const char *title;
    OptionItem options[MAX_PAGE_OPTIONS];
    int nOptions;

    MenuPage(Keyboard *keyboard, View *view, const char *title, int nOptions) : keyboard(keyboard), view(view), title(title), nOptions(nOptions) {
    }

    int pageLoop() {
        int option = 0;
        view->clear();
        int my = view->height / 3 - (2 + nOptions) / 2;
        view->renderString(-1, my, title);
        int mx = view->renderString(-1, my + 2, options[0].name);
        if(options[0].type != Label) {
            view->renderString(mx - 4, my + 2, "[x]");
        }
        else {
            option = 1;
        }
        for(int i = 1; i < nOptions; ++i) {
            if(options[i].type != Label) {
                if(option == i) {
                    view->renderString(mx - 4, my + 2 + i, "[x]");
                }
                else {
                    view->renderString(mx - 4, my + 2 + i, "[ ]");
                }
            }
            else {
                if(option == i) {
                    option++;
                }
            }
            view->renderString(mx , my + 2 + i, options[i].name);
            if(options[i].type == EnumOption) {
                view->renderString(mx + strlen(options[i].name), my + 2 + i, options[i].values.enumOption.enums[options[i].values.enumOption.result]);
            }
            if(options[i].type == StringOption) {
                view->renderString(mx + strlen(options[i].name), my + 2 + i, options[i].values.stringOption.result);
            }
        }
        
        struct timespec req, rem;
        while(1) {
            req.tv_nsec = 10000000;
            req.tv_sec = 0;
            nanosleep(&req, &rem);
            this->keyboard->pollKeys();
            if(this->keyboard->keyClicked(KEY_UP)) {
                int newOption = option == 0 ? nOptions - 1 : option - 1;
                view->renderChar(mx -3, my + 2 + option, ' ');
                view->renderChar(mx -3, my + 2 + newOption, 'x');
                option = newOption;
            }
            if(this->keyboard->keyClicked(KEY_DOWN)) {
                int newOption = option == nOptions - 1 ? 0 : option + 1;
                view->renderChar(mx -3, my + 2 + option, ' ');
                view->renderChar(mx -3, my + 2 + newOption, 'x');
                option = newOption;
            }

            OptionItem *item = options + option;
            OptionType type = item->type;

            if(this->keyboard->keyClicked(KEY_RETURN)) {
                if(type == ExitOption)
                    return option;
                if(type == EnumOption) {
                    OptionItem::Values::EnumOption *enumOption = &item->values.enumOption;
                    enumOption->result++;
                    if(enumOption->result >= enumOption->nEnums) {
                        enumOption->result = 0;
                    }
                    const char *newEnum = enumOption->enums[enumOption->result];
                    view->renderString(mx + strlen(item->name), my + 2 + option, newEnum);
                }
            }

            if(type == StringOption) {
                OptionItem::Values::StringOption *stringOption = &item->values.stringOption;
                int i = strlen(stringOption->result);
                int nTyped = keyboard->typedKeys(stringOption->result + i, stringOption->maxResultSize - i);
                if(nTyped > 0) {
                    i += nTyped;
                    stringOption->result[i] = 0;
                    view->renderString(mx + strlen(item->name), my + 2 + option, stringOption->result);
                }
                if(this->keyboard->keyClicked(KEY_BACKSPACE) && i > 0) {
                    i--;
                    stringOption->result[i] = 0;
                    view->renderChar(mx + strlen(item->name) + i, my + 2 + option, ' ');
                }
            }
        }
    }
};

MenuControl::MenuControl(View *view, Keyboard *keyboard) : view(view), keyboard(keyboard) {
}

MenuAction MenuControl::getMenuAction() {
    MenuPage page{keyboard, view, "--== WILD ==--", 2};
    page.options[0] = OptionItem{"New game", ExitOption};
    page.options[1] = OptionItem{"Exit", ExitOption};
    switch(page.pageLoop()) {
        case 0:
            return NewGame;
        case 1:
            return ExitGame;
    }
    return ExitGame;
}

Model *MenuControl::getModel() {
    Model *model = new Model();
    int nameIndex = 1;
    int difficultyIndex = 2;
    MenuPage page{keyboard, view, "Game settings", 4};
    page.options[0] = OptionItem{"Go!", ExitOption};
    page.options[nameIndex] = OptionItem{"Name: ", StringOption};
    page.options[difficultyIndex] = OptionItem{"Difficulty: ", EnumOption};
    page.options[3] = OptionItem{"Back", ExitOption};

    page.options[nameIndex].values.stringOption.result = model->player.name;
    page.options[nameIndex].values.stringOption.maxResultSize = sizeof(model->player.name) / sizeof(char) - 1;

    page.options[difficultyIndex].values.enumOption.nEnums = 3;
    page.options[difficultyIndex].values.enumOption.result = 0;
    page.options[difficultyIndex].values.enumOption.enums = new const char *[3] {
        " Easy ", 
        "Normal", 
        " Hard "};
    
    int result = page.pageLoop();
    delete page.options[difficultyIndex].values.enumOption.enums;
    
    switch(result) {
        case 0:
            model->difficulty = page.options[difficultyIndex].values.enumOption.result;
            model->seed = time(0);
            model->generateMap();
            return model;
        case 2:
            delete model;
            return 0;
    }
    return 0;
}
