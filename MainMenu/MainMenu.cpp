#include "/Users/michallejza/CLionProjects/Hexxagon/MainMenu/MainMenu.hpp"

#pragma once

/**
 * \brief Constructor of MainMenu class
 * @param x screen dimension X
 * @param y screen dimenion Y
 */
MainMenu::MainMenu(int x, int y) {
    this -> screenDimX = x;
    this -> screenDimY = y;
    this -> backGround = sf::Sprite();
    this -> logo = sf::Sprite();
    this -> font = sf::Font();
    loadTextures();
    loadButtons();
}

MainMenu::MainMenu() = default;
/**
 * \brief load all necessary textures
 */
void MainMenu::loadTextures() {
    font.loadFromFile("/Users/michallejza/Desktop/Tekstury/font.ttf.otf");
    textures[0].loadFromFile("/Users/michallejza/Desktop/Tekstury/LoadGame.png");
    textures[1].loadFromFile("/Users/michallejza/Desktop/Tekstury/OnePlayer.png");
    textures[2].loadFromFile("/Users/michallejza/Desktop/Tekstury/TwoPlayer.png");
    textures[3].loadFromFile("/Users/michallejza/Desktop/Tekstury/ScoreBoard.png");
    textures[4].loadFromFile("/Users/michallejza/Desktop/Tekstury/backGround.png");
    textures[5].loadFromFile("/Users/michallejza/Desktop/Tekstury/logo.png");
    if(!font.loadFromFile("/Users/michallejza/Desktop/Tekstury/font.ttf.otf")) {}
}
/**
 * \brief set all SFML objects, coordintes, scale textures ect.
 */
void MainMenu::loadButtons() {
    backGround.setTexture(textures[4]);
    backGround.setScale(2.f,2.f);
    logo.setTexture(textures[5]);
    logo.setPosition((float)screenDimX / 5, (float)screenDimY / 12);
    logo.setScale(0.65, 0.65);
    for(int i = 0; i < 4; i++) {
        buttons[i].setTexture(textures[i]);
        buttons[i].setScale(1,1);
        buttons[i].setTextureRect(sf::IntRect(0,0,432,120));
        buttons[i].setPosition((float)screenDimX / 2.65f, (float)screenDimY / 2.5f + (float)i * 140);
    }
}
/**
 * \brief method to check if we clicked given button
 * @param window
 * @param index index of a button from Main menu view, indexed as viewed in game
 * @return true, if we clicked on a button
 */
bool MainMenu::contains(sf::RenderWindow &window, int index) {
    return buttons[index].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)));
}
/**
 * \brief method draws all SFML object to a given window
 * @param window
 */
void MainMenu::draw(sf::RenderWindow &window) {
    window.clear();
    window.draw(backGround);
    window.draw(logo);
    for(const auto & button : buttons)
        window.draw(button);
    window.display();
}
/**
 * \brief procedure handles events, and draw SFML objects
 *
 * @param window
 * @param menu
 * @param loadGame
 * @param multi
 * @param scoreBoard
 * @param mainMenu
 * @param select
 */
void menuBoard(sf::RenderWindow &window, MainMenu &menu, bool &loadGame, bool &multi, bool &scoreBoard, bool &mainMenu, bool &select) {
    sf::Event event {};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if(menu.contains(window, 0))
                loadGame = true;
            if(menu.contains(window,1)) {
                multi = true;
                select = true;
            }
            if(menu.contains(window, 2))
                select = true;
            if(menu.contains(window, 3))
                scoreBoard = true;
            mainMenu = false;
            return;
        }
    }
    menu.draw(window);
}
