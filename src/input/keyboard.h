//
// Created by jhone on 29/03/2026.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H



#pragma once

namespace Key {

    constexpr int A = 'A';
    constexpr int B = 'B';
    constexpr int C = 'C';
    constexpr int D = 'D';
    constexpr int E = 'E';
    constexpr int F = 'F';
    constexpr int G = 'G';
    constexpr int H = 'H';
    constexpr int I = 'I';
    constexpr int J = 'J';
    constexpr int K = 'K';
    constexpr int L = 'L';
    constexpr int M = 'M';
    constexpr int N = 'N';
    constexpr int O = 'O';
    constexpr int P = 'P';
    constexpr int Q = 'Q';
    constexpr int R = 'R';
    constexpr int S = 'S';
    constexpr int T = 'T';
    constexpr int U = 'U';
    constexpr int V = 'V';
    constexpr int W = 'W';
    constexpr int X = 'X';
    constexpr int Y = 'Y';
    constexpr int Z = 'Z';


    constexpr int NUM_0 = '0';
    constexpr int NUM_1 = '1';
    constexpr int NUM_2 = '2';
    constexpr int NUM_3 = '3';
    constexpr int NUM_4 = '4';
    constexpr int NUM_5 = '5';
    constexpr int NUM_6 = '6';
    constexpr int NUM_7 = '7';
    constexpr int NUM_8 = '8';
    constexpr int NUM_9 = '9';


    constexpr int UP    = 38;
    constexpr int DOWN  = 40;
    constexpr int LEFT  = 37;
    constexpr int RIGHT = 39;


    constexpr int SPACE     = 32;
    constexpr int ENTER     = 13;
    constexpr int ESCAPE    = 27;
    constexpr int SHIFT     = 16;
    constexpr int CTRL      = 17;
    constexpr int ALT       = 18;
    constexpr int TAB       = 9;
    constexpr int BACKSPACE = 8;
    constexpr int DELETE    = 46;


    inline bool isLetter(int key) {
        return (key >= 'A' && key <= 'Z');
    }

    inline bool isNumber(int key) {
        return (key >= '0' && key <= '9');
    }
}




#endif //KEYBOARD_H
