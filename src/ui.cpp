#include "../include/ui.h"

void UI::loadTexture(const std::string& key, const std::string& path, sf::IntRect rect, bool smooth) {
    sf::Texture texture;
    if (!texture.loadFromFile(path, rect)) {
        std::cerr << "Ошибка загрузки текстуры: " << path << std::endl;
        return;
    }
    if (smooth) {
        texture.setSmooth(true);
    }
    textures[key] = std::move(texture);
}
 
void UI::setTextures() {
    loadTexture("background", PATH_BACKGROUND);
    loadTexture("stop", PATH_STOP_BTN);
    loadTexture("start", PATH_START_BTN);
    
    background.setTexture(textures["background"]);
    stopBtn.setTexture(textures["stop"]);
    startBtn.setTexture(textures["start"]);
}

UI::UI(sf::RenderWindow& window) : window(window) {

    setTextures();
    background.setPosition(0, 0);

    sf::Vector2u textureSize = textures["stop"].getSize();

    startBtn.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    stopBtn.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);

    startBtn.setPosition(650, 500);
    stopBtn.setPosition(50, 500);
}

void UI::drawUI() {
    window.draw(background);
    window.draw(startBtn);
    window.draw(stopBtn);
}

void UI::update(SlotMachine& slotMachine) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (startBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (gameState == GameState::Waiting) {
                gameState = GameState::Spinning;
                slotMachine.startSpin();
                std::cout << "START SPIN" << std::endl;
            }
        }

        if (stopBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (gameState == GameState::Spinning) {
                gameState = GameState::Stopping;
                stopClock.restart();
                std::cout << "STOP SPIN" << std::endl;
            }
        }
    }

    if (gameState == GameState::Stopping) {
        slotMachine.stopSpin();

        // Теперь используем новый метод вместо прямого доступа к `reels`
        if (slotMachine.areAllReelsStopped()) {
            gameState = GameState::Checking;
            slotMachine.checkWin();
            std::cout << "ALL REELS STOPPED! CHECKING RESULT..." << std::endl;
			slotMachine.printReels();
        }
    }

    if (gameState == GameState::Checking) {
        static sf::Clock checkClock;

        if (checkClock.getElapsedTime().asSeconds() > 3.0f) { 
            gameState = GameState::Waiting;
            checkClock.restart();
            slotMachine.resetState();
            std::cout << "✅ Game reset! Waiting for new spin..." << std::endl;
        }
    }
}
