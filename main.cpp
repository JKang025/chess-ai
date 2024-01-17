#include <iostream>
#include "chess.h"
#include "MCSTNode.h"

int main() {
    std::cout << "hehe" << std::endl;
    chess::Board board;


    MCSTNode* n = new MCSTNode();

    std::cout << std::string(n->_board) << std::endl;
    delete n;

    std::cout << "test" << std::endl;

    while (!board.is_game_over(true)) {
        while (true) {
            std::cout << board.unicode(false, true) << std::endl;

            std::string san;
            std::cout << board.ply() + 1 << ". " << (board.turn ? "[WHITE] " : "[BLACK] ") << "Enter Move: ";
            std::cin >> san;
            std::cout << std::endl;

            try {
                chess::Move move = board.parse_san(san);
                if (!move) {
                    throw std::invalid_argument("");
                }
                board.push(move);
                break;
            } catch (std::invalid_argument) {
                std::cout << "Invalid Move, Try Again..." << std::endl;
            }
        }
    }

    std::cout << "Game Over! Result: " << board.result(true);
}