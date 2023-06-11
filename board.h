#ifndef BOARD_H
#define BOARD_H

#include<string>
#include<vector>
#include "piece.h"

using namespace std;
typedef unsigned long long U64;



class Board{

    public:
        enum boardSquare{
        a8, b8, c8, d8, e8, f8, g8, h8,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a5, b5, c5, d5, e5, f5, g5, h5, 
        a4, b4, c4, d4, e4, f4, g4, h4,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a1, b1, c1, d1, e1, f1, g1, h1,
        };

        Board(string fen); 
        Board();
        void printBitboard(U64 bitboard);
        void generateMoves();
        void executeMove();

        U64 getBit(U64 bitboard, boardSquare square);
        void setBit(U64& bitboard, boardSquare square);
        void removeBit(U64& bitboard, boardSquare square);
    
    private:
        struct Move{
            int startSqaure;
            int endSqaure;
            Move(int start, int end){
                startSqaure = start;
                endSqaure = end; 
            }
        };

       
        int charToPiece(char c);
        char pieceToChar(int i);

        

        vector<int> theBoard;
        vector<Move> legalMoves;
        Piece _piece;
        
        bool whiteTurn;
        bool whiteCastleKing;
        bool whiteCastleQueen;
        bool blackCastleKing;
        bool blackCastleQueen;
        int move;
        int halfMove;
        string enpassant;
        
        
};

#endif