#ifndef GAME_H
#define GAME_H
#include <ctime>
#include <string>

#include "SkickSDL.h"
using namespace std;

// Hangman Game class
class Game {
    const int MAX_BAD_GUESS = 7;  // Max bad guess of Game
    string category;              // Name of category file
    string catName;               // Name of category title
    int difficult = -1;           // Difficulty of words
    SkickSDL* SDL;                // Selfmade SDL for easy use
    int playTime;                 // Playing time of Game
    int win;                      // Number of win games
    int loss;                     // Number of loss games
    bool quit;                    // If player want to quit Game
    string word;                  // The random word has to be guessed
    char guessChar;               // Character player pressed
    string guessedWord;           // Guessed word include suggested characters
    string guessedStr;            // Guessed word exclude suggested characters
    int badGuessCount;            // Number of bad guesses made by the player
    int suggested;                // Number of suggested characters
    int maxSuggest;               // Maximum number of suggestions
    int timeLeft;                 // Left time in second
    string badGuess;              // Bad guessed characters
    time_t startTime;             // The start time of Game
    int animatedTime;             // Time of animations made

   public:
    bool playing;                       // Game is playing or not
    Game(SkickSDL*, int);               // initialize Game
    void startGame();                   // initialize new Game
    void chooseCategory();              // Choose the category of random word
    void renderCategory();              // render category window
    void chooseCategoryEvent();         // handle choose category event
    void chooseDifficulty();            // Choose the difficulty of random word
    void renderDifficulty();            // render difficulty window
    void chooseDifficultyEvent();       // handle choose difficulty event
    void initWord();                    // initialize word need guessing
    void getSuggest();                  // get a suggestion
    void updateGuessedWord();           // update guessed word
    void updateSuggest();               // update maximum number of suggestions
    bool guessing();                    // determine whether the player is guessing or not
    void guessEvent();                  // handle guessing event
    void handleGuess();                 // handle guessing character
    void updateTime();                  // update playing time
    void badGuessed();                  // handle bad guessed
    void gameOver();                    // initialize game over
    void checkContinue(SDL_Event);      // check if player want to continue playing game
    void renderGameSDL();               // render playing game window
    void renderGameOverSDL(int);        // render game over window
    void createGameOverSDL();           // initialize game over loop for rendering
    void renderPlane(char, int);        // render flying plane
    void planeEvent(SDL_Event, bool&);  // handle event while the plane is flying
};

#endif  // GAME_H
