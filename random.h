#ifndef RANDOM_H
#define RANDOM_H

#include "libconstants.h"

class Random{
    
    public:
        Random();
        unsigned int state = 1804289383;

        unsigned int getRandomU32Num();

        libConst::U64 getRandomU64Nums();

        libConst::U64 generatemagicNumCand();


};

#endif