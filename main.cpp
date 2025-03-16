#include <SFML/Graphics.hpp>

int main() {
    // Создаем фиксированное окно 800x600
    sf::RenderWindow window(sf::VideoMode(700, 550), "SlotGames", sf::Style::Titlebar | sf::Style::Close);

    // Загружаем текстуру для фона
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.png")) {
        return -1; // Ошибка, если файл не найден
    }

    // Создаем спрайт для фона
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Главный цикл игры
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Отрисовка
        window.clear();
        window.draw(backgroundSprite); // Рисуем фон
        window.display();
    }

    return 0;
}
