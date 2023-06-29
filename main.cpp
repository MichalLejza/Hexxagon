#include "Board/Board.cpp"
#include "SelectMode/SelectMode.cpp"
#include "MainMenu/MainMenu.cpp"
#include "GameBoard/GameBoard.cpp"
#include "FileHandling/FileHandling.cpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main()
{
    // Dimensions of screen
    int X =  1700;
    int Y = 1200;

    // Creating all necessary objects
    Board board = Board(9);
    GameBoard gameBoard = GameBoard(board, Field(0, 0, false, false, false, 0), X, Y);
    MainMenu menu = MainMenu(X, Y);
    SelectMode selectMode = SelectMode(X, Y);
    LoadFile loadFile = LoadFile(X, Y);
    ScoreBoard scoreBoard1 = ScoreBoard(X,Y);

    // Create Window
    sf::RenderWindow window(sf::VideoMode(X,Y), "Hexxagon", sf::Style::Titlebar | sf::Style::Close);

    // all bool value to display right window view
    bool mainMenu = true;
    bool scoreBoard = false;
    bool loadGame = false;
    bool multiGame = false;
    bool play = false;
    bool select = false;

    while (window.isOpen())
    {
        if(mainMenu)
            menuBoard(window,menu,loadGame, multiGame, scoreBoard, mainMenu, select);
        if(scoreBoard)
            showScoreBoard(window,scoreBoard1, mainMenu, scoreBoard);
        if(loadGame)
            showLoadFile(window, loadFile, gameBoard, mainMenu, loadGame, play);
        if(select)
            chooseMode(selectMode, window, gameBoard, mainMenu, play, multiGame, select);
        if(play)
            playGame(window, gameBoard, mainMenu, play);
    }
    return 0;
}