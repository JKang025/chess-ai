#include <iostream>
#include "board.h"

/*
"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", 
"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", 
"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", 
"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", 
"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", 
"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", 
"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", 
"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", 
*/
int main(int argc, char const *argv[]){
    /*
    Board b = Board();
    U64 bitboard = 0ULL;
    cout << (1ULL << 8) << endl;
    b.setBit(bitboard, Board::e4);
    cout << b.getBit(bitboard, Board::e4) << endl;
    cout << b.getBit(bitboard, Board::e5) << endl;
    b.printBitboard(bitboard);
    */
   Board bruh = Board();
   //bruh.printBitboard(bruh.maskPawnAttacks(Board::a4, Board::black));
   //bruh.initializeLeaperPieces();
   //for(int i = 0; i < 64; i++){
    //bruh.printBitboard(bruh.generateRookAttacks(static_cast<Board::boardSquare>(i)));
  // }
  //bruh.printBitboard(bruh.generateRealBishopAttacks(Board::d4, 0ULL));
   //bruh.printBitboard(bruh.generateBishopAttacks(Board::e4));
   U64 block = 0ULL;
    bruh.setBit(block, Board::d3);
    bruh.setBit(block, Board::d7);
    bruh.setBit(block, Board::g4);
    //bruh.setBit(block, Board::e2);
    bruh.printBitboard(bruh.generateRealRookAttacks(Board::d4, block));

    
};