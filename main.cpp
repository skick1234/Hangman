#include "Game.h"
#include "SkickSDL.h"

using namespace std;

const int PLAY_TIME = 90;               // Time of a hangman game
const int SCREEN_WIDTH = 950;           // SDL window width
const int SCREEN_HEIGHT = 900;          // SDL window height
const string WINDOW_TITLE = "Hangman";  // SDL window title

int main(int argc, char* argv[]) {
    // initialize SkickSDL window
    SkickSDL* SDL = new SkickSDL(WINDOW_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL->openFont("VeraMoBd.ttf", 30);         // text font and size
    srand(time(0));                            // random seeds
    Game* hangman = new Game(SDL, PLAY_TIME);  // initialize game
    while (hangman->playing) {                 // while player is playing game
        hangman->startGame();                  // start a game
        do {                                   // initialize game loop for rendering
            hangman->renderGameSDL();          // render SDL game
            hangman->guessEvent();             // handle SDL keypress event
            hangman->handleGuess();            // handle game based on event
            hangman->updateTime();             // update playTime
        } while (hangman->guessing());         // render game if the player is guessing
        hangman->gameOver();                   // handle game over data and render SDL
    }
    return 0;
}
