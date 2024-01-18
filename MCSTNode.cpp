#include "MCSTNode.h"

#include <cmath>
#include <cstdlib>  
#include <ctime>    
#include <climits>
#include <limits>
#include <iostream>
#include <string>

MCSTNode::MCSTNode()
    : _parent(nullptr), 
      _numberOfVisits(0), 
      _exploitFactor(0.0),
      _board(chess::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")),
      _children(), // initialize as empty vector
      _previousMove(chess::Move::null())
{
}

MCSTNode::MCSTNode(chess::Board board, MCSTNode* parent, chess::Move move)
    : _parent(parent), 
      _numberOfVisits(0), 
      _exploitFactor(0.0),
      _board(board),
      _children() ,
      _previousMove(move)
{
    
}


double UCB(MCSTNode* node){
    // double exploitation = node->_exploitFactor;
    // double exploration = 2 * (sqrt(log(node->_parent->_numberOfVisits + std::exp(1.0) + pow(10, -6)) / (node->_numberOfVisits + pow(10, -10))));
    // double upperConfidenceBound =  exploitation + exploration;
    // return upperConfidenceBound;

    if (node->_numberOfVisits == 0) {
        // To handle the case of division by zero and encourage exploration of unvisited nodes
        return std::numeric_limits<double>::max();
    }
        
    // If the node is the root, it may not have a parent, handle division by zero
    //int parentVisits = (node->_parent != nullptr) ? node->_parent->_numberOfVisits : 1;
    int parentVisits;
    if(node->_parent != nullptr){
        if(node->_parent->_numberOfVisits == 0){
            parentVisits = 1;
        }else{
            parentVisits = node->_parent->_numberOfVisits;
        }
    }else{
        parentVisits = 1;
    }

    // UCB1 formula
    double EXPLORATION_CONSTANT = 1.414;
    return node->_exploitFactor + EXPLORATION_CONSTANT * std::sqrt((2 * std::log(parentVisits)) / node->_numberOfVisits);
}



std::pair<MCSTNode*, double> rollout(MCSTNode* node){
  

    if(node->_board.outcome() != std::nullopt){
        if(node->_board.outcome()->winner == std::nullopt){
            //std::cout << "draw" <<std::endl; //FOR DEBUGGING DELETE LATER
            return std::make_pair(node, 0.5);
        }else if(node->_board.outcome()->winner == chess::WHITE){
            //std::cout << "white won" <<std::endl;
            return std::make_pair(node, 1);
        }else{
            //std::cout << "black won" <<std::endl;
            return std::make_pair(node, -1);
        }
    }

    chess::LegalMoveGenerator legalMoves = node->_board.legal_moves();
    std::vector<chess::Move> moves = legalMoves;

    for(auto move : moves){
        chess::Board temp = chess::Board(node->_board.copy());
        temp.push(move);
        MCSTNode* child = new MCSTNode(temp, node, move);
        node->_children.push_back(child);
    }

    srand(static_cast<unsigned int>(time(NULL)));
    int randomIndex = rand() % node->_children.size();
    MCSTNode* randomChild = node->_children[randomIndex];
    return rollout(randomChild);

}

MCSTNode* expand(MCSTNode* node, chess::Color color){
    if(node->_children.size() == 0){
        return node;
    }
    
    if(color == chess::WHITE){
        double maxUCB = std::numeric_limits<double>::lowest();;
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
        double minUCB = std::numeric_limits<double>::max();
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

    while(node->_parent != nullptr){
        node->_numberOfVisits += 1;
        node->_exploitFactor += reward;
        node = node->_parent;
    }
    return node;
}

std::optional<chess::Move> calculateMove(MCSTNode* node, chess::Color white, int iterations){

    // should never reach here
    if(node->_board.outcome() != std::nullopt){
        return std::nullopt;
    }

    std::unordered_map<MCSTNode*, chess::Move> nodeToMove;

    chess::LegalMoveGenerator legalMoves = node->_board.legal_moves();
    std::vector<chess::Move> moves = legalMoves;

    for(chess::Move move : moves){
        chess::Board temp = chess::Board(node->_board.board_fen());
        temp.push(move);
        MCSTNode* child = new MCSTNode(temp, node, move);
        node->_children.push_back(child);

        if (nodeToMove.find(child) == nodeToMove.end()) {
            nodeToMove.insert({child, move});
        }

    }

    while(iterations > 0){
        if(white){
            double maxUCB = std::numeric_limits<double>::lowest();;
            MCSTNode* selectChild = nullptr;
            for(auto child : node->_children){
                if(UCB(child) >= maxUCB){
                    
                    selectChild = child;
                    maxUCB = UCB(child);
                }
            }

            
            std::cout << "iteration: " << iterations << std::endl;
            

            

            MCSTNode* expandChild = expand(selectChild, chess::WHITE);

            //std::cout << std::string(expandChild->_board) << std::endl;

        

            std::pair<MCSTNode*, double> rolloutChild = rollout(expandChild);


            rollback(rolloutChild.first, rolloutChild.second);
            iterations -= 1;
        }else{
            double minUCB = std::numeric_limits<double>::max();
            MCSTNode* selectChild = nullptr;
            for(auto child : node->_children){
                if(UCB(child) <= minUCB){
                    selectChild = child;
                    minUCB = UCB(child);
                }
            }

            MCSTNode* expandChild = expand(selectChild, chess::BLACK);
            std::pair<MCSTNode*, double> rolloutChild = rollout(expandChild);
            rollback(rolloutChild.first, rolloutChild.second);
            iterations -= 1;
        }
    }

    // select best node after tree is constructed
    // used heuristic: max visited node
    int maxVisited = INT_MIN;
    MCSTNode* nextNode = nullptr;
    for(auto child : node->_children){
        if(child->_numberOfVisits >= maxVisited){
            nextNode = child;
            maxVisited = child->_numberOfVisits;
        }
    }

    auto it = nodeToMove.find(nextNode);
    if (it != nodeToMove.end()) {
        return it->second; 
    } 

    // should never reach here
    return std::nullopt;
    
}


// given any node, clean up the tree (any node linked to the given node, including parents)
void treeCleanup(MCSTNode* node){
    while(node->_parent != nullptr){
        node = node->_parent;
    }
    deleteTree(node);
}

// deletes given node and all descendents
void deleteTree(MCSTNode* root){
    if(root == nullptr){
        return;
    }
    for(auto child : root->_children){
        deleteTree(child);
    }
    delete root;
    root = nullptr;
    return;
}