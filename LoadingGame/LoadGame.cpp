#include "/Users/michallejza/CLionProjects/Hexxagon/LoadingGame/LoadGame.hpp"
#pragma once
/**
 * \brief constructor of GameFiles class
 */
GameFiles::GameFiles() {
    this -> allFiles = std::vector<std::filesystem::directory_entry>();
    this -> recentFiles = std::vector<std::filesystem::directory_entry>();
    this -> bestFiles = std::vector<std::filesystem::directory_entry>();
    this -> recentGames = std::vector<Board>();
    this -> bestGames = std::vector<Board>();
    this -> directory = "/Users/michallejza/Desktop/SavedGames";
    loadFiles();
    loadBoards();
}
/**
 * \brief methos to compre files, based on date
 * @param a
 * @param b
 * @return
 */
bool compareLastWriteTime(const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
    return std::filesystem::last_write_time(a) > std::filesystem::last_write_time(b);
}
/**
 * \brief method to compare files, based on score of two games
 * @param a
 * @param b
 * @return
 */
bool compareBestScore(const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
    std::ifstream fileA(a.path());
    int scoreA = 0;
    int number = 0;
    int i = 0;
    while (fileA >> number && i < 2) {
        scoreA += number;
        i++;
    }

    std::ifstream fileB(b.path());
    int scoreB = 0;
    number = 0;
    i = 0;
    while (fileB >> number && i < 2) {
        scoreB += number;
        i++;
    }
    return scoreA > scoreB;
}
/**
 * \brief method loads files from directory, and adds them to selected vectors
 */
void GameFiles::loadFiles() {
    for (const auto& entry : std::filesystem::directory_iterator(directory))
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
            allFiles.push_back(entry);

    std::sort(allFiles.begin(), allFiles.end(), compareLastWriteTime);
    int numFiles = std::min(5, static_cast<int>(allFiles.size()));
    for (int i = 0; i < numFiles; ++i)
        recentFiles.push_back(allFiles.at(i));

    std::sort(allFiles.begin(), allFiles.end(), compareBestScore);
    int numScore = std::min(5, static_cast<int>(allFiles.size()));
    for (int i = 0; i < numScore; ++i)
        bestFiles.push_back(allFiles.at(i));
}
/**
 * \brief method takes all info from files, changes them into boards, and adds them to vectors
 * in a file, first number folds redScore, second number holds blueScore, third number holds
 * 0 or 1, if 1, then a game is multiplayer, the fourth number is turn of game, basically, who begins.
 * later are numbers either 0,1,2 or 4. 0 is a free field, 1 is a red field, 2 is a blue field and 4
 * is out of scope. We dont include number four in gameplay
 * @param fileDirectory
 * @return
 */
Board GameFiles::changeFileToBoard(const std::filesystem::directory_entry &fileDirectory) {
    Board board = Board(9);
    std::vector<int> numbers = std::vector<int>();
    std::ifstream file(fileDirectory.path());

    int number = 0;
    while (file >> number)
        numbers.push_back(number);

    board.redScore = numbers.at(0);
    board.blueScore = numbers.at(1);
    board.multi = numbers.at(2);
    board.turn = numbers.at(3);
    int index = 4;
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if(numbers.at(index) == 0)
                board.board[i][j].changeToFree();
            if(numbers.at(index) == 1)
                board.board[i][j].changeToRed();
            if(numbers.at(index) == 2)
                board.board[i][j].changeToBlue();
            if(numbers.at(index) == 4)
                board.board[i][j].changeToOut();
            index++;
        }
    }
    board.positionBlue = board.positionsBlue();
    board.positionRed = board.positionsRed();
    return board;
}
/**
 * \brief method adds Boards to vectors from files
 */
void GameFiles::loadBoards() {
    for (auto &file : bestFiles)
        bestGames.push_back(changeFileToBoard(file));
    for(auto &file : recentFiles)
        recentGames.push_back(changeFileToBoard(file));
}
/**
 * \brief method adjust gameBoard object values, set them as they should be
 *
 *
 *
 * @param gameBoard
 * @param gameFiles
 * @param index
 */
void loadGame(GameBoard &gameBoard, GameFiles &gameFiles, int index) {
    std::filesystem::path pathName(  gameFiles.recentFiles.at(index));
    gameBoard.filename = pathName.stem().string();
    gameBoard.turn = gameFiles.recentGames.at(index).turn;
    gameBoard.multiPlayer = gameFiles.recentGames.at(index).multi == 1;
    gameBoard.board.redScore = gameFiles.recentGames.at(index).redScore;
    gameBoard.board.blueScore = gameFiles.recentGames.at(index).blueScore;

    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++) {
            if(gameFiles.recentGames.at(index).board[i][j].out)
                gameBoard.board.board[i][j].changeToOut();
            if(gameFiles.recentGames.at(index).board[i][j].free)
                gameBoard.board.board[i][j].changeToFree();
            if(gameFiles.recentGames.at(index).board[i][j].red)
                gameBoard.board.board[i][j].changeToRed();
            if(gameFiles.recentGames.at(index).board[i][j].blue)
                gameBoard.board.board[i][j].changeToBlue();
        }

    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            if(!gameBoard.board.board[i][j].out) {
                if (gameBoard.board.board[i][j].red)
                    gameBoard.buttons.at(gameBoard.board.board[i][j].index).setTexture(gameBoard.textures[5]);
                if (gameBoard.board.board[i][j].blue)
                    gameBoard.buttons.at(gameBoard.board.board[i][j].index).setTexture(gameBoard.textures[1]);
                if (gameBoard.board.board[i][j].free)
                    gameBoard.buttons.at(gameBoard.board.board[i][j].index).setTexture(gameBoard.textures[0]);
            }
}
