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

void UI::update() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (startBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (gameState == GameState::Waiting) {
                gameState = GameState::Spinning;
                std::cout << "Start Reels Rotate!" << std::endl;
            }
        }
        if (stopBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (gameState == GameState::Spinning) {
                gameState = GameState::Stopping; // Останавливаем барабаны
                std::cout << "Stop Reels Rotate!" << std::endl;
            }
        }
    }
    switch (gameState) {
    case GameState::Waiting:
        // Ожидание нажатия кнопки "Start"
        break;
    case GameState::Spinning:
        // Барабаны крутятся (позже добавим)
        break;
    case GameState::Stopping:
        // Барабаны останавливаются (позже добавим)
        if (stopClock.getElapsedTime().asSeconds() > 5) {
            std::cout << "End of Rotate" << std::endl;
            gameState = GameState::Checking;
        }
        break;
    case GameState::Checking:
        if (checkClock.getElapsedTime().asSeconds() > 10) {
            std::cout << "Your WIN!" << std::endl;
			gameState = GameState::Waiting;
        }
        // Проверяем выигрышные комбинации
        break;
    }
}