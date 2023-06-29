#include "/Users/michallejza/CLionProjects/Hexxagon/FileHandling/FileHandling.hpp"
#include <filesystem>
#include <iostream>
#pragma once

/**
 * \brief constructor of LoadFile class
 * @param screenDimX
 * @param screenDimY
 */
LoadFile::LoadFile(int screenDimX, int screenDimY){
    this -> screenDimX = screenDimX;
    this -> screenDimX = screenDimY;
    this -> backGround = sf::Sprite();
    this -> quit = sf::Sprite();
    this -> font = sf::Font();
    this -> index = 0;
    this -> play = sf::Sprite();
    this -> logo = sf::Sprite();
    this -> directory = "/Users/michallejza/CLionProjects/Hexxagon/SavedGames/";
    loadTextures();
    loadButtons();
}

/**
 * \brief loads all necessary textures
 */
void LoadFile::loadTextures() {
    textures[0].loadFromFile("/Users/michallejza/Desktop/Tekstury/backGround.png");
    textures[1].loadFromFile("/Users/michallejza/Desktop/Tekstury/quitMenu.jpg");
    textures[2].loadFromFile("/Users/michallejza/Desktop/Tekstury/loadNormal.png");
    textures[3].loadFromFile("/Users/michallejza/Desktop/Tekstury/logo.png");
    textures[4].loadFromFile("/Users/michallejza/Desktop/Tekstury/loadClicked.png");
    textures[5].loadFromFile("/Users/michallejza/Desktop/Tekstury/letsplay.jpg");
    if(!font.loadFromFile("/Users/michallejza/Desktop/Tekstury/font.ttf.otf")) {}
}
/**
 * \brief from directory, adds all file names as std::string
 */
void LoadFile::updateFiles() {
    for (int i = 0; i < gameFiles.recentFiles.size(); i++) {
        std::filesystem::path pathFile(gameFiles.recentFiles.at(i).path());
        fileInfo[i].setString(pathFile.stem().string());
    }
}

void LoadFile::resetLoadFile() {
    loadButtons();
}

/**
 * \brief method sets all SFML objects, coordintes, scale, textures ect.
 */
void LoadFile::loadButtons() {
    backGround.setTexture(textures[0]);
    backGround.setScale(2.f,2.f);

    logo.setTexture(textures[3]);
    logo.setScale(0.4,0.4);
    logo.setPosition(520,50);

    quit.setTexture(textures[1]);
    quit.setPosition(100,1000);
    quit.setTextureRect(sf::IntRect(0,0,382,99));

    play.setTexture(textures[5]);
    play.setTextureRect(sf::IntRect(0,0,382,99));
    play.setPosition(1100, 1000);

    for(int i = 0; i < 5; i++) {
        files[i].setTexture(textures[2]);
        files[i].setPosition(300, 250 + 150 * i);
        files[i].setScale(0.8,0.8);

        fileInfo[i].setFillColor(sf::Color::Black);
        fileInfo[i].setFont(font);
        fileInfo[i].setPosition(370, 270 + 150 * i);
        fileInfo[i].setCharacterSize(40);
    }
    updateFiles();
}

/**
 * \brief methods changes texture when we clicked on a game, and changes
 * all textures of other Sprites to original texture
 * @param i
 */
void LoadFile::changeTexture(int i) {
    for (auto & file : files)
        file.setTexture(textures[2]);
    files[i].setTexture(textures[4]);
}

bool LoadFile::containsFile(sf::RenderWindow &window, int i) const {
    return files[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)));
}

bool LoadFile::contains(sf::RenderWindow &window) const {
    return quit.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)));
}

bool LoadFile::containsPlay(sf::RenderWindow &window) const {
    return play.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)));
}

/**
 * \brief method draws all SFML objects to a window
 * @param window
 */
void LoadFile::draw(sf::RenderWindow &window) const {
    window.clear();
    window.draw(backGround);
    window.draw(logo);
    for(const auto& f : files)
        window.draw(f);
    for (const auto& f : fileInfo)
        window.draw(f);
    window.draw(play);
    window.draw(quit);
    window.display();
}

