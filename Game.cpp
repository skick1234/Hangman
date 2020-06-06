#include "Game.h"

#include <ctime>

#include "SkickSDL.h"
#include "utility.h"

Game::Game(SkickSDL* SkickSDL, int time) : SDL(SkickSDL), playTime(time) {
    playing = true;
    win = 0;
    lose = 0;
}

void Game::wordCategory() {
    category = "";
    while (category == "" && playing && !quit) {
        renderCategory();
        chooseCategory();
    }
    wordDifficulty();
}

void Game::chooseCategory() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            playing = false;
            quit = true;
        } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
            playing = false;
        } else if (event.type == SDL_KEYUP) {
            string keyName = SDL_GetKeyName(event.key.keysym.sym);
            if (keyName.length() == 1 && keyName[0] >= '1' && keyName[0] <= '5')
                switch (keyName[0]) {
                    case '1': category = "words.txt"; catName = "All Categories"; break;
                    case '2': category = "fruits.txt"; catName = "Fruits"; break;
                    case '3': category = "asia.txt"; catName = "Asia Countries"; break;
                    case '4': category = "jobs.txt"; catName = "Jobs"; break;
                    case '5': category = "plants.txt"; catName = "Plants"; break;
                }
        }
    }
}

void Game::renderCategory() {
    SDL->createImageBackground("hang0.png");
    SDL->createTextTexture("Choose word category:", 100, 50);
    SDL->createTextTexture("1. All Categories", 150, 100);
    SDL->createTextTexture("2. Fruits", 150, 150);
    SDL->createTextTexture("3. Asia Countries", 150, 200);
    SDL->createTextTexture("4. Jobs", 150, 250);
    SDL->createTextTexture("5. Plants", 150, 300);
    SDL->updateScreen();
}

void Game::wordDifficulty() {
    difficult = -1;
    while (difficult == -1 && playing && !quit) {
        renderDifficulty();
        chooseDifficulty();
    }
}

void Game::chooseDifficulty() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            playing = false;
            quit = true;
        } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
            playing = false;
        } else if (event.type == SDL_KEYUP) {
            string keyName = SDL_GetKeyName(event.key.keysym.sym);
            if (keyName.length() == 1 && keyName[0] >= '1' && keyName[0] <= '5')
                switch (keyName[0]) {
                    case '1': difficult = 0; break;
                    case '2': difficult = 1; break;
                }
        }
    }
}

void Game::renderDifficulty() {
    SDL->createImageBackground("hang0.png");
    SDL->createTextTexture("Category: " + catName, 100, 50);
    SDL->createTextTexture("Choose word difficulty:", 100, 100);
    SDL->createTextTexture("1. Easy", 150, 150);
    SDL->createTextTexture("2. Hard", 150, 200);
    SDL->updateScreen();
}

void Game::renderPlane(char guessedChar, int num) {
    int i = -300;
    while (i < 1000) {
        SDL->createImageBackground("hang0.png");
        SDL->createImage("plane1.png", i, 0);
        SDL->createTextTexture(string("There ") + (num == 1 ? "is " : "are ") + to_string(num) + " of " + guessedChar, i+165, 215);
        SDL->updateScreen();
        i+=5;
    }
}

void Game::startGame() {
    quit = false;
    system("cls");
    wordCategory();
    initWord();
    guessedWord = string(word.length(), '-');
    guessChar = ' ';
    badGuessCount = 0;
    badGuess = "";
    suggested = 0;
    maxSuggest = word.length() / 2;
    guessedStr = "";
    time(&startTime);
}

void Game::initWord() {
    word = chooseWord(category, difficult);
    if (word.empty()) {
        cout << "No word available in " << category << endl;
        playing = false;
        quit = true;
    }
}

void Game::guessEvent() {
    SDL_Event event;
    guessChar = ' ';
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            playing = false;
            quit = true;
        } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
            playing = false;
        } else if (event.type == SDL_KEYUP) {
            string keyName = SDL_GetKeyName(event.key.keysym.sym);
            if (event.key.keysym.sym == SDLK_SPACE)
                guessChar = '$';
            else if (keyName.length() == 1 && keyName[0] >= 'A' && keyName[0] <= 'Z')
                guessChar = tolower(keyName[0]);
        }
    }
}

