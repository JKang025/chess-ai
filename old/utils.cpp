#include "utils.h"

void myUtils::printBitboard(U64 bitboard){
    cout << endl;
    for(int rank = 0; rank < BOARD_HEIGHT; rank++){
        for(int file = 0; file < BOARD_WIDTH; file++){

            if(file == 0){
                cout << 8 - rank << " ";
            }
            int square = rank * 8 + file;
            cout << (getBit(bitboard, static_cast<boardSquare>(square)) ? 1 : 0) << " "; 
        }
        cout << "\n";
    }
    cout << "  a b c d e f g h\n \n";
    cout << bitboard << "\n";

}

// BIT MANIPULATION
// =========================
// =========================

U64 myUtils::getBit(U64 bitboard, boardSquare square){
    return (bitboard & (1ULL << square));
}

void myUtils::setBit(U64& bitboard, boardSquare square){
    bitboard |= (1ULL << square);
}

void myUtils::removeBit(U64& bitboard,boardSquare square){
    getBit(bitboard, square) ? bitboard ^= (1ULL << square): 0;
}

// counts number of bits in given ULL
int myUtils::countBits(U64 bitboard){
    int count = 0;
    while(bitboard){
        count++;
        bitboard &= bitboard - 1;
    }
    return count;
}

// counts number of bits before least significant bit in given ULL
int myUtils::firstLeastSignificantBitIndex(U64 bitboard){
    // ensure that paramater is NOT 0
    if(bitboard){
        return countBits((bitboard & -bitboard) - 1);
    }else{
        return -1; 
    }
}
// generates a specific occupancy bitboard from given num based on this 
// specific attackMask
U64 myUtils::setOccupancy(int num, int bitsInMask, U64 attackMask){
     U64 occupancy = 0ULL;
     for(int i = 0; i < bitsInMask; i++){
        int square = firstLeastSignificantBitIndex(attackMask);
        removeBit(attackMask, static_cast<boardSquare>(square));
        if(num & (1 << i)){
            occupancy |= (1ULL << square);
        }
     } 
     return occupancy;
}


// MAGIC NUMBER STUFF
// =========================
// =========================


// if bishop is 1, then it is for bishop. Otherwise, rooks


