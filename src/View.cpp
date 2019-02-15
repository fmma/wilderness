#include "View.h"

#include "windows.h"
#include "stdio.h"

struct ViewInternal {
    HANDLE hStdout;

    ViewInternal() {
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        hideCursor();
        cls(hStdout);
    }

    ~ViewInternal() {
        cls(hStdout);
        showCursor();
    }

    void hideCursor() {
        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo(hStdout, &info);
        info.bVisible = FALSE;
        SetConsoleCursorInfo(hStdout, &info);
    }

    void showCursor() {
        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo(hStdout, &info);
        info.bVisible = TRUE;
        SetConsoleCursorInfo(hStdout, &info);
    }
    
    void cls( HANDLE hConsole ) {
        COORD coordScreen = { 0, 0 }; // home for the cursor 
        DWORD cCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi; 
        DWORD dwConSize;

        // Get the number of character cells in the current buffer. 

        if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
        {
            return;
        }

        dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

        // Fill the entire screen with blanks.

        if( !FillConsoleOutputCharacter( hConsole,        // Handle to console screen buffer 
                                            (TCHAR) ' ',     // Character to write to the buffer
                                            dwConSize,       // Number of cells to write 
                                            coordScreen,     // Coordinates of first cell 
                                            &cCharsWritten ))// Receive number of characters written
        {
            return;
        }

        // Get the current text attribute.

        if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
        {
            return;
        }

        // Set the buffer's attributes accordingly.

        if( !FillConsoleOutputAttribute( hConsole,         // Handle to console screen buffer 
                                            csbi.wAttributes, // Character attributes to use
                                            dwConSize,        // Number of cells to set attribute 
                                            coordScreen,      // Coordinates of first cell 
                                            &cCharsWritten )) // Receive number of characters written
        {
            return;
        }

        // Put the cursor at its home coordinates.

        SetConsoleCursorPosition( hConsole, coordScreen );
    }

    void setCursor(int x, int y) {
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
        if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
        {
            MessageBox(NULL, TEXT("GetConsoleScreenBufferInfo"),
                TEXT("Console Error"), MB_OK);
            return;
        }
        
        csbiInfo.dwCursorPosition.X = x;
        csbiInfo.dwCursorPosition.Y = y;

        if (! SetConsoleCursorPosition(hStdout, csbiInfo.dwCursorPosition)) 
        {
            MessageBox(NULL, TEXT("SetConsoleCursorPosition"),
                TEXT("Console Error"), MB_OK);
            return;
        }
    }
};

View::View(int width, int height) : width(width), height(height), screenBuffer(new char[width * height]), internal(new ViewInternal) {
}

View::~View() {
    delete screenBuffer;
    delete internal;
}

void View::clearScreenBuffer() {
    for(int x = 0; x < width-1; ++x) {
        for(int y = 0; y < height; ++y) {
            writeChar(x, y, ' ');
        }
    }
    for(int y = 0; y < height; ++y) {
        writeChar(width-1, y, '\n');
    }
}

void View::frameScreenBuffer() {
    writeChar(0, 0, '+');
    writeChar(width - 2, 0,'+');
    writeChar(0, height-1, '+');
    writeChar(width - 2, height-1, '+');
    for(int i = 1; i < width - 2; ++i) {
        writeChar(i, 0, '-');
        writeChar(i, height - 1, '-');
    }
    for(int i = 1; i < height-1; ++i) {
        writeChar(0, i, '|');
        writeChar(width - 2, i, '|');
    }
}

void View::clear() {
    this->clearScreenBuffer();
    this->frameScreenBuffer();
    this->render();
}

void View::renderChar(int x, int y, char c) {
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    this->renderString(x, y, buf);
}

int View::renderString(int x, int y, const char *s) {
    if(x == -1) {
        x = (width - strlen(s)) / 2;
    }
    internal->setCursor(x, y);
    printf("%s", s);
    fflush(stdout);
    return x;
}

int View::writeString(int x, int y, const char *s) {
    if(x == -1) {
        x = (width - strlen(s)) / 2;
    }
    for(int i = 0; i + x < width && s[i] != 0; ++i) {
        writeChar(x + i, y, s[i]);
    }
    return x;
}

void View::render() {
    this->renderString(0, 0, screenBuffer);
}
