#include "MCSTNode.h"

#include <cmath>
#include <cstdlib>  
#include <ctime>    
#include <climits>
#include <limits>
#include <iostream>
#include <string>
#include <set>

double EPSILON = 0.1; // for the select function determines how random it is
double EXPLORATION_CONSTANT = 4; // for the UCB1 function, determines the exploration exploitation balance

MCSTNode::MCSTNode()
    : _parent(nullptr), 
      _numberOfVisits(0), 
      _exploitFactor(0.0),
      _children(), // initialize as empty vector
      _fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
      _turn(chess::BLACK)
{
}

MCSTNode::MCSTNode(MCSTNode* parent, std::string fen, chess::Color color)
    : _parent(parent), 
      _numberOfVisits(0), 
      _exploitFactor(0.0),
      _children(),
      _fen(fen),
      _turn(color)
{
    
}


double UCB(MCSTNode* node){

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
    return node->_exploitFactor / node->_numberOfVisits + EXPLORATION_CONSTANT * std::sqrt((std::log(parentVisits)) / node->_numberOfVisits);
}

MCSTNode* select(MCSTNode* node, chess::Board& board){

    setupBoard(node, board);
    chess::LegalMoveGenerator legalMoves = board.legal_moves();
    std::vector<chess::Move> moves = legalMoves;

    // not fully expanded, a "leaf" node or meets terminated node
    if(moves.size() > node->_children.size() || board.outcome() != std::nullopt){ 
        
        return node;
    }
     
    // continue to select

    double maxUCB = std::numeric_limits<double>::lowest();;
    MCSTNode* child = nullptr;

     
    double randValue = static_cast<double>(rand()) / RAND_MAX; // Random value between 0 and 1
    if (randValue < EPSILON) {
        // Choose a random child
        int randomIndex = rand() % node->_children.size();
        auto it = node->_children.begin();
        std::advance(it, randomIndex);
        child = *it;
    } else {
        // continue to select based on UCB
        for(auto e : node->_children){
            double childUCB = UCB(e);
            if(childUCB >= maxUCB){
                maxUCB = childUCB;
                child = e;
            }
        }
    }
    return select(child, board);
}

MCSTNode* expand(MCSTNode* node, chess::Board& board){
    

    setupBoard(node, board);
    chess::LegalMoveGenerator legalMoves = board.legal_moves();
    std::vector<chess::Move> moves = legalMoves;

    for(auto move : moves){
        board.push(move);
        std::string moveFen = board.fen();
        board.pop();

        bool found = false;
        for(auto child : node->_children){
            if(child->_fen == moveFen){
                found = true;
                break;
            }
        }
        if(!found){
            return new MCSTNode(node, moveFen, !node->_turn);
        }
    }

    // should never reach
    return nullptr; 
    
}




std::pair<MCSTNode*, std::optional<chess::Color>> rollout(MCSTNode* node, chess::Board& board){
  
    setupBoard(node, board);

    if(board.is_game_over(true)){

        // FOR DEBUGGING
        // auto res = board.outcome()->winner;
        // if(res == std::nullopt){
        // }else{
        //     if(res == chess::WHITE){
        //         std::cout << "white" << std::endl;
        //     }else{
        //         std::cout << "black" << std::endl;
        //     }
        // }
        //std::cout << board.outcome()->result() << std::endl;

       
       
        return std::make_pair(node, board.outcome()->winner);
        
    }

    chess::LegalMoveGenerator legalMoves = board.legal_moves();
    std::vector<chess::Move> moves = legalMoves;
   
    srand(static_cast<unsigned int>(time(NULL)));
    int randomIndex = rand() % moves.size();
    chess::Move move = moves[randomIndex];
    board.push(move);
    MCSTNode* child = new MCSTNode(node, board.fen(), !node->_turn);
    board.pop();
    node->_children.push_back(child);


    return rollout(child, board);

}



MCSTNode* rollback(MCSTNode* node, std::optional<chess::Color> winner){

    while(node->_parent != nullptr){
        node->_numberOfVisits += 1;

       if(winner == std::nullopt){
            node = node->_parent;
            continue;
       }

        if(winner == node->_turn){
            node->_exploitFactor += 1;
        }else if(!winner == node->_turn){
            node->_exploitFactor -= 1;
        }

        node = node->_parent;
    }
    node->_numberOfVisits += 1;
    return node;
}




std::optional<chess::Move> calculateMove(MCSTNode* node, chess::Board board, chess::Color white, int iterations){

    setupBoard(node, board);

    // should never reach here
    if(board.outcome(true) != std::nullopt){
        return std::nullopt;
    }

    std::unordered_map<MCSTNode*, chess::Move> nodeToMove;

    chess::LegalMoveGenerator legalMoves = board.legal_moves();
    std::vector<chess::Move> moves = legalMoves;

    // initial expansion to every child node to get started
    for(chess::Move move : moves){
        
        board.push(move);
        MCSTNode* child = new MCSTNode(node, board.fen(), !node->_turn); //chhange
        board.pop();
        node->_children.push_back(child);

        if (nodeToMove.find(child) == nodeToMove.end()) {
            nodeToMove.insert({child, move});
        }

    }
    while(iterations > 0){
        
            if(iterations % 500 == 0){
                std::cout << "iteration: " << iterations << std::endl;
            }

            MCSTNode* selectChild = select(node, board);


            MCSTNode* expandChild = expand(selectChild, board);
            std::pair<MCSTNode*, std::optional<chess::Color>> rolloutChild = rollout(expandChild, board);
            rollback(rolloutChild.first, rolloutChild.second);

            iterations -= 1;
        
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

void setupBoard(MCSTNode* node, chess::Board& board){
    board.set_fen(node->_fen);
    //board.turn = node->_turn;
}