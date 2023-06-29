#ifndef HEXXAGON_GAMEBOARD_HPP
#define HEXXAGON_GAMEBOARD_HPP
#pragma once

#include "/Users/michallejza/CLionProjects/Hexxagon/Board/Board.cpp"
#include "/Users/michallejza/CLionProjects/Hexxagon/Computer/Computer.cpp"
#include <utility>
#include <vector>
#include <iostream>
#include <map>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

/**
 * \brief class GameBoard
 * Has all necessary SFML objects, buttons are field buttons
 */
class GameBoard
{
public:

    std::map<int, sf::Sprite> buttons;
    sf::Sprite button, backGround, end;
    sf::Text redScore, blueScore, endText;
    sf::Font font;
    sf::Texture textures[12];
    Field change;
    Board board;
    std::string filename;
    bool multiPlayer;
    int screenDimX{}, screenDimY{}, turn{};

    explicit GameBoard(const Board &b, Field change, int screenDimX, int screenDimY);
    ~GameBoard();
    void saveGame();
    void reset();
    void loadTextures();
    void loadButtons();
    void changeTexture(int index, int t);
    bool containsExit(sf::RenderWindow &window) const;
    bool contains(sf::RenderWindow &window, int index);
    void draw(sf::RenderWindow &window);
    void resetTemporaryTexture();
    void clickedOnTaken(size_t i, size_t j, int index, bool red);
    void clickedOnOne(size_t i, size_t j, int index, bool red);
    void clickedOnTwo(size_t i, size_t j, int index, bool red);
};
void playGame(sf::RenderWindow &window, GameBoard &gameBoard, bool &menu, bool &play);

#endif //HEXXAGON_GAMEBOARD_HPP
