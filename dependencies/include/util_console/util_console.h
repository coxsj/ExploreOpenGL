#pragma once

#include <windows.h>
#include <memory>
#include <string>

class CursorUtil
{
private:
    COORD storedCursorPos;

public:
    //http://www.cplusplus.com/articles/4z18T05o/
    static void cursorToZeroZero() { cursorTo(0, 0); }// print_f("\033[0;0H");
    static void cursorTo(const short newV, const short newH);
    static void cursorTo(COORD newCoord);
    static void clear_screen(char fill = ' ');
    static COORD GetConsoleCursorPosition();
    static unsigned int const GetConsoleCursorPositionH() { return GetConsoleCursorPosition().X; }
    static unsigned int const GetConsoleCursorPositionV() { return GetConsoleCursorPosition().Y; }
    void print_At(const short atV, const short atH, std::string& s);
    void print_NChars(const unsigned int& num, const char& c);
    void randomSeed();
    void restoreStoredCursorPos() { cursorTo(storedCursorPos); }
    void storeCursorPos() { storedCursorPos = GetConsoleCursorPosition(); }
};

class ConsoleTextBlock {
private:
    //std::string blankingStr;
    short hLength;
    short vLength;

protected:
    short H;
    short V;

public:
    ConsoleTextBlock(short newV = 0, short newH = 0, short newvLen = 1, short newhLen = 1) : H(newH), V(newV), vLength(newvLen) { setHlen(newhLen); }
    virtual ~ConsoleTextBlock()=default;
    void blankAll();
    void blankLine(int lineNum=0);
    void cursorEnd();
    void cursorHome();
    short getH() { return H; }
    short getHlen() { return hLength; }
    short getLineAfter() { return V + vLength; }
    short getV() { return V; }
    short getVlen() { return vLength; }
    void print_At(const short lineNum, const std::string& s);
    void setH(short newH) { H = newH;}
    void setHlen(short newHlen) { hLength = newHlen; }
    void setV(short newV) { V = newV; }
    void setVlen(short newVlen) { vLength = newVlen; }

protected:
    void cursorTo(const short newV = 0, const short newH = 0);
    void cursorToLine(const short lineNum=0) {  CursorUtil::cursorTo(V + lineNum, H); }
};

class ConsoleBorder : public ConsoleTextBlock {
private:
    static char const defaultHCh = char(196);
    static char const defaultVCh = char(179);
    char borderChar;
    bool borderIsHorizontal;

public:
    ConsoleBorder(const short vStart, const short hStart, const short len, const bool isHorizontal)
        : borderIsHorizontal(isHorizontal), borderChar(defaultVCh), ConsoleTextBlock(vStart, hStart, len, 1) {
        if (borderIsHorizontal) {
            setHlen(len); 
            setVlen(1);
            borderChar = defaultHCh;
        }
    }
    void clear() { draw(' '); }
    void draw() { draw(borderChar); }
    void draw(char c);
    void setBorderChar(char const c) { borderChar = c; }
};
