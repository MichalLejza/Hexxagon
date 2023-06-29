#ifndef HEXXAGON_SELECTMODE_HPP
#define HEXXAGON_SELECTMODE_HPP

#pragma once

#include "/Users/michallejza/CLionProjects/Hexxagon/Board/Board.cpp"
#include "/Users/michallejza/CLionProjects/Hexxagon/GameBoard/GameBoard.cpp"
#include <utility>
#include <vector>
#include <iostream>
#include <map>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

/**
 * \brief Class SelectMode
 */
class SelectMode
{
public:
    sf::Texture textures[5];
    sf::Sprite buttons[5];
    sf::Text text;
    sf::Font font;
    std::string inputText;

    int screenDimX, screenDimY;

    SelectMode(int screenDimX, int screenDimY);
    void reset();
    void loadTextures();
    void loadButtons();
    bool contains(sf::RenderWindow &window) const;
    bool containsQuit(sf::RenderWindow &window) const;
    void draw(sf::RenderWindow &window) const;
};

/**
 * \brief procedure that changes Window and displays all buttons from SelectMode class
 *
 *  Detailed instruction in .cpp file
 *
 * @param object // SelectMode object
 * @param window  // Window to display object elements
 * @param mainMenu // bool value, when turned to true, window changes to mainMenu view
 * @param play  // bool value, when turned to true, window changes to Play view
 * @param multiGame // bool value, when turned to true, window changes to Multiplayer  view
 * @param select    // bool value, when turned to false, window changes to different view
 */
void chooseMode(SelectMode &object, sf::RenderWindow &window, bool &mainMenu, bool &play, bool &multiGame, bool &select);

#endif //HEXXAGON_SELECTMODE_HPP
