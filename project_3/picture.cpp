//
//  main.cpp
//  project_3
//
//  Created by Leison Gao on 10/22/23.
//

#include <iostream>
#include "grid.h"
#include <cassert>
using namespace std;

const int HORIZ = 0;
const int VERT = 1;

const int FG = 0;
const int BG = 1;

int ground = 0;
char pChar = '*';
int row = 1;
int col = 1;

bool plotHorizontalLine(int r, int c, int distance, char ch, int fgbg);
bool plotVerticalLine(int r, int c, int distance, char ch);
//void plotRectangle(int r, int c, int height, int width, char ch);

void fgbgChar(int r, int c, char ch, bool background);

bool plotLine(int& r, int& c, int distance, int dir, char plotChar, int fgbg);
bool inGrid(int r, int c);

int performCommands(string commandString, char& plotChar, int& mode, int& badPos);

int translateInstructionString(string text);

bool processLine(string text, int& pos);
bool processFGBG(string text, int& pos);
bool processClear(string text, int& pos);

int main()
{
    setSize(8, 20);
    int p = -1;
    performCommands("FH8", pChar, ground, p);
    cout << p << endl;
    performCommands("v2b h12fHh1fih0", pChar, ground, p);
    cout << p << endl;
    performCommands("CV6", pChar, ground, p);
    cout << p << endl;
    draw();
    
}

bool plotHorizontalLine(int r, int c, int distance, char ch, int fgbg) {
    bool foreground = fgbg == FG;
    if (distance == 0) {
        fgbgChar(r, c, ch, foreground);
        return true;
    }
    else if (distance > 0) {
        for (int i = 0; i < distance + 1; i ++) {
            if (c + i > 0 && c + i <= getCols()) {
                fgbgChar(r, c + i, ch, foreground);
            }
            else {
                return false;
            }
        }
        return true;
    }
    else {
        for (int i = c; i >= c + distance; i --) {
            if (i > 0 && i <= getCols()) {
                fgbgChar(r, i, ch, foreground);
            }
            else {
                return false;
            }
        }
        return true;
    }
}

bool plotVerticalLine(int r, int c, int distance, char ch, int fgbg) {
    bool foreground = fgbg == FG;
    if (distance == 0) {
        fgbgChar(r, c, ch, foreground);
        return true;
    }
    else if (distance > 0) {
        for (int i = 0; i < distance + 1; i ++) {
            if (r + i > 0 && r + i <= getRows()) {
                fgbgChar(r + i, c, ch, foreground);
            }
            else {
                return false;
            }
        }
        return true;
    }
    else {
        for (int i = r; i >= r + distance; i --) {
            if (i > 0 && i <= getRows()) {
                fgbgChar(i, c, ch, foreground);
            }
            else {
                return false;
            }
        }
        return true;
    }
}

bool plotLine(int& r, int& c, int distance, int dir, char plotChar, int fgbg) {
    bool dirCorr = (dir == HORIZ || dir == VERT);
    bool fgbgCorr = (fgbg == FG || fgbg == BG);
    bool charCorr = isprint(plotChar);
    
    if (dirCorr && fgbgCorr && charCorr && inGrid(r, c)) {
        if (dir == HORIZ) {
            if (inGrid(r, c + distance)) {
                plotHorizontalLine(r, c, distance, plotChar, fgbg);
                c += distance;
            }
            else {
                return false;
            }
        }
        else if (dir == VERT) {
            if (inGrid(r + distance, c)) {
                plotVerticalLine(r, c, distance, plotChar, fgbg);
                r += distance;
            }
            else {
                return false;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

void fgbgChar(int r, int c, char ch, bool foreground) {
    if (foreground) {
        setChar(r, c, ch);
    }
    else {
        if (getChar(r, c) == ' ') {
            setChar(r, c, ch);
        }
    }
}

bool inGrid(int r, int c) {
    return (0 < r && r <= getRows() && 0 < c && c <= getCols());
}

int performCommands(string commandString, char& plotChar, int& mode, int& badPos) {
    if (!isprint(plotChar) || (mode != FG && mode != BG)) {
        return 2;
    }
//    int r = 1;
//    int c = 1;
//    int dist;
//    int fgbg;
//    int dir;
//    for (int i = 0; i < commandString.size(); i ++) {
//        char commandChar = tolower(commandString[i]);
//        if (commandChar == 'h') {
//            if (commandString[i] == '-') {
//                cout << "hoiasjdofij";
//            }
//        }
//    }
//    return 0;
    row = 1;
    col = 1;
    pChar = plotChar;
    ground = mode;
    int pos = 0;
    string text = commandString;
    while (pos < text.size()) {
        if (char(tolower(text[pos])) == 'h' || char(tolower(text[pos])) == 'v') {
            if (!processLine(text, pos)) {
                badPos = pos;
                return 1;
            }
        }
        else if (char(tolower(text[pos])) == 'f' || char(tolower(text[pos])) == 'b') {
            if (!processFGBG(text, pos)) {
                badPos = pos;
                return 1;
            }
        }
        else if (char(tolower(text[pos])) == 'c') {
            if (!processClear(text, pos)) {
                badPos = pos;
                return 1;
            }
        }
        else {
            badPos = pos;
            return 1;
        }
    }
    return 0;
}
// must decide when to increment pos, increments it at the end of each function
int translateInstructionString(string text) {
    int pos = 0;
    while (pos < text.size()) {
        if (char(tolower(text[pos])) == 'h' || char(tolower(text[pos])) == 'v') {
            if (!processLine(text, pos)) {
                return false;
            }
        }
        else if (char(tolower(text[pos])) == 'f' || char(tolower(text[pos])) == 'b') {
            if (!processFGBG(text, pos)) {
                return false;
            }
        }
        else if (char(tolower(text[pos])) == 'c') {
            if (!processClear(text, pos)) {
                return false;
            }
        }
        else {
            return false;
        }
    }
    return true;
}

bool processLine(string text, int& pos) {
    int dir;
    if (tolower(text[pos]) == 'h') {
        dir = HORIZ;
    }
    else {
        dir = VERT;
    }
    
    pos ++;
    
    bool neg = false;
    int dist = 0;
    int digits = 0;
    if (pos == text.size()) {
        return false;
    }
    // double negative sign --> error
    if (text[pos] == '-') {
        if (neg == true) // double negative sign --> error
            return false;
        else {
            neg = true;
            pos ++;
        }
            
    }
    if (isnumber(text[pos])) {
        for (; digits < 2 && pos != text.size() && isnumber(text[pos]); pos ++) {
            dist = dist * 10 + int(text[pos] - '0');
            digits ++;
        }
        if (neg)
            dist = -dist;
        
        if (!plotLine(row, col, dist, dir, pChar, ground)) {
            return false;
        }
        else {
            return true;
        }
    }
    else {
        return false;
    }
}

bool processFGBG(string text, int& pos) {
    if (tolower(text[pos]) == 'f') {
        ground = FG;
        pos ++;
    }
    else if (tolower(text[pos]) == 'b') {
        ground = BG;
        pos ++;
    }
    else {
        return false;
    }
    
    if (isprint(text[pos])) {
        pChar = text[pos];
        pos ++;
        return true;
    }
    else {
        return false;
    }
}

bool processClear(string text, int& pos) {
    clearGrid();
    row = 1;
    col = 1;
    ground = FG;
    pChar = '*';
    pos ++;
    return true;
}