/**
 * \brief method to handle events in LoadFile View
 * @param window
 * @param loadFile
 * @param gameBoard
 * @param menu
 * @param lf loadFile, when we press quit/load game it changes to false
 * @param play
 */
void showLoadFile(sf::RenderWindow &window, LoadFile &loadFile, GameBoard &gameBoard, bool &menu, bool &lf, bool &play) {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (loadFile.contains(window)) {
                menu = true;
                lf = false;
                loadFile.resetLoadFile();
                return;
            }
            if (loadFile.containsPlay(window)) {
                menu = false;
                lf = false;
                play = true;
                loadGame(gameBoard, loadFile.gameFiles, loadFile.index);
                loadFile.resetLoadFile();
                return;
            }
            for (int i = 0; i < 5; i++) {
                if (loadFile.containsFile(window, i))
                {
                    loadFile.changeTexture(i);
                    loadFile.index = i;
                }
            }
        }
    }
    loadFile.draw(window);
}

ScoreBoard::ScoreBoard(int screenDimX, int screenDimY) {
    this -> screenDimX = screenDimX;
    this -> screenDimX = screenDimY;
    this -> backGround = sf::Sprite();
    this -> quit = sf::Sprite();
    this -> font = sf::Font();
    this -> logo = sf::Sprite();
    this -> directory = "/Users/michallejza/Desktop/SavedGames";
    loadTextures();
    loadButtons();
}

void ScoreBoard::loadTextures() {
    textures[0].loadFromFile("/Users/michallejza/Desktop/Tekstury/backGround.png");
    textures[1].loadFromFile("/Users/michallejza/Desktop/Tekstury/quitMenu.jpg");
    textures[2].loadFromFile("/Users/michallejza/Desktop/Tekstury/loadNormal.png");
    textures[3].loadFromFile("/Users/michallejza/Desktop/Tekstury/logo.png");
    if(!font.loadFromFile("/Users/michallejza/Desktop/Tekstury/font.ttf.otf")) {}
}

void ScoreBoard::updateFiles() {
    for(int i = 0; i < gameFiles.bestFiles.size(); i++) {
        std::filesystem::path pathFile(gameFiles.bestFiles.at(i).path());
        std::string score = "   score:   " + std::to_string(gameFiles.bestGames.at(i).redScore) + " : " + std::to_string(gameFiles.bestGames.at(i).blueScore);
        scoreInfo[i].setString(pathFile.stem().string() + score);
    }
}

void ScoreBoard::loadButtons() {
    gameFiles = GameFiles();
    backGround.setTexture(textures[0]);
    backGround.setScale(2.f,2.f);

    logo.setTexture(textures[3]);
    logo.setScale(0.4,0.4);
    logo.setPosition(520,50);

    quit.setTexture(textures[1]);
    quit.setPosition(100,1000);
    quit.setTextureRect(sf::IntRect(0,0,382,99));

    for(int i = 0; i < 5; i++) {
        scores[i].setTexture(textures[2]);
        scores[i].setPosition(300, 250 + 150 * i);
        scores[i].setScale(0.8,0.8);

        scoreInfo[i].setFillColor(sf::Color::Black);
        scoreInfo[i].setFont(font);
        scoreInfo[i].setPosition(370, 270 + 150 * i);
        scoreInfo[i].setCharacterSize(40);
    }
    updateFiles();
}

bool ScoreBoard::contains(sf::RenderWindow &window) const {
    return quit.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)));
}

void ScoreBoard::draw(sf::RenderWindow &window) const {
    window.clear();
    window.draw(backGround);
    window.draw(logo);
    for(const auto& f : scores)
        window.draw(f);
    for (const auto& f : scoreInfo)
        window.draw(f);
    window.draw(quit);
    window.display();
}

void showScoreBoard(sf::RenderWindow &window, ScoreBoard &scoreBoard, bool &menu, bool &sb) {
    sf::Event event{};
    scoreBoard.updateFiles();
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if(scoreBoard.contains(window)) {
                menu = true;
                sb = false;
                return;
            }
        }
    }
    scoreBoard.draw(window);
}