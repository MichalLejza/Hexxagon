#ifndef HEXXAGON_COMPUTER_HPP
#define HEXXAGON_COMPUTER_HPP

#pragma once

#include "../Board/Board.cpp"
#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <random>
#include <set>
#include <memory>

// blue is computer move
// red is users move
struct Node {
    Field field;
    std::set<Field> oneMove;
    std::set<Field> twoMove;
    size_t scoreRed;
    size_t scoreBlue;
};

struct PromisingField {
    bool twoMove;
    Field original;
    Field move;
    int value;
};

Node *createNewNode(Field &field, Board &board);
int evaluate(Node *check);
int minimax(const std::shared_ptr<Board>& board, Node *currentNode, int depth, bool maximizingPlayer);
bool operator < (PromisingField &f1, PromisingField &f2);
PromisingField *createPromisingField(bool twoMove, Field original, Field move, int value);
PromisingField *bestChoice(std::set<PromisingField *> &fields);
Field testMiniMax(Board &board, bool &twoMove, Field &toChange);


#endif //HEXXAGON_COMPUTER_HPP
