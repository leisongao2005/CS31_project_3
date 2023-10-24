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

//int ground = 0;
//char pChar = '*';
//int row = 1;
//int col = 1;

bool plotHorizontalLine(int r, int c, int distance, char ch, int fgbg);
bool plotVerticalLine(int r, int c, int distance, char ch, int fgbg);
//void plotRectangle(int r, int c, int height, int width, char ch);

void fgbgChar(int r, int c, char ch, bool background);

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
bool inGrid(int r, int c);

int performCommands(string commandString, char& plotChar, int& mode, int& badPos);

int translateInstructionString(string text);

bool processLine(string text, int& pos, int& row, int& col, int plotChar, int mode);
bool processFGBG(string text, int& pos, char& plotChar, int& mode);
bool processClear(string text, int& pos, int& row, int& col, int& mode, char& plotChar);

//int main()
//{
//    for (;;)
//    {
//        cout << "Enter the number of grid rows and columns (max 30 each): ";
//        int nRows;
//        int nCols;
//        cin >> nRows >> nCols;
//        cin.ignore(10000, '\n');
//        if (nRows >= 1  &&  nRows <= MAXROWS  &&  nCols >= 1  &&  nCols <= MAXCOLS)
//        {
//            setSize(nRows, nCols);
//            break;
//        }
//        cout << "The numbers must be between 1 and 30." << endl;
//    }
//    char currentChar = '*';
//    int currentMode = FG;
//    for (;;)
//    {
//        cout << "Enter a command string (empty line to quit): ";
//        string cmd;
//        getline(cin, cmd);
//        if (cmd == "")
//            break;
//        int position;
//        int status = performCommands(cmd, currentChar, currentMode, position);
//        switch (status)
//        {
//          case 0:
//            draw();
//            break;
//          case 1:
//            cout << "Syntax error at position " << position << endl;
//            break;
//          case 2:
//            if (!isprint(currentChar))
//                cout << "Current charaacter is not printable" << endl;
//            if (currentMode != FG  &&  currentMode != BG)
//                cout << "Current mode is " << currentMode << ", not FG or BG" << endl;
//            break;
//          case 3:
//            cout << "Cannot perform command at position " << position << endl;
//            break;
//          default:
//              // It should be impossible to get here.
//            cout << "performCommands returned " << status << "!" << endl;
//        }
//    }
//}

int main()
{
    setSize(12, 15);
    assert(plotLine(3, 5, 2, HORIZ, '@', FG));
    for (int c = 5; c <= 7; c++)
        assert(getChar(3, c) == '@');
    assert(getChar(3, 8) == ' ');
    clearGrid();
    char pc = '%';
    int m = FG;
    int bad = 999;
    //A successful command string should not change bad
    assert(performCommands("V2", pc, m, bad) == 0  &&  getChar(3, 1) == '%'  &&  bad == 999);
    assert(performCommands("V2H2Q2", pc, m, bad) == 1  &&  bad == 4);
    assert(performCommands("H4V3V-1H-9", pc, m, bad) == 3  &&  bad == 7);
    cout << "All tests succeeded." << endl;
}

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg) {
    bool dirCorr = (dir == HORIZ || dir == VERT);
    bool fgbgCorr = (fgbg == FG || fgbg == BG);
    bool charCorr = isprint(plotChar);
    
    if (dirCorr && fgbgCorr && charCorr && inGrid(r, c)) {
        if (dir == HORIZ) {
            if (inGrid(r, c + distance)) {
                plotHorizontalLine(r, c, distance, plotChar, fgbg);
            }
            else {
                return false;
            }
        }
        else if (dir == VERT) {
            if (inGrid(r + distance, c)) {
                plotVerticalLine(r, c, distance, plotChar, fgbg);
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

int performCommands(string commandString, char& plotChar, int& mode, int& badPos) {
    if (!isprint(plotChar) || (mode != FG && mode != BG)) {
        return 2;
    }
    int row = 1;
    int col = 1;
    int pos = 0;
    string text = commandString;
    while (pos < text.size()) {
        if (char(tolower(text[pos])) == 'h' || char(tolower(text[pos])) == 'v') {
            if (!processLine(text, pos, row, col, plotChar, mode)) {
                badPos = pos;
                return 1;
            }
        }
        else if (char(tolower(text[pos])) == 'f' || char(tolower(text[pos])) == 'b') {
            if (!processFGBG(text, pos, plotChar, mode)) {
                badPos = pos;
                return 1;
            }
        }
        else if (char(tolower(text[pos])) == 'c') {
            if (!processClear(text, pos, row, col, mode, plotChar)) {
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

bool processLine(string text, int& pos, int& row, int& col, int plotChar, int mode) {
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
        
        if (!plotLine(row, col, dist, dir, plotChar, mode)) {
            return false;
        }
        else {
            if (dir == HORIZ) {
                col += dist;
            }
            else if (dir == VERT){
                row += dist;
            }
            return true;
        }
    }
    else {
        return false;
    }
}

bool processFGBG(string text, int& pos, char& plotChar, int& mode) {
    if (tolower(text[pos]) == 'f') {
        mode = FG;
        pos ++;
    }
    else if (tolower(text[pos]) == 'b') {
        mode = BG;
        pos ++;
    }
    else {
        return false;
    }
    
    if (isprint(text[pos])) {
        plotChar = text[pos];
        pos ++;
        return true;
    }
    else {
        return false;
    }
}

bool processClear(string text, int& pos, int& row, int& col, int& mode, char& plotChar) {
    clearGrid();
    row = 1;
    col = 1;
    mode = FG;
    plotChar = '*';
    pos ++;
    return true;
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
