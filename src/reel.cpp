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

    if (!font.loadFromFile("fonts/Arial.ttf")) { // Загружаем шрифт
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

    drawScore(window);
}

void SlotMachine::update() {
    for (auto& reel : reels) {
        if (!reel.spinning) continue;

        // 🔥 Теперь вращаем и `symbols`, и `spriteSymbols`
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


void SlotMachine::printReels() {
    std::cout << "\n--- STATUS REEL ---\n";
    for (size_t i = 0; i < reels.size(); i++) {
        std::cout << "REEL " << i + 1 << " (spinning: " << (reels[i].spinning ? "true" : "false") << "): ";

        for (size_t j = 0; j < reels[i].symbols.size(); j++) { // Выводим символы напрямую
            std::cout << static_cast<int>(reels[i].symbols[j]) << "(" << reels[i].spriteSymbols[j].getPosition().y << ")  ";
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

void SlotMachine::checkWin() {
    if (reels.empty()) return;

    // Таблица наград
    std::map<Symbol, int> rewardTable = {
        {Symbol::Lemon, 1000000},
        {Symbol::Pineapple, 100000},
        {Symbol::Grapes, 10000},
        {Symbol::Strawberry, 1000}
    };

    // Подсчёт символов во втором ряду
    std::map<Symbol, int> count;
    for (size_t i = 0; i < reels.size(); i++) {
        Symbol symbol = reels[i].symbols[1]; // Берём второй символ каждого барабана
        count[symbol]++;
    }

    // Проверяем, есть ли хотя бы 2 одинаковых символа
    Symbol winningSymbol = Symbol::Strawberry; // По умолчанию
    int maxCount = 0;

    for (const auto& pair : count) {
        if (pair.second >= 2) { // Если есть хотя бы 2 одинаковых
            if (pair.second > maxCount || (pair.second == maxCount && rewardTable[pair.first] > rewardTable[winningSymbol])) {
                winningSymbol = pair.first;
                maxCount = pair.second;
            }
        }
    }

    // Если не нашли хотя бы 2 одинаковых символа → нет выигрыша
    if (maxCount < 2) {
        std::cout << "❌ NO WIN (All symbols different)" << std::endl;
        return;
    }

    // Начисляем очки
    int winAmount = (maxCount == 4) ? rewardTable[winningSymbol] : rewardTable[winningSymbol] / 2;
    score += winAmount;

    std::cout << "🎉 WIN! Score: " << score << " (" << maxCount << "x " << static_cast<int>(winningSymbol) << ")" << std::endl;

    scoreText.setString(std::to_string(score)); // Обновляем текст счёта
}




void SlotMachine::drawScore(sf::RenderWindow& window) {
    window.draw(scoreText);
}

void SlotMachine::resetState() {
    reelIndex = 0;  // 🔥 Теперь reelIndex не static и сбрасывается
    stopClock.restart(); // 🔥 Сбрасываем таймер остановки перед новым спином

    for (auto& reel : reels) {
        reel.spinning = false; // 🔥 Все барабаны снова в состоянии покоя
        reel.speed = 0.0f;
    }

    std::cout << "🔄 Reset completed! Ready for new spin.\n";
}
