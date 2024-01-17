#ifndef MCSTNODE_H
#define MCSTNODE_H

#include <unordered_set>
#include <vector>
#include "chess.h"


class MCSTNode{
    public: 

        MCSTNode();
        MCSTNode(chess::Board board, MCSTNode* parent);

        chess::Board _board;
        MCSTNode* _parent;
        int _numberOfVisits;
        int _parentsNumberOfVisits;
        double _exploitFactor; // Upper Confidence Bound
        std::vector<MCSTNode*> _children;

    private:
        


};

double UCB(MCSTNode* node);
MCSTNode* select(MCSTNode* node);
MCSTNode* expand(MCSTNode* node, chess::Color color);
std::pair<MCSTNode*, double> rollout(MCSTNode* node); //node - result
MCSTNode* rollback(MCSTNode* node, double reward);

chess::Move calculateMove(MCSTNode* node, bool isOver, chess::Color color, int iterations);




#endif