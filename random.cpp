#include "random.h"

Random::Random(){

}

unsigned int Random::getRandomU32Num(){
        unsigned int num = state;
        num ^= num << 13;
        num ^= num >> 17;
        num ^= num << 5;
        state = num;
        return num;

}

libConst::U64 Random::getRandomU64Nums(){
        libConst::U64 n1, n2, n3, n4;

        // init random number slicing 17 bits from most significsant bit side
        n1 = (libConst::U64)(getRandomU32Num()) & 0xFFFF;
        n2 = (libConst::U64)(getRandomU32Num()) & 0xFFFF;
        n3 = (libConst::U64)(getRandomU32Num()) & 0xFFFF;
        n4 = (libConst::U64)(getRandomU32Num()) & 0xFFFF;

        return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

libConst::U64 Random::generatemagicNumCand(){
        return getRandomU64Nums() & getRandomU64Nums() & getRandomU64Nums();
}