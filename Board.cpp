#include "/Users/michallejza/CLionProjects/Hexxagon/Board/Board.hpp"
#pragma once

/**
 * \brief constructor for Field struct, each Field has coordinates, index and a bool value
 *
 * @param x  // coordinate x
 * @param y // coordinate y
 * @param red // bool value, true if field is blue
 * @param blue // bool value, true if field is red
 * @param out // bool value, true if field is out of board
 * @param i // index of a field
 */
Field::Field(size_t x, size_t y, bool red, bool blue, bool out, int i) {
    this -> x = x;
    this -> y = y;
    this -> red = red;
    this -> blue = blue;
    this -> out = out;
    this -> free = true;
    this -> index = i++;
}

void Field::changeToRed() {
    this -> red = true;
    this -> blue = false;
    this -> free = false;
    this -> out = false;
}

void Field::changeToBlue() {
    this -> red = false;
    this -> blue = true;
    this -> free = false;
    this -> out = false;
}

void Field::changeToOut() {
    this -> red = false;
    this -> blue = false;
    this -> free = false;
    this -> out = true;
}

void Field::changeToFree() {
    this -> red = false;
    this -> blue = false;
    this -> free = true;
    this -> out = false;
}

/*
 *  0  0  0  0  0  X  X  X  X
 *  0  0  0  0  0  0  X  X  X
 *  0  0  0  0  0  0  0  X  X
 *  0  0  0  0  0  0  0  0  X
 *  0  0  0  0  0  0  0  0  0
 *  0  0  0  0  0  0  0  0  X
 *  0  0  0  0  0  0  0  X  X
 *  0  0  0  0  0  0  X  X  X
 *  0  0  0  0  0  X  X  X  X
 */

/**
 * \brief constructor for Board class
 * @param dim
 */
Board::Board(size_t dim) {
    this -> dim = dim;
    this -> blueScore = 3;
    this -> redScore = 3;
    this -> turn = 0;
    this -> multi = 0;
    this -> board = std::vector<std::vector<Field>>(dim, std::vector<Field>(dim, Field(0,0, false, false,false, 0)));
    prepareBoard();
    this -> positionRed = positionsRed();
    this -> positionBlue = positionsBlue();
}

Board::~Board() = default;

/**
 * \brief Indexujemy elementy vectora i nadajemy im współrzedne zgodnie ze współrzędnymi
 * vectora
 */
void Board::prepareBoard() {
    static int index = 0;
    for (size_t i = 0; i < dim; i++)
        for (size_t j = 0; j < dim; j++) {
            this -> board[i][j].x = i;
            this -> board[i][j].y = j;
            this -> board[i][j].index = index++;
        }
    setFields();
}

/**
 * Kontynuacja poprzedniej metody, ustawiamy wartości logiczne wszystkich pól tak aby były zgodne
 * z początkowym stanem gry hexxagon
 */
void Board::setFields() {
    for (size_t i = 0; i < dim; i++)
        for(size_t j = 0; j < dim; j++)
            this -> board[i][j].changeToFree();

    for (size_t i = 0; i < dim; i++) {
        if (i <= dim / 2)
            for (size_t j = i + dim / 2 + 1; j < dim; j++)
                this -> board[i][j].changeToOut();
        else
            for (size_t j = dim + dim / 2 - i; j < dim; j++)
                this -> board[i][j].changeToOut();
    }
    this -> board[3][3].changeToOut();
    this -> board[4][5].changeToOut();
    this -> board[5][3].changeToOut();
    this -> board[0][0].changeToRed();
    this -> board[8][0].changeToRed();
    this -> board[4][8].changeToRed();
    this -> board[0][4].changeToBlue();
    this -> board[8][4].changeToBlue();
    this -> board[4][0].changeToBlue();
}

bool operator < (const Field &f1, const Field &f2) {
    return f1.index < f2.index;
}

/**
 * \brief znajdujemy pozycje czerwonych
 * @return
 */
std::set<Field> Board::positionsRed() {
    std::set<Field> positions = std::set<Field>();
    for (size_t i = 0; i < dim; i++)
        for (size_t j = 0; j < dim; j++)
            if (board[i][j].red)
                positions.insert(board[i][j]);
    return positions;
}

/**
 * znajdujemy pozycje niebieskich
 * @return
 */
