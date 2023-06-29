#include "/Users/michallejza/CLionProjects/Hexxagon/GameBoard/GameBoard.hpp"
#include <utility>
#include <fstream>
#pragma once

/**
 * \brief constructor of GameBoard class
 * @param b Board object
 * @param change Field that will be changed to free when we move two moves ahead
 * @param screenDimX
 * @param screenDimY
 */
GameBoard::GameBoard(const Board &b, Field change, int screenDimX, int screenDimY) : board(b), change(change) {
    this -> filename = "";
    this -> multiPlayer = false;
    this -> turn = 0;
    this -> end = sf::Sprite();
    this -> endText = sf::Text();
    this -> buttons = std::map<int, sf::Sprite>();
    this -> button = sf::Sprite();
    this -> backGround = sf::Sprite();
    this -> redScore = sf::Text();
    this -> blueScore = sf::Text();
    this -> screenDimX = screenDimX;
    this -> screenDimY = screenDimY;
    this -> font = sf::Font();
    loadTextures();
    loadButtons();
}

/**
 * \brief method to save game to a file. Name of a file is given by user
 * po kolei zapisujemy wszystkie dane z gry, stan tablicy, wynik itp.
 */
void GameBoard::saveGame() {
    std::string directory = "/Users/michallejza/Desktop/SavedGames";
    std::ofstream file(directory + "/" + filename + ".txt");
    if (!file) {
        std::cout << "Failed to create the file." << std::endl;
        return;
    }
    file << board.redScore << " " << board.blueScore << std::endl;
    file << multiPlayer << std::endl;
    file << turn << std::endl;
    for(int i = 0; i < board.dim; i++) {
        for(int j = 0; j < board.dim; j++) {
            if(board.board[i][j].red)
                file << 1 << " ";
            if(board.board[i][j].free)
                file << 0 << " ";
            if(board.board[i][j].blue)
                file << 2 << " ";
            if(board.board[i][j].out)
                file << 4 << " ";
        }
        file << std::endl;
    }
    file.close();
}

/**
 * \brief method resets all gameBoard value to original values
 */
void GameBoard::reset() {
    saveGame();
    board.setFields();
    this -> filename = "";
    this -> turn = 0;
    this -> multiPlayer = false;
    this -> board.blueScore = 3;
    this -> board.redScore = 3;
    this -> board.positionBlue = this -> board.positionsBlue();
    this -> board.positionRed = this -> board.positionsRed();

    for(const auto& f : buttons)
        changeTexture(f.first, 0);

    for(const auto& f: buttons) {
        if(board.board[f.first / 9][f.first % 9].red)
            changeTexture(f.first, 5);
        if(board.board[f.first / 9][f.first % 9].blue)
            changeTexture(f.first, 1);
    }
}

/**
 * \brief method loads all necessary textures
 */
void GameBoard::loadTextures() {
    textures[0].loadFromFile("/Users/michallejza/Desktop/Tekstury/Empty.png"); // empty field
    textures[1].loadFromFile("/Users/michallejza/Desktop/Tekstury/TakenBlue.png");  // taken blue
    textures[2].loadFromFile("/Users/michallejza/Desktop/Tekstury/OneBlue.png");  // one move blue
    textures[3].loadFromFile("/Users/michallejza/Desktop/Tekstury/TwoBlue.png"); // two move blue
    textures[4].loadFromFile("/Users/michallejza/Desktop/Tekstury/ClickedBlue.png"); // clicked blue
    textures[5].loadFromFile("/Users/michallejza/Desktop/Tekstury/TakenRed.png"); // taken red
    textures[6].loadFromFile("/Users/michallejza/Desktop/Tekstury/OneRed.png");  // one move red
    textures[7].loadFromFile("/Users/michallejza/Desktop/Tekstury/TwoRed.png");  // two move red
    textures[8].loadFromFile("/Users/michallejza/Desktop/Tekstury/ClickedRed.png");  // clicked red;
    textures[9].loadFromFile("/Users/michallejza/Desktop/Tekstury/backGround.png");  // background
    textures[10].loadFromFile("/Users/michallejza/Desktop/Tekstury/quitandsave.jpg"); // save and quit button
    textures[11].loadFromFile("/Users/michallejza/Desktop/Tekstury/button.png");
    if(!font.loadFromFile("/Users/michallejza/Desktop/Tekstury/font.ttf.otf")) {}
}

/**
 * \brief method adjust all SFML objects accordingly to screen dimensions
 */
