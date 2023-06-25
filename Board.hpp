#ifndef HEXXAGON_BOARD_HPP
#define HEXXAGON_BOARD_HPP

#pragma once

#include <vector>
#include <iostream>
#include <set>

/**
 * \brief Struct that represent Fields on board
 * Struktra reprezentuje pole na planszy, każde pole może być albo
 * czerwone(zajęte przez gracza czerwonego) analogicznie niebieskie
 * free czyli wolne(można tam postawić swój pionek) albo out
 * out oancza że pole nie należy do planszy. Ponieważ plansza jest heksagonem
 * i przedstawiłem ją w postaci tablicy, niektóre pola muszą być nie brane pod uwagę
 */
struct Field {
    size_t x, y;
    int index;
    bool red, blue, out, free;

    Field(size_t x, size_t y, bool red, bool blue, bool out, int i);

    void changeToRed();
    void changeToBlue();
    void changeToOut();
    void changeToFree();
};

/**
 * \brief Class Board holds every necessary thing to calculate next move on board and
 * board itself
 * Jest to jednostka logiczna, która zawiera planszę (kwadratowy vector ze structurami Field)
 * oraz metody wyliczające możliwe ruchy na podstawie tego wektora oraz na bieżąco aktualizuje informacje
 * o stanie gry aby potem móc łatwiej zapisać i odczytać stan gry
 * */
class Board {
public:
    std::vector<std::vector<Field>> board;
    std::set<Field> positionRed;
    std::set<Field> positionBlue;
    size_t dim;
    size_t redScore;
    size_t blueScore;
    size_t turn;
    size_t multi;

    explicit Board(size_t dim);
    ~Board();
    void prepareBoard();
    void setFields();
    std::set<Field> positionsRed();
    std::set<Field> positionsBlue();
    std::set<Field> possibleMovesOne(const Field &field);
    std::set<Field> possibleMovesTwo(const Field &Field);
    std::set<Field> possiblePositionsOne(const Field &field);
    std::set<Field> possiblePositionsTwo(const Field &field);
    std::set<Field> oneMove(Field &field, bool red);
    std::set<Field> twoMove(Field &field, Field &remove, bool red);
};
#endif //HEXXAGON_BOARD_HPP
