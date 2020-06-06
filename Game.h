#ifndef GAME_H
#define GAME_H
#include <ctime>
#include <string>

#include "SkickSDL.h"
using namespace std;

class Game {
    const int MAX_BAD_GUESS = 7;
    string category;
    string catName;
    int difficult = -1;
    SkickSDL* SDL;
    int playTime;
    int win;
    int lose;
    bool quit;
    string word;
    char guessChar;
    string guessedWord;
    string guessedStr;
    int badGuessCount;
    int suggested;
    int maxSuggest;
    int timeLeft;
    string badGuess;
    time_t startTime;

   public:
    bool playing;
    Game(SkickSDL* SkickSDL, int time);
    void wordCategory();
    void renderCategory();
    void chooseCategory();
    void wordDifficulty();
    void renderDifficulty();
    void chooseDifficulty();
    void getSuggest();
    void updateGuessedWord();
    void updateSuggest();
    bool guessing();
    void guessEvent();
    void handleGuess();
    void updateTime();
    void initWord();
    void startGame();
    void badGuessed();
    void gameOver();
    void checkContinue(SDL_Event e);
    void renderGameSDL();
    void renderGameOverSDL(int imageIndex);
    void createGameOverSDL();
    void renderPlane(char guessedChar, int num);
};

#endif  // GAME_H
