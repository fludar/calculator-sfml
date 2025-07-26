#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 300, 500 }), "Calculator", sf::Style::Close);

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
	std::unordered_map<int, std::string> buttonLabels = {
		{0, "7"}, {1, "8"}, {2, "9"}, {3, "/"},
		{4, "4"}, {5, "5"}, {6, "6"}, {7, "*"},
		{8, "1"}, {9, "2"}, {10, "3"}, {11, "-"},
		{12, "0"}, {13, "."}, {14, "as"}, {15, "+"}
	};

    posY = 100.0f;
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            sf::Text buttonText(font);
            buttonText.setCharacterSize(18);
            buttonText.setStyle(sf::Text::Regular);
            buttonText.setFillColor(buttonTextColor);
            buttonText.setString(buttonLabels[count]);
            if (count == 14) {
                buttonText.setString(sf::String(L"←"));
            }
            buttonText.setPosition(sf::Vector2f(padding.x + j * (buttonSize.x + padding.x) + buttonSize.x / 2 - 5, posY + padding.y + buttonSize.y / 2 - 10));
            buttonTexts.push_back(buttonText);
			count++;
        }
        posY = posY + 75.0f;
    }

    sf::RectangleShape equalsButton(sf::Vector2f(buttonSize.x*3 + 20.0f, buttonSize.y));
	equalsButton.setFillColor(btnColor);
	equalsButton.setPosition(sf::Vector2f(padding.x, posY + padding.y));
	buttons.push_back(equalsButton);

	sf::Text equalsText(font);
	equalsText.setCharacterSize(40);
	equalsText.setStyle(sf::Text::Regular);
	equalsText.setFillColor(buttonTextColor);
	equalsText.setString("=");
    equalsText.setPosition(sf::Vector2f(padding.x + (buttonSize.x * 3) / 2, posY - 15.0f + buttonSize.y / 2));
	buttonTexts.push_back(equalsText);

    sf::RectangleShape clearButton(buttonSize);
    clearButton.setFillColor(secondaryBtnColor);
    clearButton.setPosition(sf::Vector2f(padding.x + 3 * (buttonSize.x + padding.x), posY + padding.y));
	buttons.push_back(clearButton);

	sf::Text clearText(font);
	clearText.setCharacterSize(18);
    clearText.setStyle(sf::Text::Regular);
    clearText.setFillColor(buttonTextColor);
    clearText.setString("AC");
    clearText.setPosition(sf::Vector2f(padding.x + (buttonSize.x * 8 - 22.0f) / 2, posY - 5.0f + padding.y / 2 + buttonSize.y / 2));
    buttonTexts.push_back(clearText);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad7))
			buttons[0].setFillColor(calcTextColor);
		else
			buttons[0].setFillColor(btnColor);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad8))
			buttons[1].setFillColor(calcTextColor);
        else 
			buttons[1].setFillColor(btnColor);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad9))
		    buttons[2].setFillColor(calcTextColor);
        else
			buttons[2].setFillColor(btnColor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Slash) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Slash))
			buttons[3].setFillColor(calcTextColor);
        else
			buttons[3].setFillColor(secondaryBtnColor);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad4))
			buttons[4].setFillColor(calcTextColor);
		else
			buttons[4].setFillColor(btnColor);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad5))
			buttons[5].setFillColor(calcTextColor);
		else
			buttons[5].setFillColor(btnColor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad6))
			buttons[6].setFillColor(calcTextColor);
		else
			buttons[6].setFillColor(btnColor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Multiply))
			buttons[7].setFillColor(calcTextColor);
		else
			buttons[7].setFillColor(secondaryBtnColor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad1))
			buttons[8].setFillColor(calcTextColor);
        else
			buttons[8].setFillColor(btnColor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2))
			buttons[9].setFillColor(calcTextColor);
        else
			buttons[9].setFillColor(btnColor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad3))
			buttons[10].setFillColor(calcTextColor);
        else
			buttons[10].setFillColor(btnColor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Hyphen) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract))
			buttons[11].setFillColor(calcTextColor);
        else
			buttons[11].setFillColor(secondaryBtnColor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad0))
			buttons[12].setFillColor(calcTextColor);
        else
			buttons[12].setFillColor(btnColor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Period) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Comma))
			buttons[13].setFillColor(calcTextColor);
        else
			buttons[13].setFillColor(btnColor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace))
			buttons[14].setFillColor(calcTextColor);
        else
			buttons[14].setFillColor(secondaryBtnColor);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add))
			buttons[15].setFillColor(calcTextColor);
        else 
			buttons[15].setFillColor(secondaryBtnColor);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::NumpadEnter) 
            || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal))
			buttons[16].setFillColor(calcTextColor);
        else
			buttons[16].setFillColor(btnColor);



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