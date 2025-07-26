#include <SFML/Graphics.hpp>

void Equal(std::string& calcTextStr){

	for (int i = 0; i < calcTextStr.size(); i++)
	{
        switch (calcTextStr[i]) {
        case '/':
            calcTextStr = "division";
			return;
		case '*':
			calcTextStr = "multiplication";
			return;
		case '-':
			calcTextStr = "subtraction";
			return;
		case '+':
			calcTextStr = "addition";
			return;
        }
	}
}

void handleButtonPress(
    sf::RenderWindow& window,
    sf::RectangleShape& button,
    const std::vector<sf::Keyboard::Key>& keys,
    const sf::Color& activeColor,
    const sf::Color& defaultColor,
    std::string& calcTextStr,
    const std::string& buttonValue)
{
    static std::unordered_map<std::string, bool> wasPressed;

    bool isKeyPressed = std::any_of(keys.begin(), keys.end(), [](sf::Keyboard::Key key) {
        return sf::Keyboard::isKeyPressed(key);
        });

    bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
        button.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));

    if ((isKeyPressed || isMousePressed) && !wasPressed[buttonValue]) {
        button.setFillColor(activeColor);

        if (buttonValue == "AC") {
            calcTextStr.clear();
        }
        else if (buttonValue == "←") {
        if (!calcTextStr.empty()) {
            calcTextStr.pop_back();
            }
        }
        else if (buttonValue == "=") {
            Equal(calcTextStr);
        }
        else calcTextStr += buttonValue; 
        wasPressed[buttonValue] = true;
    }
    else if (!isKeyPressed && !isMousePressed) {
        button.setFillColor(defaultColor);
        wasPressed[buttonValue] = false; 
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode({ 300, 500 }), "Calculator", sf::Style::Close);

	sf::Vector2f padding(10.0f, 10.0f);

    sf::RectangleShape background(sf::Vector2f(400.0f, 600.0f));
	sf::Color bgColor(197, 209, 235, 255);
	background.setFillColor(bgColor);
	background.setPosition(sf::Vector2f(0.0f, 0.0f));

    const sf::Font font("FjallaOne-Regular.ttf");


    std::string calcTextStr = "";
    sf::Text calcText(font);
	sf::Color calcTextColor(57, 91, 80, 255);
    calcText.setCharacterSize(60);
    calcText.setStyle(sf::Text::Regular);
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
	    calcText.setString(calcTextStr);

        handleButtonPress(window, buttons[0], { sf::Keyboard::Key::Num7, sf::Keyboard::Key::Numpad7 }, calcTextColor, btnColor, calcTextStr, "7");
        handleButtonPress(window, buttons[1], { sf::Keyboard::Key::Num8, sf::Keyboard::Key::Numpad8 }, calcTextColor, btnColor, calcTextStr, "8");
        handleButtonPress(window, buttons[2], { sf::Keyboard::Key::Num9, sf::Keyboard::Key::Numpad9 }, calcTextColor, btnColor, calcTextStr, "9");
        handleButtonPress(window, buttons[3], { sf::Keyboard::Key::Slash }, calcTextColor, secondaryBtnColor, calcTextStr, "/");
        handleButtonPress(window, buttons[4], { sf::Keyboard::Key::Num4, sf::Keyboard::Key::Numpad4 }, calcTextColor, btnColor, calcTextStr, "4");
        handleButtonPress(window, buttons[5], { sf::Keyboard::Key::Num5, sf::Keyboard::Key::Numpad5 }, calcTextColor, btnColor, calcTextStr, "5");
        handleButtonPress(window, buttons[6], { sf::Keyboard::Key::Num6, sf::Keyboard::Key::Numpad6 }, calcTextColor, btnColor, calcTextStr, "6");
        handleButtonPress(window, buttons[7], { sf::Keyboard::Key::Multiply }, calcTextColor, secondaryBtnColor, calcTextStr, "*");
        handleButtonPress(window, buttons[8], { sf::Keyboard::Key::Num1, sf::Keyboard::Key::Numpad1 }, calcTextColor, btnColor, calcTextStr, "1");
        handleButtonPress(window, buttons[9], { sf::Keyboard::Key::Num2, sf::Keyboard::Key::Numpad2 }, calcTextColor, btnColor, calcTextStr, "2");
        handleButtonPress(window, buttons[10], { sf::Keyboard::Key::Num3, sf::Keyboard::Key::Numpad3 }, calcTextColor, btnColor, calcTextStr, "3");
        handleButtonPress(window, buttons[11], { sf::Keyboard::Key::Hyphen, sf::Keyboard::Key::Subtract }, calcTextColor, secondaryBtnColor, calcTextStr, "-");
        handleButtonPress(window, buttons[12], { sf::Keyboard::Key::Num0, sf::Keyboard::Key::Numpad0 }, calcTextColor, btnColor, calcTextStr, "0");
        handleButtonPress(window, buttons[13], { sf::Keyboard::Key::Period, sf::Keyboard::Key::Comma }, calcTextColor, btnColor, calcTextStr, ".");
        handleButtonPress(window, buttons[14], { sf::Keyboard::Key::Backspace }, calcTextColor, secondaryBtnColor, calcTextStr, "←");
        handleButtonPress(window, buttons[15], { sf::Keyboard::Key::Add }, calcTextColor, secondaryBtnColor, calcTextStr, "+");
        handleButtonPress(window, buttons[16], {sf::Keyboard::Key::Enter, sf::Keyboard::Key::Equal}, calcTextColor, btnColor, calcTextStr, "=");
        handleButtonPress(window, buttons[17], {}, calcTextColor, secondaryBtnColor, calcTextStr, "AC");


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