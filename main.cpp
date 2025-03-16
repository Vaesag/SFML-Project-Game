#include <SFML/Graphics.hpp>

int main() {
    // ������� ������������� ���� 800x600
    sf::RenderWindow window(sf::VideoMode(700, 550), "SlotGames", sf::Style::Titlebar | sf::Style::Close);

    // ��������� �������� ��� ����
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.png")) {
        return -1; // ������, ���� ���� �� ������
    }

    // ������� ������ ��� ����
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // ������� ���� ����
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // ���������
        window.clear();
        window.draw(backgroundSprite); // ������ ���
        window.display();
    }

    return 0;
}
