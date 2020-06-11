#include "Game.h"

#include <ctime>
#include <tr1/unordered_map>

#include "SkickSDL.h"
#include "utility.h"

using namespace std::tr1;

Game::Game(SkickSDL* SkickSDL, int time) : SDL(SkickSDL), playTime(time) {
    playing = true;
    win = 0;
    loss = 0;
}

void Game::startGame() {
    quit = false;
    system("cls");
    chooseCategory();
    chooseDifficulty();
    initWord();
    guessedWord = string(word.length(), '-');
    guessChar = ' ';
    badGuessCount = 0;
    maxSuggest = 10;
    badGuess = "";
    suggested = 0;
    guessedStr = "";
    animatedTime = 0;
    time(&startTime);
    for (unsigned int i = 0; i < word.length(); i++)
        if (word[i] == ' ')
            guessedWord[i] = ' ';
    updateSuggest();
}

void Game::chooseCategory() {
    category = "";
    while (category == "" && playing && !quit) {
        renderCategory();
        chooseCategoryEvent();
    }
}

void Game::chooseCategoryEvent() {
    SDL_Event event;
    if (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            playing = false;
            quit = true;
        } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
            playing = false;
        } else if (event.type == SDL_KEYUP) {
            string keyName = SDL_GetKeyName(event.key.keysym.sym);
            if (keyName.length() == 1 && keyName[0] >= '1' && keyName[0] <= '5')
                switch (keyName[0]) {
                    case '1':
                        category = "all.txt";
                        catName = "All Categories";
                        break;
                    case '2':
                        category = "fruits.txt";
                        catName = "Fruits";
                        break;
                    case '3':
                        category = "asia.txt";
                        catName = "Asia Countries";
                        break;
                    case '4':
                        category = "jobs.txt";
                        catName = "Jobs";
                        break;
                    case '5':
                        category = "plants.txt";
                        catName = "Plants";
                        break;
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

void Game::chooseDifficulty() {
    difficult = -1;
    while (difficult == -1 && playing && !quit) {
        renderDifficulty();
        chooseDifficultyEvent();
    }
}

void Game::chooseDifficultyEvent() {
    SDL_Event event;
    if (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            playing = false;
            quit = true;
        } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
            playing = false;
        } else if (event.type == SDL_KEYUP) {
            string keyName = SDL_GetKeyName(event.key.keysym.sym);
            if (keyName.length() == 1 && keyName[0] >= '1' && keyName[0] <= '5')
                switch (keyName[0]) {
                    case '1':
                        difficult = 0;
                        break;
                    case '2':
                        difficult = 1;
                        break;
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
    time_t startT, endT;
    time(&startT);
    int i = -300;
    bool skip = false;
    while (i < 1000 && !skip) {
        SDL_Event event;
        planeEvent(event, skip);
        SDL->createImageBackground("hang0.png");
        SDL->createImage("plane.png", i, 0);
        SDL->createTextTexture(string("There ") + (num == 1 ? "is " : "are ") + to_string(num) + " of " + guessedChar, i + 165, 215);
        SDL->createTextTexture("Press 'Space' to skip", 300, 850);
        SDL->updateScreen();
        i += 5;
    }
    time(&endT);
    animatedTime += difftime(endT, startT);
}

void Game::planeEvent(SDL_Event e, bool& skip) {
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_SPACE)
            skip = true;
        if (e.type == SDL_QUIT) {
            playing = false;
            quit = true;
            skip = true;
        }
    }
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
            if (keyName == "Escape")
                playing = false;
            else if (keyName == "Space")
                guessChar = '$';
            else if (keyName.length() == 1 && keyName[0] >= 'A' && keyName[0] <= 'Z')
                guessChar = keyName[0];
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
    timeLeft = playTime - difftime(now, startTime) + animatedTime;
}

void Game::gameOver() {
    if (guessedWord != word)
        loss++;
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
        unordered_map<char, int> m;
        for (int i = 0; i < n; i++)
            if (guessedWord[i] == '-')
                m[word[i]]++;
        suggest = m.size();
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
    SDL->createTextTexture("Loss: " + to_string(loss), 750, 85);
    SDL->createTextTexture("Current Guess    :     " + guessedWord, 100, 750);
    SDL->createTextTexture("Bad Guesses      :     " + badGuess, 100, 800);
    SDL->createTextTexture("Used suggestions :     " + to_string(suggested) + "/" + to_string(maxSuggest) + "   (Press 'Space')", 100, 850);
    SDL->updateScreen();
    SDL_Delay(10);
}

void Game::renderGameOverSDL(int imageIndex) {
    string status = (guessedWord == word ? "free" : "hanged");
    SDL->createImageBackground(status + to_string(imageIndex) + ".png");
    if (timeLeft <= 0)
        SDL->createTextTexture("Time Up!!!", 750, 5);
    SDL->createTextTexture("Win : " + to_string(win), 750, 45);
    SDL->createTextTexture("Loss: " + to_string(loss), 750, 85);
    if (guessedWord == word)
        SDL->createTextTexture("Congrats!!! You are free.", 100, 750);
    else
        SDL->createTextTexture("Game Over!!! You are hanged!", 100, 750);
    SDL->createTextTexture("Correct word: " + word, 100, 800);
    SDL->createTextTexture("Press 'Enter' to keep playing, 'ESC' to exit.", 100, 850);
    SDL->updateScreen();
}

void Game::createGameOverSDL() {
    int imageIndex = 0;
    while (!quit) {
        SDL_Event e;
        checkContinue(e);
        renderGameOverSDL(imageIndex);
        SDL_Delay(200);
        ++imageIndex %= 4;
    }
}

void Game::checkContinue(SDL_Event e) {
    while (SDL_PollEvent(&e)) {
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
