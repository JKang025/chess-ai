#ifndef MCSTNODE_H
#define MCSTNODE_H

#include <vector>
#include "chess.h"


class MCSTNode{
    public: 

        MCSTNode();
        //MCSTNode(chess::Board board, MCSTNode* parent, chess::Move previousMove);

        MCSTNode(MCSTNode* parent, std::string fen, chess::Color);

        //chess::Board _board;
        MCSTNode* _parent;
        int _numberOfVisits;
        //int _parentsNumberOfVisits;
        double _exploitFactor; // Upper Confidence Bound
        std::vector<MCSTNode*> _children;
        //chess::Move _previousMove;

        std::string _fen; 
        chess::Color _turn;

    private:
        


};

double UCB(MCSTNode* node);
MCSTNode* select(MCSTNode* node, chess::Board& board);
MCSTNode* expand(MCSTNode* node, chess::Board& color);
std::pair<MCSTNode*, std::optional<chess::Color>> rollout(MCSTNode* node, chess::Board& board); //node - result
MCSTNode* rollback(MCSTNode* node, double reward);

void treeCleanup(MCSTNode* node);
void deleteTree(MCSTNode* root);

std::optional<chess::Move> calculateMove(MCSTNode* node, chess::Board board, chess::Color white, int iterations);

void setupBoard(MCSTNode* node, chess::Board& board);


#endif