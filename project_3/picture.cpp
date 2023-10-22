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

void plotHorizontalLine(int r, int c, int distance, char ch, int fgbg);
void plotVerticalLine(int r, int c, int distance, char ch);
void plotRectangle(int r, int c, int height, int width, char ch);

void fgbgChar(int r, int c, char ch, bool background);

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
bool inGrid(int r, int c);

int main()
{
    setSize(2, 12);  // 2 rows, 12 columns
    assert(plotLine(1, 1, 0, HORIZ, 'H', FG));
    assert(plotLine(1, 2, 0, HORIZ, 'i', FG));
    assert(plotLine(1, 3, 0, HORIZ, '!', FG));
    draw();  //  displays  Hi!  in the top row of the grid
    assert(plotLine(1, 3, 0, HORIZ, ' ', FG));
    draw();  //  displays  Hi   in the top row of the grid
    assert(plotLine(1, 1, 10, HORIZ, ' ', BG));
    draw();  //  displays  Hi   in the top row of the grid
    assert( ! plotLine(1, 1, 10, HORIZ, '\n', FG));
    draw();  //  displays  Hi   in the top row of the grid
}

void plotHorizontalLine(int r, int c, int distance, char ch, int fgbg) {
    bool foreground = fgbg == FG;
    if (distance == 0) {
        fgbgChar(r, c, ch, foreground);
    }
    else if (distance > 0) {
        for (int i = 0; i < distance + 1; i ++) {
            if (c + i > 0 && c + i <= getCols()) {
                fgbgChar(r, c + i, ch, foreground);
            }
        }
    }
    else {
        for (int i = c; i >= c + distance; i --) {
            if (i > 0 && i <= getCols()) {
                fgbgChar(r, i, ch, foreground);
            }
        }
    }
}

void plotVerticalLine(int r, int c, int distance, char ch, int fgbg) {
    bool foreground = fgbg == FG;
    if (distance == 0) {
        fgbgChar(r, c, ch, foreground);
    }
    else if (distance > 0) {
        for (int i = 0; i < distance + 1; i ++) {
            if (r + i > 0 && r + i <= getRows()) {
                fgbgChar(r + i, c, ch, foreground);
            }
        }
    }
    else {
        for (int i = r; i >= r + distance; i --) {
            if (i > 0 && i <= getRows()) {
                fgbgChar(i, c, ch, foreground);
            }
        }
    }
}
//
//void plotRectangle(int r, int c, int height, int width, char ch) {
//    plotVerticalLine(r, c, height, ch);
//    plotVerticalLine(r, c + width, height, ch);
//    plotHorizontalLine(r, c, width, ch);
//    plotHorizontalLine(r + height, c, width, ch);
//}

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
