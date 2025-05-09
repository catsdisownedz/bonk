#include "physics/Swing.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

// length ->  x, width -> y
// idk why but left is right and right is left
// Swing::Swing(pair<double, double> top, pair<double, double> bottom, double boardLength, double boardWidth){
//     ropeStart = top;
//     ropeEnd = bottom;
//     leftCenter.first = bottom.first - 0.5 * boardLength;
//     leftCenter.second = bottom.second + 0.5 * boardWidth;
//     rightCenter.first = bottom.first + 0.5 * boardLength;
//     rightCenter.second = bottom.second + 0.5 * boardWidth;
//     this -> boardLength = boardLength;
//     this -> boardWidth = boardWidth;
//     angularVelocity = 0;
//     this->ropeLength = sqrt(pow(top.first - bottom.first, 2) + pow(top.second - bottom.second, 2));
//     this->angle = 3.14 * 1.5;  // 270 degrees in radians

//     assignLeftPlatBounds(0, {leftCenter.first - 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth});
//     assignLeftPlatBounds(1,{leftCenter.first + 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth});
//     assignLeftPlatBounds(2, {leftCenter.first + 0.5 * boardLength + leanAmount,leftCenter.second + 0.5 * boardWidth });
//     assignLeftPlatBounds(3, {leftCenter.first - 0.5 * boardLength + leanAmount, leftCenter.second + 0.5 * boardWidth});

//     assignRightPlatBounds(0,{rightCenter.first - 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth});
//     assignRightPlatBounds(1, {rightCenter.first + 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth});
//     assignRightPlatBounds(2, {rightCenter.first + 0.5 * boardLength - leanAmount, rightCenter.second + 0.5 * boardWidth});
//     assignRightPlatBounds(3, {rightCenter.first - 0.5 * boardLength - leanAmount, rightCenter.second + 0.5 * boardWidth});

// }


// void Swing::draw() {
//     //double diagonal = calculateDiagonals();
//     //cout<< diagonal<<"\n";
//     glColor3f(0.7f, 0.7f, 0.7f);
//     glBegin(GL_LINES);
//         glVertex2f(ropeStart.first, ropeStart.second);
//         glVertex2f(ropeEnd.first, ropeEnd.second);
//     glEnd();
//     // BL BR TR TL
//     glBegin(GL_QUADS);
//         glVertex2f(leftCenter.first - 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth);
//         assignLeftPlatBounds(0 , {leftCenter.first - 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth});
//         glVertex2f(leftCenter.first + 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth);
//         assignLeftPlatBounds(1,{leftCenter.first + 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth});
//         glVertex2f(leftCenter.first + 0.5 * boardLength + leanAmount, leftCenter.second + 0.5 * boardWidth);
//         assignLeftPlatBounds(2, {leftCenter.first + 0.5 * boardLength + leanAmount,leftCenter.second + 0.5 * boardWidth });
//         glVertex2f(leftCenter.first - 0.5 * boardLength + leanAmount, leftCenter.second + 0.5 * boardWidth);
//         assignLeftPlatBounds(3, {leftCenter.first - 0.5 * boardLength + leanAmount, leftCenter.second + 0.5 * boardWidth});
//     glEnd();
//     glBegin(GL_QUADS);
//         glVertex2f(rightCenter.first - 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth);
//         assignRightPlatBounds(0,{rightCenter.first - 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth});
//         glVertex2f(rightCenter.first + 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth);
//         assignRightPlatBounds(1, {rightCenter.first + 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth});
//         glVertex2f(rightCenter.first + 0.5 * boardLength - leanAmount, rightCenter.second + 0.5 * boardWidth);
//         assignRightPlatBounds(2, {rightCenter.first + 0.5 * boardLength - leanAmount, rightCenter.second + 0.5 * boardWidth});
//         glVertex2f(rightCenter.first - 0.5 * boardLength - leanAmount, rightCenter.second + 0.5 * boardWidth);
//         assignRightPlatBounds(3, {rightCenter.first - 0.5 * boardLength - leanAmount, rightCenter.second + 0.5 * boardWidth});
//     glEnd();
// }

