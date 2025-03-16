#include <SFML/Graphics.hpp>
#include "include/UI.h"
#include "include/reel.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SlotGames", sf::Style::Titlebar | sf::Style::Close);

    UI ui(window);
    SlotMachine slotMachine(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        slotMachine.update();
        ui.update(slotMachine);

        window.clear();
        ui.drawUI();
        slotMachine.draw();
       
        window.display();
    }

    return 0;
}
