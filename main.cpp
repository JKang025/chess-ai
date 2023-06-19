#include <iostream>
#include "board.h"
#include "random.h"

/*

*/



int main(int argc, char const *argv[]){
  
   Board bruh = Board();
   Random rand = Random();
   
    bruh.initializeLeaperPieces();
    bruh.printBitboard((U64)rand.getRandomU32Num());
    bruh.printBitboard((U64)rand.getRandomU32Num() & 0xFFFF); //remove first 16 bits
    bruh.printBitboard(rand.getRandomU64Nums());
    bruh.printBitboard(rand.generatemagicNumCand());

    
    
    

    
};