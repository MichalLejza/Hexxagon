#ifndef HEXXAGON_MAINMENU_HPP
#define HEXXAGON_MAINMENU_HPP

#pragma once

#include "/Users/michallejza/CLionProjects/Hexxagon/Board/Board.cpp"
#include <utility>
#include <vector>
#include <iostream>
#include <map>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

/**
 * \brief Class MainMenu, made to show Main Menu, all buttons and handle events
 */
class MainMenu
{
public:

    sf::Texture textures[6];
    sf::Sprite backGround;
    sf::Sprite logo;
    sf::Sprite buttons[4];
    sf::Font font;

    int screenDimX{}, screenDimY{};

    MainMenu(int x, int y);
    MainMenu();
    void loadTextures();
    void loadButtons();
    bool contains(sf::RenderWindow &window, int index);
    void draw(sf::RenderWindow &window);
};

void menuBoard(sf::RenderWindow &window, MainMenu &menu, bool &loadGame, bool &multi, bool &scoreBoard, bool &mainMenu, bool &select);

#endif //HEXXAGON_MAINMENU_HPP
