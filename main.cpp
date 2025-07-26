#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 300, 500 }), "Calculator");

	sf::Vector2f padding(10.0f, 10.0f);

    sf::RectangleShape background(sf::Vector2f(400.0f, 600.0f));
	sf::Color bgColor(197, 209, 235, 255);
	background.setFillColor(bgColor);
	background.setPosition(sf::Vector2f(0.0f, 0.0f));

    const sf::Font font("FjallaOne-Regular.ttf");

    sf::Text calcText(font);
	sf::Color calcTextColor(57, 91, 80, 255);
    calcText.setCharacterSize(60);
    calcText.setStyle(sf::Text::Regular);
	calcText.setString("5*5");
	calcText.setFillColor(calcTextColor);
	calcText.setPosition(sf::Vector2f(0.0f + padding.x, 0.0f + padding.y));

    std::vector<sf::RectangleShape> buttons;
    sf::Vector2f buttonSize(62.5f, 62.5f);
    sf::Color btnColor(146, 175, 215, 255);
    sf::Color secondaryBtnColor(90, 118, 132, 255);

	std::vector<sf::Text> buttonTexts;
	sf::Color buttonTextColor(255, 255, 255, 255);

	float posY = 100.0f;
    for (int i = 0; i < 4; ++i)
    {

        for (int j = 0; j < 4; ++j)
        {
            sf::RectangleShape button(buttonSize);
            button.setFillColor(btnColor);
            if (j == 3) button.setFillColor(secondaryBtnColor);
            button.setPosition(sf::Vector2f(padding.x + j * (buttonSize.x + padding.x), posY + padding.y));
            buttons.push_back(button);
        }
        posY = posY + 75.0f;
    }

    posY = 100.0f;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            sf::Text buttonText(font);
            buttonText.setCharacterSize(18);
            buttonText.setStyle(sf::Text::Regular);
            buttonText.setFillColor(buttonTextColor);
            buttonText.setString(std::to_string(i + 1));
            buttonText.setPosition(sf::Vector2f(padding.x + j * (buttonSize.x + padding.x) + buttonSize.x / 2 - 5, posY + padding.y + buttonSize.y / 2 - 10));
            buttonTexts.push_back(buttonText);
        }
        posY = posY + 75.0f;
    }

    sf::RectangleShape equalsButton(sf::Vector2f(buttonSize.x*3 + 20.0f, buttonSize.y));
	equalsButton.setFillColor(btnColor);
	equalsButton.setPosition(sf::Vector2f(padding.x, posY + padding.y));
	buttons.push_back(equalsButton);

    sf::RectangleShape clearButton(buttonSize);
    clearButton.setFillColor(secondaryBtnColor);
    clearButton.setPosition(sf::Vector2f(padding.x + 3 * (buttonSize.x + padding.x), posY + padding.y));
	buttons.push_back(clearButton);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(background);
		window.draw(calcText);
        for (const auto& button : buttons)
        {
            window.draw(button);
        }
        for (const auto& buttonText : buttonTexts)
        {
            window.draw(buttonText);
        }
        window.display();
    }
}