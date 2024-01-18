#include <iostream>
#include "chess.h"
#include "MCSTNode.h"

int main() {
    
    MCSTNode* node = new MCSTNode(nullptr, "8/8/5p2/1P1K1k2/8/2r5/8/7R w - - 0 0", chess::BLACK);
    //MCSTNode* node = new MCSTNode(nullptr, "8/8/4k1KP/p5P1/r7/8/8/8 w - - 0 0", chess::BLACK);

    //early/mid game pos
    //MCSTNode* node = new MCSTNode(nullptr, "r1bqk2r/pppp1ppp/2n2n2/2b5/2BpP3/2P2N2/PP3PPP/RNBQK2R w KQkq - 0 6", chess::BLACK);
    //
    //MCSTNode* node = new MCSTNode();
    

    chess::Board board;
    setupBoard(node, board);

    chess::Color origTurn = node->_turn;

    while (!board.is_game_over(true)) {
        while (true) {
            
            std::cout << board.unicode(false, true) << std::endl;
            std::cout << std::endl;

            if(origTurn == node->_turn){
                std::cout << "Engine recomendation: " << std::endl;
                std::optional<chess::Move> optionalMove = calculateMove(node, board, board.turn, 10000);

                for(auto n : node->_children){
                    std::cout << UCB(n) << "   " << UCB(n) - n->_exploitFactor / n->_numberOfVisits << " " << n ->_exploitFactor << "  " << n->_numberOfVisits << std::endl;
                }

                if(optionalMove.has_value()){
                    std::cout << board.san(*optionalMove) << std::endl;
                }
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

                MCSTNode* temp = new MCSTNode(nullptr, board.fen(), !node->_turn);
                treeCleanup(node);
                node = temp;
                
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