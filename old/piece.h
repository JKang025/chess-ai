#ifndef PIECE_H
#define PIECE_H

class Piece{
    public:

        const int none = 0;
        const int pawn = 1;
        const int knight = 2;
        const int bishop = 3;
        const int rook = 4;
        const int queen = 5;
        const int king = 6;

        const int white = 8;
        const int black = 16;

        bool isSlidingPiece(int piece);
        bool isRightColor(int piece, bool isWhite);



};

#endif