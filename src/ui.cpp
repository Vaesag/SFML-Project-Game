#include "../include/ui.h"
#include "../include/SpinningState.h"
#include "../include/StoppingState.h"

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

void UI::checkBtn(SlotMachine& slotMachine) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (startBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (dynamic_cast<WaitingState*>(slotMachine.getCurrentState())) {
                slotMachine.changeState(new SpinningState());
                std::cout << "Start button pressed!" << std::endl;
            }
        }

        if (stopBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (dynamic_cast<SpinningState*>(slotMachine.getCurrentState())) {
                slotMachine.changeState(new StoppingState());
                std::cout << "Stop button pressed!" << std::endl;
            }
        }
    }
}
