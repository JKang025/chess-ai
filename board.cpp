#include <unordered_map>
#include <sstream>
#include <iostream>
#include <string>
#include "board.h"

using namespace std; 

const string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const int BOARD_WIDTH = 8;
const int BOARD_HEIGHT = 8;

Board::Board() : Board(STARTING_FEN){
}

Board::Board(string fen){

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
}

void Board::printBitboard(U64 bitboard){
    cout << endl;
    for(int rank = 0; rank < BOARD_HEIGHT; rank++){
        for(int file = 0; file < BOARD_WIDTH; file++){

            if(file == 0){
                cout << 8 - rank << " ";
            }
            int square = rank * 8 + file;
            cout << (getBit(bitboard, square) ? 1 : 0) << " "; 
        }
        cout << "\n";
    }
    cout << "  a b c d e f g h\n \n";
    cout << bitboard << "\n";

}

U64 Board::getBit(U64 bitboard, int square){
    return (bitboard & (1ULL << square));
}

void Board::setBit(U64& bitboard, Board::boardSquare square){
    bitboard |= (1ULL << square);
}

void Board::removeBit(U64& bitboard, Board::boardSquare square){
    getBit(bitboard, square) ? bitboard ^= (1ULL << square): 0;
}

//non bitboard displayboard. Most likely useless
/*
void Board::displayBoard(){

    std::cout << "  a b c d e f g h" << std::endl;
    std::cout << " -----------------" << std::endl;

    for (int rank = BOARD_HEIGHT - 1; rank >= 0; rank--) {
        std::cout << rank + 1 << "|";
        for (int file = 0; file < BOARD_HEIGHT; file++) {
            int p = theBoard[rank * 8 + file];
            std::cout << pieceToChar(p) << "|";
        }
        std::cout << " " << rank + 1 << std::endl;
    }

    std::cout << " -----------------" << std::endl;
    std::cout << "  a b c d e f g h" << std::endl;
    
}
*/

int Board::charToPiece(char c){
    unordered_map<char, int> charToPieces;
    charToPieces['n'] = _piece.black | _piece.knight;
    charToPieces['k'] = _piece.black | _piece.king;
    charToPieces['b'] = _piece.black | _piece.bishop;
    charToPieces['p'] = _piece.black | _piece.pawn;
    charToPieces['q'] = _piece.black | _piece.queen;
    charToPieces['r'] = _piece.black | _piece.rook;
    charToPieces['N'] = _piece.white | _piece.knight;
    charToPieces['K'] = _piece.white | _piece.king;
    charToPieces['B'] = _piece.white | _piece.bishop;
    charToPieces['P'] = _piece.white | _piece.pawn;
    charToPieces['Q'] = _piece.white | _piece.queen;
    charToPieces['R'] = _piece.white | _piece.rook;
    charToPieces[' '] = _piece.none;
    return charToPieces[c];
}

char Board::pieceToChar(int i){
    unordered_map<int, char> pieceToChar;
    pieceToChar[_piece.black | _piece.knight] = 'n';
    pieceToChar[_piece.black | _piece.king] = 'k';
    pieceToChar[_piece.black | _piece.bishop] = 'b';
    pieceToChar[_piece.black | _piece.pawn] = 'p';
    pieceToChar[_piece.black | _piece.queen] = 'q';
    pieceToChar[_piece.black | _piece.rook] = 'r';
    pieceToChar[_piece.white | _piece.knight] = 'N';
    pieceToChar[_piece.white | _piece.king] = 'K';
    pieceToChar[_piece.white | _piece.bishop] = 'B';
    pieceToChar[_piece.white | _piece.pawn] = 'P';
    pieceToChar[_piece.white | _piece.queen] = 'Q';
    pieceToChar[_piece.white | _piece.rook] = 'R';
    pieceToChar[_piece.none] = ' ';
    return pieceToChar[i];

}
