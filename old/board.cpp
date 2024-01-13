#include <unordered_map>
#include <sstream>
#include <iostream>
#include <string>
#include "board.h"
#include "utils.h"
#include "random.h"


const string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

/*
Bitboard represetation: 000...0001 represents a8, 000...0010 represents b8,
000...100000000 represents a7, etc
*/

/* 
NOT_A_FILE REPRESENTS:
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
*/

const char* squareToCoordinates[64] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", 
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", 
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", 
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", 
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", 
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", 
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", 
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1" 
    };


const U64 NOT_A_FILE = 18374403900871474942ULL;
const U64 NOT_H_FILE = 9187201950435737471ULL;
const U64 NOT_HG_FILE = 4557430888798830399ULL;
const U64 NOT_AB_FILE = 18229723555195321596ULL;

// relevant occupancy bit count for every square 
// for example a bishop on a8 would have 6 occupancy bits
const int BISHOP_RELEVANT_BITS[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

const int ROOK_RELEVANT_BITS[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

 const U64 ROOK_MAGIC_NUMBERS[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL
};

// bishop magic numbers
const U64 BISHOP_MAGIC_NUMBERS[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL
};


Board::Board() : Board(STARTING_FEN){
}

Board::Board(string fen){
    /* WILL IMPLEMENT LATER!!!!!
    ===========================


    theBoard = vector<int>(64, _piece.none);

    //seperates FEN notation into distinct sections for easy parsing
    istringstream iss(fen);
    string boardPart, turnPart, castlingPart, halfmovePart, fullmovePart, enpassantPart;
    getline(iss, boardPart, ' ');
    getline(iss, turnPart, ' ');
    getline(iss, castlingPart, ' ');
    getline(iss, enpassantPart, ' ');
    getline(iss, halfmovePart, ' ');
    getline(iss, fullmovePart, ' ');

    int rank = 7;  // Starting from the 8th rank (index 7)
    int file = 0;
    for (int i = 0; i < boardPart.size(); i++) {
        
        if (boardPart[i] == '/') {
            rank--;  // Move to the next rank
            file = 0;  // Reset file for the new rank
        }
        else if (isdigit(boardPart[i])) {
            int emptySquares = fen[i] - '0';
            file += emptySquares;
        }
        else {
            theBoard[rank * 8 + file] = charToPiece(boardPart[i]);
            file++;
        }
    }

    // Sets up the correct turn
    whiteTurn = turnPart == "w";

    // Sets up correct castling conditions
    whiteCastleKing = false;
    whiteCastleQueen = false;
    blackCastleKing = false;
    blackCastleKing = false;
    for(char c : castlingPart){
        if(c == '-'){
            break;
        }else if(c == 'K'){
            whiteCastleKing = true;
        }else if(c == 'Q'){
            whiteCastleQueen = true;
        }else if(c == 'k'){
            blackCastleKing = true;
        }else if(c == 'q'){
            blackCastleQueen = true;
        }
    }

    // Sets up enpassant sqaure
    enpassant = enpassantPart;

    // Sets up correct move and half-move 
    move = stoi(fullmovePart);
    halfMove = stoi(halfmovePart);  
    */ 
}



// ATTACKS
// =========================
// =========================

// generates possible attacks by pawn given its position square and side
U64 Board::generatePawnAttacks(boardSquare square, color side){

    U64 attacks = 0ULL; // bitboard of possible attacks by pawn occupied in square
    U64 bitboard = 0ULL; 
    setBit(bitboard, square); // sets pawn to given location

    // white
    if(!side){
        if((bitboard >> 7) & NOT_A_FILE){
            attacks |= bitboard >> 7;
        }
        if((bitboard >> 9) & NOT_H_FILE){
            attacks |= bitboard >> 9;
        }
    }
    // black
    else{  
        if((bitboard << 7) & NOT_H_FILE){
            attacks |= bitboard << 7;
        }
        if((bitboard << 9) & NOT_A_FILE){
            attacks |= bitboard << 9;
        }

    }
    return attacks;
}

// generates possible attacks by knight given its position square
U64 Board::generateKnightAttacks(boardSquare square){
    U64 attacks = 0ULL; // bitboard of possible attacks by knight occupied in square
    U64 bitboard = 0ULL; 
    setBit(bitboard, square); // sets knight to given location

    // 17, 15, 10, 6 are the offsets for a knight's L-shaped move
    if((bitboard >> 17) & NOT_H_FILE){
        attacks |= bitboard >> 17;
    }
    if((bitboard >> 15) & NOT_A_FILE){
        attacks |= bitboard >> 15;
    }
    if((bitboard >> 10) & NOT_HG_FILE){
        attacks |= bitboard >> 10;
    }
    if((bitboard >> 6) & NOT_AB_FILE){
        attacks |= bitboard >> 6;
    }

    // in the opposite direction
    if((bitboard << 17) & NOT_A_FILE){
        attacks |= bitboard << 17;
    }
    if((bitboard << 15) & NOT_H_FILE){
        attacks |= bitboard << 15;
    }
    if((bitboard << 10) & NOT_AB_FILE){
        attacks |= bitboard << 10;
    }
    if((bitboard << 6) & NOT_HG_FILE){
        attacks |= bitboard << 6;
    }
    return attacks;
}

// generates possible attacks by king given its position square
U64 Board::generateKingAttacks(boardSquare square){
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    setBit(bitboard, square);

    if(bitboard >> 8){
        attacks |= bitboard >> 8; 
    }
    if((bitboard >> 9) & NOT_H_FILE){
        attacks |= bitboard >> 9;
    }
    if((bitboard >> 7) & NOT_A_FILE){
        attacks |= bitboard >>7;
    }
    if((bitboard >> 1) & NOT_H_FILE){
        attacks |= bitboard >> 1;
    }

    if(bitboard << 8){
        attacks |= bitboard << 8; 
    }
    if((bitboard << 9) & NOT_A_FILE){
        attacks |= bitboard << 9;
    }
    if((bitboard << 7) & NOT_H_FILE){
        attacks |= bitboard << 7;
    }
    if((bitboard << 1) & NOT_A_FILE){
        attacks |= bitboard << 1;
    }

    return attacks;
    
}

// initializes arrays that contain possible attack moves given a square for LEAPING pieces
void Board::initializeLeaperPieces(){
    for(int i = 0; i < 64; i++){
        pawnAttacks[white][i] = Board::generatePawnAttacks(static_cast<boardSquare>(i), white);
        pawnAttacks[black][i] = Board::generatePawnAttacks(static_cast<boardSquare>(i), black);

        knightAttacks[i] = Board::generateKnightAttacks(static_cast<boardSquare>(i));

        kingAttacks[i] = Board::generateKingAttacks(static_cast<boardSquare>(i));
    }
}

// generates relevant occupancy spots for bishops for magic bitboard
U64 Board::generateBishopAttacks(boardSquare square){
    U64 attacks = 0ULL;

    int rank, file;
    int targetRank =  square / 8;
    int targetFile = square % 8;

    // mask relevant occupancy spots for bishop
    for(rank = targetRank + 1, file = targetFile + 1; rank < 7 && file < 7; rank++, file++){
        attacks |=  (1ULL << (rank * 8 + file));
    }
    for(rank = targetRank - 1, file = targetFile + 1; rank > 0 && file < 7; rank--, file++){
        attacks |=  (1ULL << (rank * 8 + file));
    }
    for(rank = targetRank + 1, file = targetFile - 1; rank < 7 && file > 0; rank++, file--){
        attacks |=  (1ULL << (rank * 8 + file));
    }
    for(rank = targetRank - 1, file = targetFile - 1; rank > 0 && file > 0; rank--, file--){
        attacks |=  (1ULL << (rank * 8 + file));
    }
    return attacks;
}

// generates relevant occupancy spots for rooks for magic bitboard
U64 Board::generateRookAttacks(boardSquare square){
    U64 attacks = 0ULL;

    int rank, file;
    int targetRank =  square / 8;
    int targetFile = square % 8;

    // mask relevant occupancy spots for rook
    for(rank = targetRank + 1; rank < 7; rank++){
        attacks |= (1ULL << (rank * 8 + targetFile));
    }
    for(rank = targetRank - 1; rank > 0; rank--){
        attacks |= (1ULL << (rank * 8 + targetFile));
    }
    for(file = targetFile + 1; file < 7; file++){
        attacks |= (1ULL << (targetRank * 8 + file)); 
    }
    for(file = targetFile - 1; file > 0; file--){
        attacks |= (1ULL << (targetRank * 8 + file)); 
    }
    
    return attacks;
}

// generates bishop atacking moves given block
U64 Board::generateRealBishopAttacks(boardSquare square, U64 block){
    U64 attacks = 0ULL;

    int rank, file;
    int targetRank =  square / 8;
    int targetFile = square % 8;

    // bishop attacks
    for(rank = targetRank + 1, file = targetFile + 1; rank < 8 && file < 8; rank++, file++){
        attacks |=  (1ULL << (rank * 8 + file));
        if((1ULL << (rank * 8 + file)) & block){
            break;
        }
    }
    for(rank = targetRank - 1, file = targetFile + 1; rank >= 0 && file < 8; rank--, file++){
        attacks |=  (1ULL << (rank * 8 + file));
        if((1ULL << (rank * 8 + file)) & block){
            break;
        }
    }
    for(rank = targetRank + 1, file = targetFile - 1; rank < 8 && file >= 0; rank++, file--){
        attacks |=  (1ULL << (rank * 8 + file));
        if((1ULL << (rank * 8 + file)) & block){
            break;
        }
    }
    for(rank = targetRank - 1, file = targetFile - 1; rank >= 0 && file >= 0; rank--, file--){
        attacks |=  (1ULL << (rank * 8 + file));
        if((1ULL << (rank * 8 + file)) & block){
            break;
        }
    }
    return attacks;
}

// generates rook atacking moves given block
U64 Board::generateRealRookAttacks(boardSquare square, U64 block){
    U64 attacks = 0ULL;

    int rank, file;
    int targetRank =  square / 8;
    int targetFile = square % 8;

    // mask relevant occupancy spots for rook
    for(rank = targetRank + 1; rank < 8; rank++){
        attacks |= (1ULL << (rank * 8 + targetFile));
        if(1ULL << (rank * 8 + targetFile) & block){
            break;
        }
    }
    for(rank = targetRank - 1; rank >= 0; rank--){
        attacks |= (1ULL << (rank * 8 + targetFile));
        if(1ULL << (rank * 8 + targetFile) & block){
            break;
        }
    }
    for(file = targetFile + 1; file < 8; file++){
        attacks |= (1ULL << (targetRank * 8 + file)); 
        if(1ULL << (targetRank * 8 + file) & block){
            break;
        }
    }
    for(file = targetFile - 1; file >= 0; file--){
        attacks |= (1ULL << (targetRank * 8 + file)); 
        if(1ULL << (targetRank * 8 + file) & block){
            break;
        }
    }
    
    return attacks;
}

void Board::initializeAll(){
    initializeLeaperPieces();
    // initializeMagicNumber(); NOT NESSESARY, ALREADY PRE GENERATED
    

}

// MAGIC NUM
// ===================
// ===================

U64 Board::findMagicNumber(boardSquare square, int relevant_bits, int isBishop)
{
    // init occupancies
    U64 occupancies[4096];
    
    // init attack tables
    U64 attacks[4096];
    
    // init used attacks
    U64 used_attacks[4096];
    
    // init attack mask for a current piece
    U64 attack_mask = isBishop ? generateBishopAttacks(square) : generateRookAttacks(square);
    
    // init occupancy indicies
    int occupancy_indicies = 1 << relevant_bits;
    
    // loop over occupancy indicies
    for (int index = 0; index < occupancy_indicies; index++)
    {
        // init occupancies
        occupancies[index] = setOccupancy(index, relevant_bits, attack_mask);
        
        // init attacks
        attacks[index] = isBishop ? generateRealBishopAttacks(square, occupancies[index]) :
                                    generateRealRookAttacks(square, occupancies[index]);
    }
    Random rand = Random();
    // test magic numbers loop
    for (int random_count = 0; random_count < 100000000; random_count++)
    {
        // generate magic number candidate
        U64 magic_number = rand.generatemagicNumCand();
        
        // skip inappropriate magic numbers
        if (countBits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;
        
        // init used attacks
        memset(used_attacks, 0ULL, sizeof(used_attacks));
        
        // init index & fail flag
        int index, fail;
        
        // test magic index loop
        for (index = 0, fail = 0; !fail && index < occupancy_indicies; index++)
        {
            // init magic index
            int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));
            
            // if magic index works
            if (used_attacks[magic_index] == 0ULL)
                // init used attacks
                used_attacks[magic_index] = attacks[index];
            
            // otherwise
            else if (used_attacks[magic_index] != attacks[index])
                // magic index doesn't work
                fail = 1;
        }
        
        // if magic number works
        if (!fail)
            // return it
            return magic_number;
    }
    
    // if magic number doesn't work
    printf("  Magic number fails!\n");
    return 0ULL;
}

// function used to generate the magic number constants seen above for rook and bishop
/*
void Board::initializeMagicNumber()
{
    for(int i = 0; i < 64; i++){
        printf(" 0x%lluxULL\n", findMagicNumber(static_cast<boardSquare>(i), BISHOP_RELEVANT_BITS[i], 1));
    }
  
    for (int square = 0; square < 64; square++)
        // init rook magic numbers
        ROOK_MAGIC_NUMBERS[square] = findMagicNumber(static_cast<boardSquare>(square), ROOK_RELEVANT_BITS[square], 0);

    for (int square = 0; square < 64; square++)
        // init bishop magic numbers
        BISHOP_MAGIC_NUMBERS[square] = findMagicNumber(static_cast<boardSquare>(square), BISHOP_RELEVANT_BITS[square], 1);
    
}
*/