std::set<Field> Board::positionsBlue() {
    std::set<Field> positions = std::set<Field>();
    for (size_t i = 0; i < dim; i++)
        for (size_t j = 0; j < dim; j++)
            if (board[i][j].blue)
                positions.insert(board[i][j]);
    return positions;
}

/**
 * Dla danego pola metoda zwraca zbior tych pól które sa oddalone
 * o jedną pozycję. Nie bierze pod uwagę ich stanu(czy jest red free itd)
 * @param field
 * @return
 */
std::set<Field> Board::possibleMovesOne(const Field &field) {
    std::set<Field> possible = std::set<Field>();

    size_t up = (field.x - 1 >= 0 ? field.x - 1 : dim);
    size_t down = (field.x + 1 < dim ? field.x + 1 : dim);
    size_t left = (field.y - 1 >= 0 ? field.y - 1 : dim);
    size_t right = (field.y + 1 < dim ? field.y + 1 : dim);

    if (up < dim)
        possible.insert(board[up][field.y]);
    if (down < dim)
        possible.insert(board[down][field.y]);
    if (left < dim)
        possible.insert(board[field.x][left]);
    if (right < dim)
        possible.insert(board[field.x][right]);
    if (field.x <= dim / 2 && up < dim && left < dim)
        possible.insert(board[up][left]);
    if (field.x == dim / 2 && down < dim && left < dim)
        possible.insert(board[down][left]);
    if (field.x < dim / 2 && down < dim && right < dim)
        possible.insert(board[down][right]);
    if (field.x > dim / 2 && up < dim && right < dim)
        possible.insert(board[up][right]);
    if (field.x > dim / 2 && down < dim && left < dim)
        possible.insert(board[down][left]);
    return possible;
}

/**
 * Dla danego pola metoda zwraca zbior tych pól które sa oddalone
 * o dwie pozycje. Nie bierze pod uwagę ich stanu(czy jest red free itd)
 * @param field
 * @return
 */
std::set<Field> Board::possibleMovesTwo(const Field &field)
{
    std::set<Field> possible = std::set<Field>();

    size_t upTwo = (field.x - 2 >= 0 ? field.x - 2 : dim);
    size_t downTwo = (field.x + 2 < dim ? field.x + 2 : dim);
    size_t leftTwo = (field.y - 2 >= 0 ? field.y - 2 : dim);
    size_t rightTwo = (field.y + 2 < dim ? field.y + 2 : dim);

    size_t up = (field.x - 1 >= 0 ? field.x - 1 : dim);
    size_t down = (field.x + 1 < dim ? field.x + 1 : dim);
    size_t left = (field.y - 1 >= 0 ? field.y - 1 : dim);
    size_t right = (field.y + 1 < dim ? field.y + 1 : dim);

    if (upTwo < dim)
        possible.insert(board[upTwo][field.y]);
    if (downTwo < dim)
        possible.insert(board[downTwo][field.y]);
    if (leftTwo < dim)
        possible.insert(board[field.x][leftTwo]);
    if (rightTwo < dim)
        possible.insert(board[field.x][rightTwo]);
    if (field.x <= 4) {
        if (up < dim && right < dim)
            possible.insert(board[up][right]);
        if (leftTwo < dim && up < dim)
            possible.insert(board[up][leftTwo]);
        if (left < dim && upTwo < dim)
            possible.insert(board[upTwo][left]);
        if (leftTwo < dim && upTwo < dim)
            possible.insert(board[upTwo][leftTwo]);
    }
    if (field.x >= 4) {
        if(down < dim && right < dim)
            possible.insert(board[down][right]);
        if(down < dim && leftTwo < dim)
            possible.insert(board[down][leftTwo]);
        if(downTwo < dim && left < dim)
            possible.insert(board[downTwo][left]);
        if(downTwo < dim && leftTwo < dim)
            possible.insert(board[downTwo][leftTwo]);
    }
    if (field.x == 3) {
        if (down < dim && left < dim)
            possible.insert(board[down][left]);
        if (down < dim && rightTwo < dim)
            possible.insert(board[down][rightTwo]);
        if (downTwo < dim && right < dim)
            possible.insert(board[downTwo][right]);
        if (downTwo < dim && left < dim)
            possible.insert(board[downTwo][left]);
    }
    if (field.x == 5) {
        if (up < dim && left < dim)
            possible.insert(board[up][left]);
        if (up < dim && rightTwo < dim)
            possible.insert(board[up][rightTwo]);
        if (upTwo < dim && right < dim)
            possible.insert(board[upTwo][right]);
        if (upTwo < dim && left < dim)
            possible.insert(board[upTwo][left]);
    }
    if (field.x < 3) {
        if (down < dim && left < dim)
            possible.insert(board[down][left]);
        if (down < dim && rightTwo < dim)
            possible.insert(board[down][rightTwo]);
        if (downTwo < dim && right < dim)
            possible.insert(board[downTwo][right]);
        if (downTwo < dim && rightTwo < dim)
            possible.insert(board[downTwo][rightTwo]);
    }
    if (field.x > 5) {
        if (up < dim && left < dim)
            possible.insert(board[up][left]);
        if (up < dim && rightTwo < dim)
            possible.insert(board[up][rightTwo]);
        if (upTwo < dim && right < dim)
            possible.insert(board[upTwo][right]);
        if (upTwo < dim && rightTwo < dim)
            possible.insert(board[upTwo][rightTwo]);
    }
    return possible;
}

