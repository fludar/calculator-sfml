#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 300, 500 }), "Calculator");

    sf::RectangleShape background(sf::Vector2f(300.0f, 500.0f));

	background.setFillColor(sf::Color::Cyan);
	background.setPosition(sf::Vector2f(0.0f, 0.0f));
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(background);
        window.display();
    }
}