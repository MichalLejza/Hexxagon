#include "/Users/michallejza/CLionProjects/Hexxagon/Computer/Computer.hpp"
#pragma once

/**
 * \brief Struct to hold all necessary information about game
 * Aby przechowywać dane o planszy, aktualnym wyniku i możliwych posunięciach
 * stworzyłem strukturę króra to wszystko przechowuje
 * @param field
 * @param board
 * @return New node
 */
Node *createNewNode(Field &field, Board &board) {
    Node *newNode = (Node *)(malloc(sizeof(Node)));
    newNode -> field = field;
    newNode -> oneMove = board.possiblePositionsOne(field);
    newNode -> twoMove = board.possiblePositionsTwo(field);
    newNode -> scoreRed = board.redScore;
    newNode -> scoreBlue = board.blueScore;
    return newNode;
}

/**
 * \brief Funkcja zwracająca wynik dla danego stanu gry do algorytmu MiniMax
 * @param check
 * @return Funkcja zwraca róznice wyniku niebieskich i czerwonych
 */
int evaluate(Node *check) {
    return check -> scoreBlue - check -> scoreRed;
}
/**
 * \brief MiniMax Algorithm
 * Algorytm Minimax, rekurencyjnie tworzy kopie gry, i symuluje rozgrywkę robiąc na zmianę
 * kolejne ruchy. Aby móc swobodnie tworzyć kopie planszy użyłem shared_ptr aby bezpiecznie móc
 * robić kopie planszy, potem je usuwać bez naruszenia stanu oryginalnej planszy
 * @param board Plansza
 * @param currentNode Node dla którego wyliczamy następne ruchy
 * @param depth głębokość do której schodzi algorytm
 * @param maximizingPlayer true jeśli liczymy ruch komputera
 * @return wartość najbardziej optymalnego ruchu
 */
int minimax(const std::shared_ptr<Board>& board, Node *currentNode, int depth, bool maximizingPlayer) {
    // jesli ten ruch kończy rozgrywkę zwracamy bardzo duzą liczbę
    if (currentNode -> oneMove.empty() && currentNode -> twoMove.empty())
        return (maximizingPlayer ? 1000000 : -1000000);
    // jeśli doszlismy do końca drzewa rekurencyjnego
    if (depth == 0)
        return evaluate(currentNode);

    int bestValue;
    // wyliczamy wszystkie ruchy niebieskich
    if (maximizingPlayer) {
        bestValue = -100000;
        for(auto f : currentNode -> oneMove) {
            std::shared_ptr copy = std::make_shared<Board>(*board);
            std::set<Field> changeFields = copy -> oneMove(f, true);
            Node *nextNode = createNewNode(f, *copy);

            int value = minimax(board, nextNode, depth - 1, false);
            bestValue = std::max(value, bestValue);

            changeFields.clear();
            copy.reset();
            delete nextNode;
        }
        for(auto f : currentNode -> twoMove) {
            std::shared_ptr copy = std::make_shared<Board>(*board);
            copy -> board[currentNode -> field.x][currentNode -> field.y].changeToFree();
            std::set<Field> chaneFields = copy -> oneMove(f, true);
            Node *nextNode = createNewNode(f, *copy);

            int value = minimax(board, nextNode, depth - 1, false);
            bestValue = std::max(value, bestValue);

            chaneFields.clear();
            copy.reset();
            delete nextNode;
        }
    }
    // wyliczamy wszystkie ruchy czerwonych
    else {
        bestValue = 100000;
        for(auto f : currentNode -> oneMove) {
            std::shared_ptr copy = std::make_shared<Board>(*board);
            std::set<Field> chaneFields = copy -> oneMove(f, false);
            Node *nextNode = createNewNode(f, *copy);

            int value = minimax(board, nextNode, depth - 1, true);
            bestValue = std::min(value, bestValue);

            chaneFields.clear();
            copy.reset();
            delete nextNode;
        }
        for(auto f : currentNode -> twoMove) {
            std::shared_ptr copy = std::make_shared<Board>(*board);
            copy -> board[currentNode -> field.x][currentNode -> field.y].changeToFree();
            std::set<Field> chaneFields = copy -> oneMove(f, false);
            Node *nextNode = createNewNode(f, *copy);
            int value = minimax(board, nextNode, depth - 1, false);
            bestValue = std::max(value, bestValue);

            chaneFields.clear();
            copy.reset();
            delete nextNode;
        }
    }
    return bestValue;
}
/**
 * \brief przeciążenie operatora < dla std::set
 * @param f1
 * @param f2
 * @return
 */
bool operator < (PromisingField &f1, PromisingField &f2) {
    return f1.value < f2.value;
}

PromisingField *createPromisingField(bool twoMove, Field original, Field move, int value) {
    auto *newField = (PromisingField *)(malloc(sizeof(PromisingField)));
    newField -> twoMove = twoMove;
    newField -> original = original;
    newField -> move = move;
    newField -> value = value;
    return newField;
}

/**
 * \brief funkcja spośród wszystkich ruchów wybiera najlepsze
 * i następnie spośród tych najlepszych, wybiera jeden losowo
 * @param fields
 * @return
 */
PromisingField *bestChoice(std::set<PromisingField *> &fields) {
    std::set<PromisingField *> possible;
    int max = -100000;
    for(auto f : fields)
        if(f -> value > max)
            max = f -> value;

    for(auto f : fields)
        if(f -> value == max)
            possible.insert(f);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, possible.size() - 1);

    auto it = possible.begin();
    std::advance(it, dis(gen));
    return *it;
}

/**
 * \brief główna funckja aktywująca MiniMax i zwracająca losowe, najlepsze pole
 * @param board
 * @param twoMove
 * @param toChange
 * @return
 */
Field testMiniMax(Board &board, bool &twoMove, Field &toChange) {
    std::set<PromisingField *> possibleFields;
    for(auto f : board.positionBlue) {
        std::set<Field> possibleMovesOne = board.possiblePositionsOne(f);
        for(auto field : possibleMovesOne) {
            std::shared_ptr<Board> copy = std::make_shared<Board>(board);
            copy -> oneMove(field, false);
            Node *newNode = createNewNode(field, *copy);
            int value = minimax(copy, newNode, 1, true);
            PromisingField *newField = createPromisingField(false, f, field, value);
            possibleFields.insert(newField);
        }
        possibleMovesOne.clear();

        std::set<Field> possibleMovesTwo = board.possiblePositionsTwo(f);
        for(auto field : possibleMovesTwo) {
            std::shared_ptr<Board> copy = std::make_shared<Board>(board);
            copy -> twoMove(field, f, false);
            Node *newNode = createNewNode(field, *copy);
            int value = minimax(copy, newNode, 1, true);
            PromisingField *newField = createPromisingField(true, f, field, value);
            possibleFields.insert(newField);
        }
        possibleMovesTwo.clear();
    }
    PromisingField *promisingField;
    if(!possibleFields.empty())
        promisingField = bestChoice(possibleFields);
    else
        return {0,0, false, false, false, -1};
    twoMove = false;
    if(promisingField -> twoMove) {
        twoMove = true;
        toChange = promisingField->original;
    }
    possibleFields.clear();
    return promisingField->move;
}