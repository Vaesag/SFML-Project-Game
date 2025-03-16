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
    printReels();
}

void SlotMachine::startSpin() {
    for (auto& reel : reels) {
        reel.spinning = true;
        reel.speed = 1.0f + rand() % 1;
    }
    spinClock.restart();
}

void SlotMachine::stopSpin() {
    static size_t reelIndex = 0; 

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
}

void SlotMachine::update() {
    for (auto& reel : reels) {
        if (!reel.spinning) continue;

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

void SlotMachine::printReels() {
    std::cout << "\n--- STATUS REEL ---\n";
    for (size_t i = 0; i < reels.size(); i++) {
        std::cout << "REEL " << i + 1 << " (spinning: " << (reels[i].spinning ? "true" : "false") << "): ";

        for (auto& sprite : reels[i].spriteSymbols) {
            float y = sprite.getPosition().y;

            // Определяем, какой символ находится в ячейке (по текстуре)
            for (auto& pair : symbolTextures) {
                if (sprite.getTexture() == &pair.second) {
                    std::cout << static_cast<int>(pair.first) << "(" << y << ")  ";
                    break;
                }
            }
        }
        std::cout << "\n";
    }
}

bool SlotMachine::areAllReelsStopped() const {
    for (const auto& reel : reels) {
        if (reel.spinning) {
            return false; 
        }
    }
    return true; 
}