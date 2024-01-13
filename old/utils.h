#ifndef UTILS_H
#define UTILS_H

#include "libconstants.h"
#include <iostream>

using namespace std;
using namespace myConstants;

namespace myUtils{
    void printBitboard(U64 bitboard);
    U64 getBit(U64 bitboard, boardSquare square);
    void setBit(U64& bitboard, boardSquare square);
    void removeBit(U64& bitboard,boardSquare square);
    int countBits(U64 bitboard);
    int firstLeastSignificantBitIndex(U64 bitboard);
    U64 setOccupancy(int num, int bitsInMask, U64 attackMask);
    
    
}


#endif