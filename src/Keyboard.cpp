#include "Keyboard.h"

#include <windows.h>

struct KeyEvent {
    KEY_EVENT_RECORD eventRecord;
    bool isNew;
};

struct KeyboardInternal {
    int nKeyEvents;
    void *hStdin;
    void *hStdout;
    KeyEvent keyEvents[MAX_EVENTS];
    
    KeyboardInternal() {
        hStdin = GetStdHandle(STD_INPUT_HANDLE);
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    void unNew() {
        for(int i = 0; i < nKeyEvents; ++i) {
            keyEvents[i].isNew = 0;
        }
    }

    void addEvent(KEY_EVENT_RECORD &ev) {
        for(int i = 0; i < nKeyEvents; ++i) {
            if(keyEvents[i].eventRecord.wVirtualKeyCode == ev.wVirtualKeyCode) {
                keyEvents[i] = KeyEvent{ev, 0};
                return;
            }
        }
        if(nKeyEvents < MAX_EVENTS) {
            keyEvents[nKeyEvents] = KeyEvent{ev, 1};
            nKeyEvents++;
        }
    }

    void removeEvent(KEY_EVENT_RECORD &ev) {
        for(int i = 0; i < nKeyEvents; ++i) {
            if(keyEvents[i].eventRecord.wVirtualKeyCode == ev.wVirtualKeyCode) {
                for(int j = i + 1; j < nKeyEvents; ++j) {
                    keyEvents[j-1] = keyEvents[j];
                }
                nKeyEvents--;
                return;
            }
        }
    }
};

Keyboard::Keyboard() : internal(new KeyboardInternal()) {
};

Keyboard::~Keyboard() {
    delete this->internal;
};

void Keyboard::pollKeys() {
    this->internal->unNew();
    DWORD nEvents = 0;
    DWORD nEventsRead = 0;
    GetNumberOfConsoleInputEvents(this->internal->hStdin, &nEvents);
    if(nEvents > MAX_EVENTS) {
        nEvents = MAX_EVENTS;
    }
    if(nEvents > 0){
        INPUT_RECORD eventBuffer[nEvents];
        ReadConsoleInput(this->internal->hStdin, eventBuffer, nEvents, &nEventsRead);
        this->internal->nKeyEvents = nEventsRead;
        for(DWORD i = 0; i < nEventsRead; ++i){
            KEY_EVENT_RECORD ev = eventBuffer[i].Event.KeyEvent;
            if(ev.bKeyDown) {
                this->internal->addEvent(ev);
            }
            else {
                this->internal->removeEvent(ev);
            }
        }
    } 
};

bool Keyboard::keyClicked(char c) {
    for(int i = 0; i < this->internal->nKeyEvents; ++i) {
        if(this->internal->keyEvents[i].eventRecord.wVirtualKeyCode == c && this->internal->keyEvents[i].isNew) {
            return 1;
        }
    }
    return 0;
};

bool Keyboard::keyHold(char c) {
    for(int i = 0; i < this->internal->nKeyEvents; ++i) {
        if(this->internal->keyEvents[i].eventRecord.wVirtualKeyCode == c) {
            return 1;
        }
    }
    return 0;
};

int Keyboard::typedKeys(char *buffer, int bufferSize) {
    int n = 0;
    for(int i = 0; i < this->internal->nKeyEvents; ++i) {
        char c = this->internal->keyEvents[i].eventRecord.uChar.AsciiChar;
        if(c >= ' ' && c <= '~' && this->internal->keyEvents[i].isNew) {
            if(n < bufferSize) {
                *(buffer + n) = c;
            }
            ++n;
        }
    }
    return n;
}