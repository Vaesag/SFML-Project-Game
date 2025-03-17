#include "../include/reel.h"

void SlotMachine::loadTexture(Symbol key, const std::string& path) {
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cerr << "Ошибка загрузки текстуры: " << path << std::endl;
        return;
    }
    symbolTextures[key] = std::move(texture);
}

void SlotMachine::setTextures() {
    loadTexture(Symbol::Strawberry, PATH_STRAWBERRY);
    loadTexture(Symbol::Grapes, PATH_GRAPES);
    loadTexture(Symbol::Pineapple, PATH_PINEAPPLE);
    loadTexture(Symbol::Lemon, PATH_LEMON);
}

SlotMachine::SlotMachine(sf::RenderWindow& win) : window(win) {
    setTextures();

    if (!font.loadFromFile("fonts/Arial.ttf")) {
        std::cerr << "Error download font" << std::endl;
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(25);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(215, 480);
    scoreText.setString("0");

    for (int i = 0; i < REEL_COUNT; i++) {
        reels.emplace_back();
        float xPos = 105 + i * 133;
        float yOffset = 80;

        int index = 0;
        for (Symbol s : { Symbol::Strawberry, Symbol::Grapes, Symbol::Pineapple, Symbol::Lemon }) {
            sf::Sprite sprite;
            sprite.setTexture(symbolTextures[s]);
            float yPos = 100 + index * yOffset; 
            sprite.setPosition(xPos, yPos);
            reels[i].spriteSymbols.push_back(sprite);
            reels[i].initialPositions.push_back(yPos);
            index++;
        }
    }
}

void SlotMachine::startSpin() {
    for (auto& reel : reels) {
        reel.spinning = true;
        reel.speed = 1.0f + rand() % 1;
    }
    spinClock.restart();
}

void SlotMachine::stopSpin() {
    if (reelIndex < reels.size() && stopClock.getElapsedTime().asSeconds() > 1.0f) {
        reels[reelIndex].spinning = false;
        reels[reelIndex].speed = 0.0f;

        std::cout << "Stopping reel " << reelIndex + 1 << std::endl;

        for (size_t i = 0; i < reels[reelIndex].spriteSymbols.size(); i++) {
            reels[reelIndex].spriteSymbols[i].setPosition(
                reels[reelIndex].spriteSymbols[i].getPosition().x,
                reels[reelIndex].initialPositions[i]); 
        }

        reelIndex++; 
        stopClock.restart(); 
    }
}

void SlotMachine::draw() {
    for (auto& reel : reels) {
        if (reel.spriteSymbols.empty()) continue;

        for (auto& sprite : reel.spriteSymbols) {
            window.draw(sprite);
        }
    }
    window.draw(scoreText);
}

bool SlotMachine::areAllReelsStopped() const {
    for (const auto& reel : reels) {
        if (reel.spinning) {
            return false;
        }
    }
    return true;
}

void SlotMachine::update() {
    if (currentState) {
        currentState->update(*this);
    }

    for (auto& reel : reels) {
        if (!reel.spinning) continue;

        std::rotate(reel.symbols.rbegin(), reel.symbols.rbegin() + 1, reel.symbols.rend());
        std::rotate(reel.spriteSymbols.rbegin(), reel.spriteSymbols.rbegin() + 1, reel.spriteSymbols.rend());

        float yOffset = 80;
        for (size_t i = 0; i < reel.spriteSymbols.size(); i++) {
            float newY = reel.spriteSymbols[i].getPosition().y + reel.speed;

            if (newY >= 350) {
                newY = 100 - yOffset;
            }

            reel.spriteSymbols[i].setPosition(reel.spriteSymbols[i].getPosition().x, newY);
        }
    }
}

void SlotMachine::checkWin() {
    if (reels.empty()) return;

    std::map<Symbol, int> rewardTable = {
        {Symbol::Lemon, 1000000},
        {Symbol::Pineapple, 100000},
        {Symbol::Grapes, 10000},
        {Symbol::Strawberry, 1000}
    };

    std::map<Symbol, int> count;
    for (size_t i = 0; i < reels.size(); i++) {
        Symbol symbol = reels[i].symbols[1]; 
        count[symbol]++;
    }

    Symbol winningSymbol = Symbol::Strawberry; 
    int maxCount = 0;

    for (const auto& pair : count) {
        if (pair.second >= 2) { 
            if (pair.second > maxCount || (pair.second == maxCount && rewardTable[pair.first] > rewardTable[winningSymbol])) {
                winningSymbol = pair.first;
                maxCount = pair.second;
            }
        }
    }

    if (maxCount < 2) {
        std::cout << "NO WIN (All symbols different)" << std::endl;
        return;
    }

    int winAmount = (maxCount == 4) ? rewardTable[winningSymbol] : rewardTable[winningSymbol] / 2;
    score += winAmount;

    std::cout << "WIN! Score: " << score << " (" << maxCount << "x " << static_cast<int>(winningSymbol) << ")" << std::endl;

    scoreText.setString(std::to_string(score));
}

void SlotMachine::resetState() {
    reelIndex = 0;
    stopClock.restart();

    for (auto& reel : reels) {
        reel.spinning = false;
        reel.speed = 0.0f;
    }

    std::cout << "Reset completed! Ready for new spin.\n";
}

void SlotMachine::changeState(State* newState) {
    if (currentState) {
        currentState->exit(*this);
        delete currentState;
    }
    currentState = newState;
    currentState->enter(*this);
}