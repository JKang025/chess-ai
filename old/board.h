#ifndef BOARD_H
#define BOARD_H

#include<string>
#include "piece.h"
#include "libconstants.h"
#include "utils.h"

using namespace std;
using namespace myConstants;
using namespace myUtils;



class Board{

    public:

        Board(string fen); 
        Board();
        //void generateMoves();
        //void executeMove();

        U64 generatePawnAttacks(boardSquare square, color side);
        U64 pawnAttacks[2][64];

        U64 generateKnightAttacks(boardSquare sqaure);
        U64 knightAttacks[64];

        U64 generateKingAttacks(boardSquare square);
        U64 kingAttacks[64];

        void initializeLeaperPieces();

        U64 bishopMasks[64];
        U64 rookMasks[64];

        // [square][occupancy]
        U64 bishopAttacks[64][512];

        // [square][occpuancy]
        U64 rookAttacks[64][4096];


        U64 generateBishopAttacks(boardSquare square);
        //U64 bishopAttacks[64];
        U64 generateRealBishopAttacks(boardSquare square, U64 block);

        U64 generateRookAttacks(boardSquare sqaure);
        //64 rookAttacks[64];
        U64 generateRealRookAttacks(boardSquare square, U64 block);
        

        U64 findMagicNumber(boardSquare square, int relevantBits, int bishop);
        void initializeMagicNumber();

        void initializeAll();
    
    private:
        struct Move{
            int startSqaure;
            int endSqaure;
            Move(int start, int end){
                startSqaure = start;
                endSqaure = end; 
            }
        };

       // PROLLY not used
       // --------------------------

        
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