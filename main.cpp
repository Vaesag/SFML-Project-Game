#include "include/UI.h"
#include "include/WaitingState.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SlotGames", sf::Style::Titlebar | sf::Style::Close);

    UI ui(window);
    SlotMachine slotMachine(window);

    slotMachine.changeState(new WaitingState());

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        slotMachine.update();
        ui.checkBtn(slotMachine);

        window.clear();

        ui.drawUI();
        slotMachine.draw();
        window.display();
    }

    return 0;
}
