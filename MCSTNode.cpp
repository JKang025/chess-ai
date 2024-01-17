#include "MCSTNode.h"

#include <cmath>

MCSTNode::MCSTNode(): _parent(nullptr), _numberOfVisits(0), _exploitFactor(0.0), 
                      _board(chess::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")){

    std::unordered_set<MCSTNode*> s;
    _children = s; 
};

double UCB(MCSTNode* node){
    double exploitation = node->_exploitFactor;
    double exploration = 2 * (sqrt(log(node->_parent->_numberOfVisits + std::exp(1.0) + pow(10, -6)) / (node->_numberOfVisits + pow(10, -10))));
    double upperConfidenceBound =  exploitation + exploration;
    return upperConfidenceBound;
}

std::pair<MCSTNode*, int> rollout(MCSTNode* node){

}