void GameBoard::loadButtons() {
    redScore.setFont(font);
    redScore.setFillColor(sf::Color::Red);
    redScore.setCharacterSize(screenDimY / 20);
    redScore.setPosition((float)screenDimX / 12.f, (float)screenDimY / 12.f);

    blueScore.setFont(font);
    blueScore.setFillColor(sf::Color::Blue);
    blueScore.setCharacterSize(screenDimY / 20);
    blueScore.setPosition((float)screenDimX / 1.36f, (float)screenDimY / 12.f);

    backGround.setTexture(textures[9]);
    backGround.setScale((float)screenDimX / (float)textures[9].getSize().x, (float)screenDimY / (float)textures[9].getSize().y);

    button.setTexture(textures[10]);
    button.setPosition((float)screenDimX / 17.f, (float)screenDimY / 1.14f);

    end.setTexture(textures[11]);
    end.setPosition((float)screenDimX / 3.4f, (float)screenDimY / 2.4f);

    endText.setFont(font);
    endText.setString("End Of Game!");
    endText.setFillColor(sf::Color::Black);
    endText.setCharacterSize(65);
    endText.setPosition((float)screenDimX / 2.83f, (float)screenDimY / 2.18f);
    int x = 380;
    int y = 800;
    for(int i = 0; i < board.dim; i++) {
        for(int j = 0; j < board.dim; j++) {
            if(!board.board[i][j].out) {
                sf::Sprite sprite;
                if (board.board[i][j].red)
                    sprite.setTexture(textures[5]);
                if (board.board[i][j].blue)
                    sprite.setTexture(textures[1]);
                if (board.board[i][j].free)
                    sprite.setTexture(textures[0]);
                sprite.setScale(0.36, 0.36);
                sprite.setTextureRect(sf::IntRect(0, 0, 300, 300));
                sprite.setPosition((float)x, (float)y - (float)j * 108);
                buttons.emplace(board.board[i][j].index, sprite);
            }
        }
        x += 108;
        (i < 4 ? y += 54 : y -= 54);
    }
}

/**
 * \brief Method changes texture of Sprite
 * @param index index of Sprite
 * @param t index of texture
 */
void GameBoard::changeTexture(int index, int t) {
    buttons.at(index).setTexture(textures[t]);
}

/**
 * \ method checks if we clicked exit button
 * @param window
 * @return true if we clicked exit
 */
bool GameBoard::containsExit(sf::RenderWindow &window) const {
    return button.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)));
}

/**
 * @param window
 * @param index index of a Sprite
 * @return true if we clicked on a Sprite representing field
 */
bool GameBoard::contains(sf::RenderWindow &window, int index) {
    return buttons.at(index).getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)));
}

/**
 * \brief method draws all SFML object to a window
 * @param window
 */
void GameBoard::draw(sf::RenderWindow &window) {
    window.clear();
    redScore.setString("Red: " + std::to_string(board.redScore));
    blueScore.setString("Blue: " + std::to_string(board.blueScore));
    window.draw(backGround);
    window.draw(redScore);
    window.draw(blueScore);
    window.draw(button);
    for (int i = 0; i < board.dim; i++)
        for (int j = 0; j < board.dim; j++)
            if (!board.board[i][j].out)
                window.draw(buttons.at(board.board[i][j].index));
}

/**
 * \brief method resets all textures to a free texture
 * Jesli kliknęliśmy jakieś pole, podświetlają nam sie pola na ktore mozemy
 * sie udać, po wykanwniu ruchu/kliknięciu innego pola, musimy zmienić tekstury
 * tych spritów na oryginalny
 */
void GameBoard::resetTemporaryTexture() {
    for(const auto& f : buttons) {
        if (f.second.getTexture() == &textures[2] || f.second.getTexture() == &textures[3] ||
            f.second.getTexture() == &textures[6] || f.second.getTexture() == &textures[7])
            changeTexture(f.first, 0);
        if (f.second.getTexture() == &textures[4])
            changeTexture(f.first, 1);
        if (f.second.getTexture() == &textures[8])
            changeTexture(f.first, 5);
    }
}

/**
 * \brief if we clicked on a field that was taken, all neighbouring fileds
 * will change texture according to clicked button colour
 * @param i
 * @param j
 * @param index
 * @param red true if red is moving
 */
void GameBoard::clickedOnTaken(size_t i, size_t j, int index, bool red) {
    resetTemporaryTexture();
    std::set<Field> one = board.possiblePositionsOne(board.board[i][j]);
    std::set<Field> two = board.possiblePositionsTwo(board.board[i][j]);
    for (auto field : one)
        changeTexture(field.index, (red ? 6 : 2));
    for (auto field : two)
        changeTexture(field.index, (red ? 7 : 3));
    changeTexture(index, (red ? 8 : 4));
    one.clear();
    two.clear();
}

/**
 * \brief if we clicked on a highlighted field that is next to a taken field
 * we change board according to the color
 * @param i
 * @param j
 * @param index
 * @param red
 */
void GameBoard::clickedOnOne(size_t i, size_t j, int index, bool red) {
    std::set<Field> toChange;
    toChange = (red ? board.oneMove(board.board[i][j], true) : board.oneMove(board.board[i][j], false));
    changeTexture(index, (red ? 5 : 1));
    for (auto f : toChange)
        changeTexture(f.index, (red ? 5 : 1));
    resetTemporaryTexture();
}