void Game::handleGuess() {
    if (guessChar == ' ') return;
    if (guessChar == '$')
        getSuggest();
    else if (contains(guessedStr, guessChar))
        return;
    else if (contains(word, guessChar)) {
        updateGuessedWord();
        updateSuggest();
    } else if (!contains(badGuess, guessChar)) {
        badGuessed();
        renderPlane(guessChar, 0);
    }
}

void Game::badGuessed() {
    badGuessCount++;
    badGuess += guessChar;
    guessedStr += guessChar;
}

bool Game::guessing() {
    return badGuessCount < MAX_BAD_GUESS && guessedWord != word && timeLeft > 0 && playing;
}

void Game::updateTime() {
    time_t now;
    time(&now);
    timeLeft = playTime - difftime(now, startTime);
}

void Game::gameOver() {
    if (guessedWord != word)
        lose++;
    else
        win++;
    createGameOverSDL();
}

void Game::updateGuessedWord() {
    int n = guessedWord.length(), numOfChar = 0;
    for (int i = 0; i < n; i++) {
        if (word[i] == guessChar) {
            guessedWord[i] = guessChar;
            numOfChar++;
        }
    }
    guessedStr += guessChar;
    renderPlane(guessChar, numOfChar);
}

void Game::updateSuggest() {
    if (suggested < maxSuggest) {
        int suggest = 0, n = guessedWord.length();
        for (int i = 0; i < n; i++)
            if (guessedWord[i] == '-')
                suggest++;
        maxSuggest = suggest / 2;
    }
    if (suggested > maxSuggest) maxSuggest = suggested;
}

void Game::getSuggest() {
    if (suggested < maxSuggest) {
        suggested++;
        while (true) {
            int i = rand() % guessedWord.length();
            if (guessedWord[i] == '-') {
                guessChar = word[i];
                updateGuessedWord();
                break;
            }
        }
    }
}

void Game::renderGameSDL() {
    SDL->createImageBackground("hang" + to_string(badGuessCount) + ".png");
    SDL->createTextTexture("Time: " + to_string(timeLeft), 750, 5);
    SDL->createTextTexture("Win : " + to_string(win), 750, 45);
    SDL->createTextTexture("Lose: " + to_string(lose), 750, 85);
    SDL->createTextTexture("Current Guess    :     " + guessedWord, 100, 750);
    SDL->createTextTexture("Bad Guesses      :     " + badGuess, 100, 800);
    SDL->createTextTexture("Used suggestions :     " + to_string(suggested) + "/" + to_string(maxSuggest) + "   (Press 'Space')", 100, 850);
    SDL->updateScreen();
}

void Game::renderGameOverSDL(int imageIndex) {
    string status = (guessedWord == word ? "free" : "hanged");
    SDL->createImageBackground(status + to_string(imageIndex) + ".png");
    if (timeLeft <= 0)
        SDL->createTextTexture("Time Up!!!", 750, 5);
    SDL->createTextTexture("Win : " + to_string(win), 750, 45);
    SDL->createTextTexture("Lose: " + to_string(lose), 750, 85);
    if (guessedWord == word)
        SDL->createTextTexture("Congrats!!! You are free.", 100, 750);
    else
        SDL->createTextTexture("Game Over!!! You are hanged!", 100, 750);
    SDL->createTextTexture("Correct word: " + word, 100, 800);
    SDL->createTextTexture("Press Enter to keep playing, ESC to exit.", 100, 850);
    SDL->updateScreen();
}

void Game::createGameOverSDL() {
    int imageIndex = 0;
    time_t start, now;
    time(&start);
    while (!quit) {
        SDL_Event e;
        checkContinue(e);
        renderGameOverSDL(imageIndex);
        // framerate
        time(&now);
        if (difftime(now, start) >= 0.25) {
            time(&start);
            imageIndex++;
            if (imageIndex == 4) imageIndex = 0;
        }
    }
}

void Game::checkContinue(SDL_Event e) {
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)) {
            playing = false;
            quit = true;
        } else if (e.type == SDL_KEYUP &&
                   (e.key.keysym.sym == SDLK_RETURN ||
                    e.key.keysym.sym == SDLK_RETURN2 ||
                    e.key.keysym.sym == SDLK_KP_ENTER)) {
            playing = true;
            quit = true;
        }
    }
}
