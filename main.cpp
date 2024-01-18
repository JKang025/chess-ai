#include <iostream>
#include "chess.h"
#include "MCSTNode.h"

int main() {
    
    MCSTNode* node = new MCSTNode(nullptr, "8/8/5p2/1P1K1k2/8/2r5/8/7R w - - 0 0");
    chess::Board board;
    setupBoard(node, board);

    
    while (!board.is_game_over(true)) {
        while (true) {
            
            std::cout << board.unicode(false, true) << std::endl;
            std::cout << std::endl;
    
            std::cout << "Engine recomendation: " << std::endl;
            std::optional<chess::Move> optionalMove = calculateMove(node, board, board.turn, 3000);
            for(auto n : node->_children){
                std::cout << n->_exploitFactor << "  " << n->_numberOfVisits << std::endl;
            }
            if(optionalMove.has_value()){
                std::cout << board.san(*optionalMove) << std::endl;
            }
            std::cout << std::endl;

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
                treeCleanup(node);
                node = new MCSTNode(nullptr, board.fen());
                break;
            } catch (std::invalid_argument) {
                std::cout << "Invalid Move, Try Again..." << std::endl;
            }
        }
    }

    treeCleanup(node); // cleans up all nodes

    std::cout << "Game Over! Result: " << board.result(true);
    std::cout << std::endl;
}