/**
 * \brief if we clicked on a highlighted field that is two fields away from original
 * @param i
 * @param j
 * @param index
 * @param red
 */
void GameBoard::clickedOnTwo(size_t i, size_t j, int index, bool red) {
    std::set<Field> toChange;
    toChange = (red ? board.twoMove(board.board[i][j], change, true) : board.twoMove(board.board[i][j], change, false));
    changeTexture(index,(red ? 5 : 1));
    changeTexture(change.index,0);
    for (auto f : toChange)
        changeTexture(f.index, (red ? 5 : 1));
    resetTemporaryTexture();
}

GameBoard::~GameBoard() = default;
/**
 * \brief method checks if either player has moves, if not, the game ends
 * @param gameBoard
 * @return true if at least one player has no moves
 */
bool endOfGame(GameBoard &gameBoard)
{
    std::set<Field> possibleRed = std::set<Field>();
    std::set<Field> possibleBlue = std::set<Field>();

    for (auto f : gameBoard.board.positionRed)
    {
        std::set<Field> oneMove = gameBoard.board.possiblePositionsOne(f);
        for(auto F : oneMove)
            possibleRed.emplace(F);
        std::set<Field> twoMove = gameBoard.board.possiblePositionsTwo(f);
        for(auto F : twoMove)
            possibleRed.emplace(F);
    }
    for (auto f : gameBoard.board.positionBlue)
    {
        std::set<Field> oneMove = gameBoard.board.possiblePositionsOne(f);
        for(auto F : oneMove)
            possibleBlue.emplace(F);
        std::set<Field> twoMove = gameBoard.board.possiblePositionsTwo(f);
        for(auto F : twoMove)
            possibleBlue.emplace(F);
    }

    return possibleRed.empty() || possibleBlue.empty();
}

/**
 * method to display GameBoard, with board, scores, etc
 * @param window
 * @param gameBoard
 * @param menu change to true if we clicked quit
 * @param play change to false if we clicked quit
 */
void playGame(sf::RenderWindow &window, GameBoard &gameBoard, bool &menu, bool &play) {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (gameBoard.containsExit(window)) {
                gameBoard.reset();
                menu = true;
                play = false;
                return;
            }
            for (const auto &f: gameBoard.buttons) {
                if (gameBoard.contains(window, f.first)) {
                    if (gameBoard.turn % 2 == 0 && gameBoard.buttons.at(f.first).getTexture() == &gameBoard.textures[5]) {
                        gameBoard.change = gameBoard.board.board[f.first / 9][f.first % 9];
                        gameBoard.clickedOnTaken(f.first / 9, f.first % 9, f.first, true);
                    }
                    if (gameBoard.turn % 2 == 0 && gameBoard.buttons.at(f.first).getTexture() == &gameBoard.textures[6]) {
                        gameBoard.clickedOnOne(f.first / 9, f.first % 9, f.first, true);
                        gameBoard.turn++;
                    }
                    if (gameBoard.turn % 2 == 0 && gameBoard.buttons.at(f.first).getTexture() == &gameBoard.textures[7]) {
                        gameBoard.clickedOnTwo(f.first / 9, f.first % 9, f.first, true);
                        gameBoard.turn++;
                    }
                    if (!gameBoard.multiPlayer && gameBoard.turn % 2 != 0 && gameBoard.buttons.at(f.first).getTexture() == &gameBoard.textures[1]) {
                        gameBoard.change = gameBoard.board.board[f.first / 9][f.first % 9];
                        gameBoard.clickedOnTaken(f.first / 9, f.first % 9, f.first, false);
                    }
                    if (!gameBoard.multiPlayer && gameBoard.turn % 2 != 0 && gameBoard.buttons.at(f.first).getTexture() == &gameBoard.textures[2]) {
                        gameBoard.clickedOnOne(f.first / 9, f.first % 9, f.first, false);
                        gameBoard.turn++;
                    }
                    if (!gameBoard.multiPlayer && gameBoard.turn % 2 != 0 && gameBoard.buttons.at(f.first).getTexture() == &gameBoard.textures[3]) {
                        gameBoard.clickedOnTwo(f.first / 9, f.first % 9, f.first, false);
                        gameBoard.turn++;
                    }
                    if(gameBoard.multiPlayer && gameBoard.turn % 2 != 0) {
                        bool twoMove = false;
                        Field blueMove = testMiniMax(gameBoard.board, twoMove, gameBoard.change);
                        if (blueMove.index >= 0) {
                            if (!twoMove)
                                gameBoard.clickedOnOne(blueMove.x, blueMove.y, blueMove.index, false);
                            else
                                gameBoard.clickedOnTwo(blueMove.x, blueMove.y, blueMove.index, false);
                        }
                        gameBoard.turn++;
                    }
                }
            }
        }
    }
    gameBoard.draw(window);
    if(endOfGame(gameBoard))
    {
        window.draw(gameBoard.end);
        window.draw(gameBoard.endText);
    }
    window.display();
}