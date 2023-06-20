#ifndef UTILS
#define UTILS

#include "libconstants.h"
#include <iostream>

using namespace std;
using namespace libConst;

namespace Utils{
    void printBitboard(U64 bitboard);
    U64 getBit(U64 bitboard, boardSquare square);
    void setBit(U64& bitboard, boardSquare square);
    void removeBit(U64& bitboard,boardSquare square);
    int countBits(U64 bitboard);
    int firstLeastSignificantBitIndex(U64 bitboard);
    U64 setOccupancy(int num, int bitsInMask, U64 attackMask);
}


#endif