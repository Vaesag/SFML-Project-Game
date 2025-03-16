#include <SFML/Graphics.hpp>
#include "include/UI.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SlotGames", sf::Style::Titlebar | sf::Style::Close);

    UI ui(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ui.update();

        window.clear();
        ui.drawUI();
        window.display();
    }

    return 0;
}
