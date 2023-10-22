//
//  main.cpp
//  project_3
//
//  Created by Leison Gao on 10/22/23.
//

#include <iostream>
#include "grid.h"

void plotHorizontalLine(int r, int c, int distance, char ch);
void plotVerticalLine(int r, int c, int distance, char ch);
void plotRectangle(int r, int c, int height, int width, char ch);

int main()
{
    setSize(20, 30);
    plotRectangle(1, 1, 3, 3, '*');
    draw();
}

void plotHorizontalLine(int r, int c, int distance, char ch) {
    if (distance == 0) {
        setChar(r, c, ch);
    }
    for (int i = 0; i < distance + 1; i ++) {
        if (c + i > 0 && c + i <= getCols()) {
            setChar(r, c + i, ch);
        }
    }
}

void plotVerticalLine(int r, int c, int distance, char ch) {
    if (distance == 0) {
        setChar(r, c, ch);
    }
    for (int i = 0; i < distance + 1; i ++) {
        if (r + i > 0 && r + i <= getRows()) {
            setChar(r + i, c, ch);
        }
    }
}

void plotRectangle(int r, int c, int height, int width, char ch) {
    plotVerticalLine(r, c, height, ch);
    plotVerticalLine(r, c + width, height, ch);
    plotHorizontalLine(r, c, width, ch);
    plotHorizontalLine(r + height, c, width, ch);
}
