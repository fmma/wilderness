#ifndef __VIEW_HEADER__
#define __VIEW_HEADER__

struct ViewInternal;

class View {
    public:
        int width;
        int height;
        View(int width, int height);
        ~View();
        void clearScreenBuffer();
        void render();
        void clear();
        void frameScreenBuffer();
        void renderChar(int x, int y, char c);
        int renderString(int x, int y, const char *s);
        int writeString(int x, int y, const char *s);
        inline void writeChar(int x, int y, char c) {
            int i = (x + y * width);
            screenBuffer[i] = c;
        }
    private:
        char *screenBuffer;
        ViewInternal *internal;
};

#endif