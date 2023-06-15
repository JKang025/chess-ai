#include <unordered_map>
#include <sstream>
#include <iostream>
#include <string>
#include "board.h"

using namespace std; 

const string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const int BOARD_WIDTH = 8;
const int BOARD_HEIGHT = 8;

/*
Bitboard represetation: 000...0001 represents a8, 000...0010 represents b8,
000...100000000 represents a7, etc
*/

/* 
NOT A FILE REPRESENTS:
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
0 1 1 1 1 1 1 1 
*/
const U64 NOT_A_FILE = 18374403900871474942ULL;
const U64 NOT_H_FILE = 9187201950435737471ULL;
const U64 NOT_HG_FILE = 4557430888798830399ULL;
const U64 NOT_AB_FILE = 18229723555195321596ULL;

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

void Board::printBitboard(U64 bitboard){
    cout << endl;
    for(int rank = 0; rank < BOARD_HEIGHT; rank++){
        for(int file = 0; file < BOARD_WIDTH; file++){

            if(file == 0){
                cout << 8 - rank << " ";
            }
            int square = rank * 8 + file;
            cout << (getBit(bitboard, static_cast<Board::boardSquare>(square)) ? 1 : 0) << " "; 
        }
        cout << "\n";
    }
    cout << "  a b c d e f g h\n \n";
    cout << bitboard << "\n";

}

// BIT MANIPULATION
// =========================
// =========================
U64 Board::getBit(U64 bitboard, Board::boardSquare square){
    return (bitboard & (1ULL << square));
}

void Board::setBit(U64& bitboard, Board::boardSquare square){
    bitboard |= (1ULL << square);
}

void Board::removeBit(U64& bitboard, Board::boardSquare square){
    getBit(bitboard, square) ? bitboard ^= (1ULL << square): 0;
}

// ATTACKS
// =========================
// =========================

// generates possible attacks by pawn given its position square and side
U64 Board::generatePawnAttacks(Board::boardSquare square, Board::color side){

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
U64 Board::generateKnightAttacks(Board::boardSquare square){
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
U64 Board::generateKingAttacks(Board::boardSquare square){
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
        pawnAttacks[Board::white][i] = Board::generatePawnAttacks(static_cast<Board::boardSquare>(i), Board::white);
        pawnAttacks[Board::black][i] = Board::generatePawnAttacks(static_cast<Board::boardSquare>(i), Board::black);

        knightAttacks[i] = Board::generateKnightAttacks(static_cast<Board::boardSquare>(i));

        kingAttacks[i] = Board::generateKingAttacks(static_cast<Board::boardSquare>(i));
    }
}

// generates relevant occupancy spots for bishops for magic bitboard
U64 Board::generateBishopAttacks(Board::boardSquare square){
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
U64 Board::generateRookAttacks(Board::boardSquare square){
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
U64 Board::generateRealBishopAttacks(Board::boardSquare square, U64 block){
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
U64 Board::generateRealRookAttacks(Board::boardSquare square, U64 block){
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