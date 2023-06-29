#include "/Users/michallejza/CLionProjects/Hexxagon/SelectMode/SelectMode.hpp"
#pragma once

/**
 * \brief SelectMode constructor, initializes screenDimX, screenDimY which are Screen dimension
 *        font, sf::Sprite and sf::Text which will display name of a file to save the game
 * @param screenDimX
 * @param screenDimY
 */
SelectMode::SelectMode(int screenDimX, int screenDimY) {
    this -> screenDimX = screenDimX;
    this -> screenDimY = screenDimY;
    this -> font = sf::Font();
    this -> inputText = sf::String();
    this -> text = sf::Text();
    loadTextures();
    loadButtons();
}

/**
 * \brief resets all buttons, texts, basically resets object to original form
 */
void SelectMode::reset() {
    loadButtons();
}

/**
 * \brief loads all important textures
 */
void SelectMode::loadTextures() {
    textures[0].loadFromFile("/Users/michallejza/Desktop/Tekstury/letsplay.jpg"); // play button
    textures[1].loadFromFile("/Users/michallejza/Desktop/Tekstury/backGround.png"); // background
    textures[2].loadFromFile("/Users/michallejza/Desktop/Tekstury/textNormal.png"); // text normal color
    textures[3].loadFromFile("/Users/michallejza/Desktop/Tekstury/quitMenu.jpg");   // quitToMenu button
    textures[4].loadFromFile("/Users/michallejza/Desktop/Tekstury/instruction.jpg");  // button with instructions
    if(!font.loadFromFile("/Users/michallejza/Desktop/Tekstury/Font.ttf.otf")) {} // font
}

/**
 * \brief sets all textures, positions and sizes for all sf::Sprites etc.
 */
void SelectMode::loadButtons() {
    inputText = "";

    buttons[0].setTexture(textures[1]);
    buttons[0].setScale(2.f,2.f);

    buttons[1].setTexture(textures[4]);
    buttons[1].setPosition((float)screenDimX / 3, (float)screenDimY / 4);
    buttons[1].setScale(1.2,1.2);

    buttons[2].setTexture(textures[0]);
    buttons[2].setPosition((float)screenDimX / 2.6f, (float)screenDimY / 1.6f);
    buttons[2].setTextureRect(sf::IntRect(0,0,380,105));

    buttons[3].setTexture(textures[2]);
    buttons[3].setPosition(350, 500);

    buttons[4].setTexture(textures[3]);
    buttons[4].setPosition((float)screenDimX / 17.f, (float)screenDimY / 1.2f);
    buttons[4].setTextureRect(sf::IntRect(0,0,382,99));

    text.setString("");
    text.setCharacterSize(50);
    text.setPosition((float)screenDimX / 4.14f, (float)screenDimY / 2.14f);
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
}

/**
 * \brief functions checks if we clicked on Let's Play! button
 * @param window
 * @return true if we clicked ona 'Let's Play!' Button
 */
bool SelectMode::contains(sf::RenderWindow &window) const {
    return buttons[2].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)));
}

/**
 * \brief functions checks if we clicked on quit button
 * @param window
 * @return true if we clidked on quit button
 */
bool SelectMode::containsQuit(sf::RenderWindow &window) const {
    return buttons[4].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)));
}

/**
 * \brief procedure draw all SFML objects in a window
 * @param window
 */
void SelectMode::draw(sf::RenderWindow &window) const {
    window.clear();
    for(const auto& f : buttons)
        window.draw(f);
    window.draw(text);
    window.display();
}

/**
 * \brief procedure handles events, and draw SFML objects
 *
 *  If we clidked on quit button, method adjust bool values and returns to change window view
 *  If we clicked on Let's Play! button, method adjust bool values, saves text input, which
 *  will later be used to save game and returns to view gameboard
 *  If we write something, text input will appear. I adjusted the method to only accept letters
 *  in lowercase. Everything else will be ignored.
 *
 * @param selectMode
 * @param window
 * @param gameBoard
 * @param mainMenu
 * @param play
 * @param multiGame
 * @param select
 */
void chooseMode(SelectMode &selectMode, sf::RenderWindow &window, GameBoard &gameBoard, bool &mainMenu, bool &play, bool &multiGame, bool &select) {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if(selectMode.contains(window) && !selectMode.inputText.empty()) {
                gameBoard.filename = selectMode.inputText;
                gameBoard.multiPlayer = multiGame;
                selectMode.reset();
                multiGame = false;
                select = false;
                mainMenu = false;
                play = true;
                return;
            }
            if(selectMode.containsQuit(window)) {
                selectMode.reset();
                select = false;
                multiGame = false;
                mainMenu = true;
                play = false;
                return;
            }
        }
        if(event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128) {
                if (event.text.unicode == '\b' && !selectMode.inputText.empty()) // Handle backspace
                    selectMode.inputText.pop_back();
                else if (event.text.unicode != '\b' && selectMode.inputText.size() < 22 && event.text.unicode < 123 && event.text.unicode > 96) // Append entered character
                    selectMode.inputText += static_cast<char>(event.text.unicode);
            }
            selectMode.text.setString(selectMode.inputText);
        }
    }
    selectMode.draw(window);
}
