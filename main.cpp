#include <iostream>
#include "board.h"
#include "random.h"
#include "utils.h"

using namespace libConst;
using namespace Utils;
using namespace std;






int main(int argc, char const *argv[]){
  
   Board bruh = Board();
   Random rand = Random();
   
    bruh.initializeLeaperPieces();
    printBitboard((U64)rand.getRandomU32Num());
    printBitboard((U64)rand.getRandomU32Num() & 0xFFFF); //remove first 16 bits
    printBitboard(rand.getRandomU64Nums());
    printBitboard(rand.generatemagicNumCand());

    
    
    

    
};