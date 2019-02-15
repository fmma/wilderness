#ifndef __KEYBOARD_HEADER__
#define __KEYBOARD_HEADER__

#define MAX_EVENTS (16)

#define KEY_BACKSPACE      0x08
#define KEY_TAB            0x09
#define KEY_CLEAR          0x0C
#define KEY_RETURN         0x0D
#define KEY_SHIFT          0x10
#define KEY_CONTROL        0x11
#define KEY_MENU           0x12
#define KEY_PAUSE          0x13
#define KEY_CAPITAL        0x14
#define KEY_ESCAPE         0x1B
#define KEY_CONVERT        0x1C
#define KEY_NONCONVERT     0x1D
#define KEY_ACCEPT         0x1E
#define KEY_MODECHANGE     0x1F
#define KEY_SPACE          0x20
#define KEY_PRIOR          0x21
#define KEY_NEXT           0x22
#define KEY_END            0x23
#define KEY_HOME           0x24
#define KEY_LEFT           0x25
#define KEY_UP             0x26
#define KEY_RIGHT          0x27
#define KEY_DOWN           0x28
#define KEY_SELECT         0x29
#define KEY_PRINT          0x2A
#define KEY_EXECUTE        0x2B
#define KEY_SNAPSHOT       0x2C
#define KEY_INSERT         0x2D
#define KEY_DELETE         0x2E
#define KEY_HELP           0x2F
#define KEY_LWIN           0x5B
#define KEY_RWIN           0x5C
#define KEY_APPS           0x5D
#define KEY_SLEEP          0x5F
#define KEY_NUMPAD0        0x60
#define KEY_NUMPAD1        0x61
#define KEY_NUMPAD2        0x62
#define KEY_NUMPAD3        0x63
#define KEY_NUMPAD4        0x64
#define KEY_NUMPAD5        0x65
#define KEY_NUMPAD6        0x66
#define KEY_NUMPAD7        0x67
#define KEY_NUMPAD8        0x68
#define KEY_NUMPAD9        0x69
#define KEY_MULTIPLY       0x6A
#define KEY_ADD            0x6B
#define KEY_SEPARATOR      0x6C
#define KEY_SUBTRACT       0x6D
#define KEY_DECIMAL        0x6E
#define KEY_DIVIDE         0x6F
#define KEY_F1             0x70
#define KEY_F2             0x71
#define KEY_F3             0x72
#define KEY_F4             0x73
#define KEY_F5             0x74
#define KEY_F6             0x75
#define KEY_F7             0x76
#define KEY_F8             0x77
#define KEY_F9             0x78
#define KEY_F10            0x79
#define KEY_F11            0x7A
#define KEY_F12            0x7B
#define KEY_F13            0x7C
#define KEY_F14            0x7D
#define KEY_F15            0x7E
#define KEY_F16            0x7F
#define KEY_F17            0x80
#define KEY_F18            0x81
#define KEY_F19            0x82
#define KEY_F20            0x83
#define KEY_F21            0x84
#define KEY_F22            0x85
#define KEY_F23            0x86
#define KEY_F24            0x87
#define KEY_NUMLOCK        0x90
#define KEY_SCROLL         0x91
#define KEY_OEM_NEC_EQUAL  0x92   // '=' key on numpad
#define KEY_LSHIFT         0xA0
#define KEY_RSHIFT         0xA1
#define KEY_LCONTROL       0xA2
#define KEY_RCONTROL       0xA3
#define KEY_LMENU          0xA4
#define KEY_RMENU          0xA5
#define KEY_PLUS       0xBB   // '+' any country
#define KEY_COMMA      0xBC   // ',' any country
#define KEY_MINUS      0xBD   // '-' any country
#define KEY_PERIOD     0xBE   // '.' any country

struct KeyboardInternal;

class Keyboard {
    public:
        Keyboard();
        ~Keyboard();
        void pollKeys();
        bool keyClicked(char c);
        bool keyHold(char c);
        int typedKeys(char *buffer, int bufferSize);
    private:
        KeyboardInternal *internal;
};

#endif