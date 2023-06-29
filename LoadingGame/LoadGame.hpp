#ifndef HEXXAGON_LOADGAME_HPP
#define HEXXAGON_LOADGAME_HPP

#include "/Users/michallejza/CLionProjects/Hexxagon/Board/Board.cpp"
#include "/Users/michallejza/CLionProjects/Hexxagon/GameBoard/GameBoard.cpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

/**
 * \brief class GameFiles, made to handle saving game to a file
 * std::string directory is directory of folder where we save file
 * allFiles holds all files from directory
 * recentFiles holds all files, but it is sorted based on date of creation
 * bestFiles holds all files, but based on score of game
 * recentGames holds all Games, games created from info from files, but sorted as recentFiles
 */
class GameFiles
{
public:
    std::string directory;
    std::vector<std::filesystem::directory_entry> allFiles;
    std::vector<std::filesystem::directory_entry> recentFiles;
    std::vector<std::filesystem::directory_entry> bestFiles;
    std::vector<Board> recentGames;
    std::vector<Board> bestGames;

    GameFiles();
    void loadFiles();
    static Board changeFileToBoard(const std::filesystem::directory_entry &fileDirectory);
    void loadBoards();
};
/**
 * \brief method loads game from a file, adjusted of gameBoard objects
 * @param gameBoard
 * @param gameFiles
 * @param index
 */
void loadGame(GameBoard &gameBoard, GameFiles &gameFiles, int index);

#endif //HEXXAGON_LOADGAME_HPP