/**
 * Dla danego pola metoda zwraca zbior tych pól które sa oddalone
 * o jedną pozycję. Ale tym razem bierze pod uwagę ich stany(czy jest red free itd)
 * @param f
 * @return
 */
std::set<Field> Board::possiblePositionsOne(const Field &f) {
    std::set<Field> possible = std::set<Field>();
    std::set<Field> positions = possibleMovesOne(f);

    for (auto field : positions)
        if (field.free && !field.out && (f.blue ? !field.red : !field.blue))
            possible.insert(field);

    positions.clear();
    return possible;
}

/**
 *  * Dla danego pola metoda zwraca zbior tych pól które sa oddalone
 * o dwie pozycję. Ale tym razem bierze pod uwagę ich stany(czy jest red free itd)
 * @param f
 * @return
 */
std::set<Field> Board::possiblePositionsTwo(const Field &f) {
    std::set<Field> possible = std::set<Field>();
    std::set<Field> positions = possibleMovesTwo(f);

    for (auto field : positions)
        if (field.free && !field.out && (f.blue ? !field.red : !field.blue))
            possible.insert(field);

    positions.clear();
    return possible;
}

/**
 * dla danego pola, jesli wykonamy ruch o jeden to aktualizujemy stan gry
 * @param field
 * @param red
 * @return
 */
std::set<Field> Board::oneMove(Field &field, bool red) {
    std::set<Field> toChange = std::set<Field>();
    std::set<Field> possible = possibleMovesOne(field);

    red ?  positionRed.insert(field) : positionBlue.insert(field);
    red ? board[field.x][field.y].changeToRed() : board[field.x][field.y].changeToBlue();
    red ?  redScore++ : blueScore++;

    for (auto f : possible) {
        if (f.blue && red) {
            toChange.insert(f);
            positionRed.insert(f);
            positionBlue.erase(f);
            board[f.x][f.y].changeToRed();
            redScore++;
            blueScore--;
        }
        if (f.red && !red) {
            toChange.insert(f);
            positionBlue.insert(f);
            positionRed.erase(f);
            redScore--;
            blueScore++;
            board[f.x][f.y].changeToBlue();
        }
    }
    possible.clear();
    return toChange;
}
/**
 *  dla danego pola, jesli wykonamy ruch o dwa to aktualizujemy stan gry
 * @param field
 * @param remove
 * @param red
 * @return
 */
std::set<Field> Board::twoMove(Field &field, Field &remove, bool red) {
    std::set<Field> toChange = std::set<Field>();
    std::set<Field> possible = possibleMovesOne(field);

    red ? positionRed.insert(field) :  positionBlue.insert(field);
    red ? board[field.x][field.y].changeToRed() : board[field.x][field.y].changeToBlue();
    red ? positionRed.erase(remove) : positionBlue.erase(remove);
    board[remove.x][remove.y].changeToFree();

    for (auto f : possible) {
        if (f.blue && red) {
            toChange.insert(f);
            positionRed.insert(f);
            positionBlue.erase(f);
            redScore++;
            blueScore--;
            board[f.x][f.y].changeToRed();
        }
        if (f.red && !red) {
            toChange.insert(f);
            positionBlue.insert(f);
            positionRed.erase(f);
            blueScore++;
            redScore--;
            board[f.x][f.y].changeToBlue();
        }
    }
    possible.clear();
    return toChange;
}