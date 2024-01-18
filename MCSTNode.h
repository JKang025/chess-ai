#ifndef MCSTNODE_H
#define MCSTNODE_H

#include <vector>
#include "chess.h"


class MCSTNode{
    public: 

        MCSTNode();
        MCSTNode(chess::Board board, MCSTNode* parent, chess::Move previousMove);

        chess::Board _board;
        MCSTNode* _parent;
        int _numberOfVisits;
        //int _parentsNumberOfVisits;
        double _exploitFactor; // Upper Confidence Bound
        std::vector<MCSTNode*> _children;
        chess::Move _previousMove;

    private:
        


};

double UCB(MCSTNode* node);
MCSTNode* select(MCSTNode* node);
MCSTNode* expand(MCSTNode* node, chess::Color color);
std::pair<MCSTNode*, double> rollout(MCSTNode* node); //node - result
MCSTNode* rollback(MCSTNode* node, double reward);

void treeCleanup(MCSTNode* node);
void deleteTree(MCSTNode* root);

std::optional<chess::Move> calculateMove(MCSTNode* node, chess::Color white, int iterations);




#endif