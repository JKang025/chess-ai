#include "MCSTNode.h"

#include <cmath>
#include <cstdlib>  
#include <ctime>    

MCSTNode::MCSTNode(): _parent(nullptr), _numberOfVisits(0), _exploitFactor(0.0), _parentsNumberOfVisits(0),
                      _board(chess::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")){

    std::vector<MCSTNode*> v;
    _children = v; 
};

double UCB(MCSTNode* node){
    double exploitation = node->_exploitFactor;
    double exploration = 2 * (sqrt(log(node->_parentsNumberOfVisits + std::exp(1.0) + pow(10, -6)) / (node->_numberOfVisits + pow(10, -10))));
    double upperConfidenceBound =  exploitation + exploration;
    return upperConfidenceBound;
}



std::pair<MCSTNode*, double> rollout(MCSTNode* node, chess::Color color){
    if(node->_board.outcome() != std::nullopt){
        if(node->_board.outcome()->winner == std::nullopt){
            return std::make_pair(node, 0);
        }else if(node->_board.outcome()->winner == color){
            return std::make_pair(node, 1);
        }else{
            return std::make_pair(node, -1);
        }
    }

    chess::LegalMoveGenerator legalMoves = node->_board.legal_moves();
    std::vector<chess::Move> moves = legalMoves;

    for(auto move : moves){
        chess::Board temp = chess::Board(node->_board.board_fen());
        temp.push(move);
        MCSTNode* child = new MCSTNode(temp, node);
        node->_children.push_back(child);
    }

    srand(static_cast<unsigned int>(time(NULL)));
    int randomIndex = rand() % node->_children.size();
    MCSTNode* randomChild = node->_children[randomIndex];
    return rollout(randomChild, color);

}

MCSTNode* expand(MCSTNode* node, chess::Color color){
    if(node->_children.size() == 0){
        return node;
    }
    
    if(color == chess::WHITE){
        double maxUCB = -10000000000;
        MCSTNode* child = nullptr;
        for(auto e : node->_children){
            double childUCB = UCB(e);
            if(childUCB >= maxUCB){
                maxUCB = childUCB;
                child = e;
            }
        }
        return expand(child, chess::BLACK);
    }else{
        double minUCB = 1000000000;
        MCSTNode* child = nullptr;
        for(auto e : node->_children){
            double childUCB = UCB(e);
            if(childUCB <= minUCB){
                minUCB = childUCB;
                child = e;
            }
        }
        return expand(child, chess::WHITE);
    }

}

MCSTNode* rollback(MCSTNode* node, double reward){
    node->_numberOfVisits += 1;
    node->_exploitFactor += reward;
    while(node->_parent != nullptr){
        node->_parentsNumberOfVisits += 1;
        node = node->_parent;
    }
    return node;
}

chess::Move calculateMove(MCSTNode* node, bool isOver, chess::Color color, int iterations){

}