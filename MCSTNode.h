#ifndef MCSTNODE_H
#define MCSTNODE_H

#include <unordered_set>
#include "chess.h"


class MCSTNode{
    public: 

        MCSTNode();
        MCSTNode(chess::Board board, MCSTNode* parent);

        chess::Board _board;
        MCSTNode* _parent;
        int _numberOfVisits;
        double _exploitFactor; // Upper Confidence Bound
        std::unordered_set<MCSTNode*> _children;

    private:
        


};

double UCB(MCSTNode* node);
MCSTNode* select(MCSTNode* node);
MCSTNode* expand(MCSTNode* node);
std::pair<MCSTNode*, int> rollout(MCSTNode* node); //node - result
MCSTNode* rollback(MCSTNode* node, double reward);

uint16_t calculateMove(MCSTNode* node, bool isOver, int white, int iterations);




#endif