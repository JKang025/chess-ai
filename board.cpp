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

void Board::initializeLeaperPieces(){
    for(int i = 0; i < 64; i++){
        pawnAttacks[Board::white][i] = Board::generatePawnAttacks(static_cast<Board::boardSquare>(i), Board::white);
        pawnAttacks[Board::black][i] = Board::generatePawnAttacks(static_cast<Board::boardSquare>(i), Board::black);

        knightAttacks[i] = Board::generateKnightAttacks(static_cast<Board::boardSquare>(i));

        kingAttacks[i] = Board::generateKingAttacks(static_cast<Board::boardSquare>(i));
    }
}