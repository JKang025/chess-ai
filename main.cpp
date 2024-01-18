#include <iostream>
#include "chess.h"
#include "MCSTNode.h"

int main() {
    
    MCSTNode* node = new MCSTNode();


    std::cout << "hehe" << sizeof(node->_board) << std::endl;
    std::cout << "hehe" << sizeof(node->_children) << std::endl;
    std::cout << "hehe" << sizeof(node->_previousMove) << std::endl;

    while (!node->_board.is_game_over(true)) {
        while (true) {
            
            std::cout << node->_board.unicode(false, true) << std::endl;
            std::cout << std::endl;
    
            std::cout << "Engine recomendation: " << std::endl;
            std::optional<chess::Move> optionalMove = calculateMove(node, node->_board.turn, 100);
            for(auto n : node->_children){
                std::cout << n->_exploitFactor << "  " << n->_numberOfVisits << std::endl;
            }
            if(optionalMove.has_value()){
                std::cout << node->_board.san(*optionalMove) << std::endl;
            }
            std::cout << std::endl;

            std::string san;
            std::cout << node->_board.ply() + 1 << ". " << (node->_board.turn ? "[WHITE] " : "[BLACK] ") << "Enter Move: ";
            std::cin >> san;
            std::cout << std::endl;
            
            

            try {
                chess::Move move = node->_board.parse_san(san);
                if (!move) {
                    throw std::invalid_argument("");
                }
                node->_board.push(move);
                
                chess::Board tempBoard = node->_board;
                treeCleanup(node);
                node = new MCSTNode(tempBoard, nullptr, move);
                break;
            } catch (std::invalid_argument) {
                std::cout << "Invalid Move, Try Again..." << std::endl;
            }
        }
    }

    treeCleanup(node); // cleans up all nodes

    std::cout << "Game Over! Result: " << node->_board.result(true);
    std::cout << std::endl;
}