Swing::Swing(pair<double, double> top, pair<double, double> bottom, double boardLength, double boardWidth){
    ropeStart = top;
    ropeEnd = bottom;

    // left is to the left, right is to the right
    leftCenter.first  = bottom.first - 0.5 * boardLength;
    leftCenter.second = bottom.second + 0.5 * boardWidth;
    rightCenter.first = bottom.first + 0.5 * boardLength;
    rightCenter.second = bottom.second + 0.5 * boardWidth;

    this->boardLength = boardLength;
    this->boardWidth = boardWidth;
    angularVelocity = 0;
    this->ropeLength = sqrt(pow(top.first - bottom.first, 2) + pow(top.second - bottom.second, 2));
    this->angle = 3.14 * 1.5;  // 270 degrees

    // LEFT swing leans LEFT ⇒ subtract leanAmount
    assignLeftPlatBounds(0, {leftCenter.first - 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth});
    assignLeftPlatBounds(1, {leftCenter.first + 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth});
    assignLeftPlatBounds(2, {leftCenter.first + 0.5 * boardLength - leanAmount, leftCenter.second + 0.5 * boardWidth});
    assignLeftPlatBounds(3, {leftCenter.first - 0.5 * boardLength - leanAmount, leftCenter.second + 0.5 * boardWidth});

    // RIGHT swing leans RIGHT ⇒ add leanAmount
    assignRightPlatBounds(0, {rightCenter.first - 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth});
    assignRightPlatBounds(1, {rightCenter.first + 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth});
    assignRightPlatBounds(2, {rightCenter.first + 0.5 * boardLength + leanAmount, rightCenter.second + 0.5 * boardWidth});
    assignRightPlatBounds(3, {rightCenter.first - 0.5 * boardLength + leanAmount, rightCenter.second + 0.5 * boardWidth});
}
void Swing::draw() {
    glColor3f(0.7f, 0.7f, 0.7f);

    glBegin(GL_LINES);
        glVertex2f(ropeStart.first, ropeStart.second);
        glVertex2f(ropeEnd.first, ropeEnd.second);
    glEnd();

    // === LEFT SWING ===
    glBegin(GL_QUADS);
        glVertex2f(leftCenter.first - 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth);
        assignLeftPlatBounds(0, {leftCenter.first - 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth});

        glVertex2f(leftCenter.first + 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth);
        assignLeftPlatBounds(1, {leftCenter.first + 0.5 * boardLength, leftCenter.second - 0.5 * boardWidth});

        glVertex2f(leftCenter.first + 0.5 * boardLength - leanAmount, leftCenter.second + 0.5 * boardWidth);
        assignLeftPlatBounds(2, {leftCenter.first + 0.5 * boardLength - leanAmount, leftCenter.second + 0.5 * boardWidth});

        glVertex2f(leftCenter.first - 0.5 * boardLength - leanAmount, leftCenter.second + 0.5 * boardWidth);
        assignLeftPlatBounds(3, {leftCenter.first - 0.5 * boardLength - leanAmount, leftCenter.second + 0.5 * boardWidth});
    glEnd();

    // === RIGHT SWING ===
    glBegin(GL_QUADS);
        glVertex2f(rightCenter.first - 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth);
        assignRightPlatBounds(0, {rightCenter.first - 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth});

        glVertex2f(rightCenter.first + 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth);
        assignRightPlatBounds(1, {rightCenter.first + 0.5 * boardLength, rightCenter.second - 0.5 * boardWidth});

        glVertex2f(rightCenter.first + 0.5 * boardLength + leanAmount, rightCenter.second + 0.5 * boardWidth);
        assignRightPlatBounds(2, {rightCenter.first + 0.5 * boardLength + leanAmount, rightCenter.second + 0.5 * boardWidth});

        glVertex2f(rightCenter.first - 0.5 * boardLength + leanAmount, rightCenter.second + 0.5 * boardWidth);
        assignRightPlatBounds(3, {rightCenter.first - 0.5 * boardLength + leanAmount, rightCenter.second + 0.5 * boardWidth});
    glEnd();
}
double Swing::calculateDiagonals(){
    double diagonal = sqrt(boardLength * boardLength + boardWidth * boardWidth);
    return diagonal;
}

void Swing::assignLeftPlatBounds(int position, pair<double, double> vertex){
    if(leftPlatBounds.size() > position){
        leftPlatBounds[position].first = vertex.first;
        leftPlatBounds[position].second = vertex.second;
    }
    else{
        leftPlatBounds.push_back(vertex);
    }
}

void Swing::assignRightPlatBounds(int position, pair<double, double> vertex){
    if(rightPlatBounds.size() > position){
        rightPlatBounds[position].first = vertex.first;
        rightPlatBounds[position].second = vertex.second;
    }
    else{
        rightPlatBounds.push_back(vertex);
    }
}

pair<double, double>Swing::getPivot(){
    return ropeEnd;
}

// 🔥 NEW FUNCTION waaaaaaaaaaw
pair<PlatformBounds, PlatformBounds> Swing:: getSwingBounds() const {
        // Left platform bounds
        double leftLeft = leftCenter.first - 0.5 * boardLength;
        double leftRight = leftCenter.first + 0.5 * boardLength + 0.1; // slight lean
        double leftTop = leftCenter.second + 0.5 * boardWidth;
        double leftBottom = leftCenter.second - 0.5 * boardWidth;
        
        PlatformBounds leftBounds = { leftLeft, leftRight, leftTop, leftBottom };
        
        // Right platform bounds
        double rightLeft = rightCenter.first - 0.5 * boardLength;
        double rightRight = rightCenter.first + 0.5 * boardLength - 0.1; // slight lean
        double rightTop = rightCenter.second + 0.5 * boardWidth;
        double rightBottom = rightCenter.second - 0.5 * boardWidth;

        PlatformBounds rightBounds = { rightLeft, rightRight, rightTop, rightBottom };

        return { leftBounds, rightBounds };
    }






