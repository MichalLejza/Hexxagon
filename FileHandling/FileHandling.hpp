#ifndef HEXXAGON_FILEHANDLING_HPP
#define HEXXAGON_FILEHANDLING_HPP

#include "/Users/michallejza/CLionProjects/Hexxagon/Board/Board.cpp"
#include "/Users/michallejza/CLionProjects/Hexxagon/LoadingGame/LoadGame.cpp"
#include <iostream>
#include <fstream>
#include <filesystem>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

/**
 * LoadFile class is screen when we click 'Load Button' in Main menu
 * it has all objects necessary to display 5 recent games, button to quit menu and
 * button to load game and continue playing it
 */
class LoadFile {
public:
    std::string directory;
    std::string fileName;
    sf::Sprite files[5];
    sf::Sprite backGround, quit, logo, play;
    sf::Texture textures[6];
    sf::Text fileInfo[5];
    sf::Font font;
    GameFiles gameFiles;
    int screenDimX{}, screenDimY{}, index;

    LoadFile(int screenDimX, int screenDimY);
    void loadTextures();
    void loadButtons();
    void updateFiles();
    void resetLoadFile();
    void changeTexture(int i);
    bool containsFile(sf::RenderWindow &window, int i) const;
    bool contains(sf::RenderWindow &window) const;
    bool containsPlay(sf::RenderWindow &window) const;
    void draw(sf::RenderWindow &window) const;
};

/**
 * ScoreBoard class holds all necessary SFML objects to view
 * Score board screen with 5 best games played by player and button
 * to quit to menu
 */
class ScoreBoard {
public:
    std::string directory;
    sf::Sprite backGround, quit, logo;
    sf::Sprite scores[5];
    sf::Texture textures[4];
    sf::Text scoreInfo[5];
    sf::Font font;
    GameFiles gameFiles;
    int screenDimX{}, screenDimY{};

    ScoreBoard(  int screenDimX, int screenDimY);
    void loadTextures();
    void loadButtons();
    void updateFiles();
    bool contains(sf::RenderWindow &window) const;
    void draw(sf::RenderWindow &window) const;
};


#endif //HEXXAGON_FILEHANDLING_HPP
