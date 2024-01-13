#ifndef RANDOM_H
#define RANDOM_H

#include "libconstants.h"
#include "utils.h"


using namespace myConstants;
using namespace myUtils;

class Random{
    
    public:
        Random();
        unsigned int state = 1804289383;

        unsigned int getRandomU32Num();
        U64 getRandomU64Nums();
        U64 generatemagicNumCand();


};

#endif