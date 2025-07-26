#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <deque>

int fnEqual(std::string& strCalcText) {
    struct sOperator {
        uint8_t precedence = 0;
        uint8_t arguments = 0;
    };

    std::unordered_map<char, sOperator> mapOps;
    mapOps['/'] = { 4, 2 };
    mapOps['*'] = { 3, 2 };
    mapOps['+'] = { 2, 2 };
    mapOps['-'] = { 1, 2 };

    struct sSymbol
	{
		std::string symbol = "";

		enum class Type : uint8_t
		{
			Unknown,
			Literal_Numeric,
			Operator,
			Parenthesis_Open,
			Parenthesis_Close
		} type = Type::Unknown;

		sOperator op;
	};

    std::deque<sSymbol> stkHolding;
    std::deque<sSymbol> stkOutput;

    sSymbol symPrevious = { "0", sSymbol::Type::Literal_Numeric, 0, 0 };
    int pass = 0;
    for (const char c : strCalcText)
    {
        if (std::isdigit(c))
        {
            // Push literals straight to output, they are already in order
            stkOutput.push_back({ std::string(1, c), sSymbol::Type::Literal_Numeric });
            symPrevious = stkOutput.back();
        }
        else if (c == '(')
        {
            // Push to holding stack, it acts as a stopper when we back track
            stkHolding.push_front({ std::string(1, c), sSymbol::Type::Parenthesis_Open });
            symPrevious = stkHolding.front();
        }
        else if (c == ')')
        {
            // Backflush holding stack into output until open parenthesis
            while (!stkHolding.empty() && stkHolding.front().type != sSymbol::Type::Parenthesis_Open)
            {
                stkOutput.push_back(stkHolding.front());
                stkHolding.pop_front();
            }

            if (stkHolding.empty())
            {
                strCalcText = "ERROR";
                return 0;
            }

            // Remove corresponding open parenthesis from holding stack
            if (!stkHolding.empty() && stkHolding.front().type == sSymbol::Type::Parenthesis_Open)
            {
                stkHolding.pop_front();
            }

            symPrevious = { std::string(1, c), sSymbol::Type::Parenthesis_Close };

        }
        else if (mapOps.contains(c))
        {
            // Symbol is operator
            sOperator new_op = mapOps[c];

            if (c == '-' || c == '+')
            {
                if ((symPrevious.type != sSymbol::Type::Literal_Numeric
                    && symPrevious.type != sSymbol::Type::Parenthesis_Close) || pass == 0)
                {
                    new_op.arguments = 1;
                    new_op.precedence = 100;
                }
            }

            while (!stkHolding.empty() && stkHolding.front().type != sSymbol::Type::Parenthesis_Open)
            {
                // Ensure holding stack front is an operator (it might not be later...)
                if (stkHolding.front().type == sSymbol::Type::Operator)
                {
                    const auto& holding_stack_op = stkHolding.front().op;

                    if (holding_stack_op.precedence >= new_op.precedence)
                    {
                        stkOutput.push_back(stkHolding.front());
                        stkHolding.pop_front();
                    }
                    else
                        break;
                }
            }

            // Push the new operator onto the holding stack
            stkHolding.push_front({ std::string(1, c), sSymbol::Type::Operator, new_op });
            symPrevious = stkHolding.front();
        }
        else
        {
			strCalcText = "Bad Symbol";
            return 0;
        }

        pass++;
    }

    // Drain the holding stack
    while (!stkHolding.empty())
    {
        stkOutput.push_back(stkHolding.front());
        stkHolding.pop_front();
    }

    // Solver
    std::deque<double> stkSolve;

    for (const auto& inst : stkOutput)
    {
        switch (inst.type)
        {
        case sSymbol::Type::Literal_Numeric:
        {
            stkSolve.push_front(std::stod(inst.symbol));
        }
        break;

        case sSymbol::Type::Operator:
        {
            std::vector<double> mem(inst.op.arguments);
            for (uint8_t a = 0; a < inst.op.arguments; a++)
            {
                if (stkSolve.empty())
                {
                    strCalcText = "Bad Expression";
                }
                else
                {
                    mem[a] = stkSolve[0];
                    stkSolve.pop_front();
                }
            }

            double result = 0.0;
            if (inst.op.arguments == 2)
            {
                if (inst.symbol[0] == '/') result = mem[1] / mem[0];
                if (inst.symbol[0] == '*') result = mem[1] * mem[0];
                if (inst.symbol[0] == '+') result = mem[1] + mem[0];
                if (inst.symbol[0] == '-') result = mem[1] - mem[0];
            }

            if (inst.op.arguments == 1)
            {
                if (inst.symbol[0] == '+') result = +mem[0];
                if (inst.symbol[0] == '-') result = -mem[0];
            }


            stkSolve.push_front(result);
        }
        break;

        }
    }

    strCalcText = std::to_string(stkSolve[0]);
}

void fnHandleButton(
    sf::RenderWindow& rwWindow,
    sf::RectangleShape& rsButton,
    const std::vector<sf::Keyboard::Key>& vecKeys,
    const sf::Color& colActive,
    const sf::Color& colDefault,
    std::string& strCalcText,
    const std::string& strButtonVal)
{
    static std::unordered_map<std::string, bool> mapWasPressed;

    bool bKeyPressed = std::any_of(vecKeys.begin(), vecKeys.end(), [](sf::Keyboard::Key key) {
        return sf::Keyboard::isKeyPressed(key);
        });

    bool bMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
        rsButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(rwWindow)));

    if ((bKeyPressed || bMousePressed) && !mapWasPressed[strButtonVal]) {
        rsButton.setFillColor(colActive);

        if (strButtonVal == "AC") {
            strCalcText.clear();
        }
        else if (strButtonVal == "←") {
            if (!strCalcText.empty()) {
                strCalcText.pop_back();
            }
        }
        else if (strButtonVal == "=") {
            fnEqual(strCalcText);
        }
        else {
            strCalcText += strButtonVal;
        }
        mapWasPressed[strButtonVal] = true;
    }
    else if (!bKeyPressed && !bMousePressed) {
        rsButton.setFillColor(colDefault);
        mapWasPressed[strButtonVal] = false;
    }
}

void fnTextWrap(sf::Text& txtCalc)
{
    unsigned int uFontSize = txtCalc.getCharacterSize();
    if (txtCalc.getLocalBounds().size.x > 250.0f && uFontSize > 10.0f) {
        txtCalc.setCharacterSize(uFontSize - 5);
    }
    else if (txtCalc.getLocalBounds().size.x < 180.0f && uFontSize < 80.0f) {
        txtCalc.setCharacterSize(uFontSize + 5);
    }
}

int main()
{
    sf::RenderWindow rwWindow(sf::VideoMode({ 300, 500 }), "Calculator", sf::Style::Close);
    sf::Vector2f vec2Padding(10.0f, 10.0f);

    sf::RectangleShape rsBackground(sf::Vector2f(400.0f, 600.0f));
    sf::Color colBg(197, 209, 235, 255);
    rsBackground.setFillColor(colBg);
    rsBackground.setPosition(sf::Vector2f(0.0f, 0.0f));

    const sf::Font fntMain("worksans.ttf");

    std::string strCalcText = "";
    sf::Text txtCalc(fntMain);
    sf::Color colCalcText(57, 91, 80, 255);
    txtCalc.setCharacterSize(80);
    txtCalc.setStyle(sf::Text::Regular);
    txtCalc.setFillColor(colCalcText);
    txtCalc.setPosition(sf::Vector2f(0.0f + vec2Padding.x, 0.0f + vec2Padding.y));

    std::vector<sf::RectangleShape> vecButtons;
    sf::Vector2f vec2ButtonSize(62.5f, 62.5f);
    sf::Color colBtn(146, 175, 215, 255);
    sf::Color colSecondaryBtn(90, 118, 132, 255);

    std::vector<sf::Text> vecButtonTexts;
    sf::Color colButtonText(255, 255, 255, 255);

    float fPosY = 100.0f;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            sf::RectangleShape rsButton(vec2ButtonSize);
            rsButton.setFillColor(colBtn);
            if (j == 3) rsButton.setFillColor(colSecondaryBtn);
            rsButton.setPosition(sf::Vector2f(vec2Padding.x + j * (vec2ButtonSize.x + vec2Padding.x), fPosY + vec2Padding.y));
            vecButtons.push_back(rsButton);
        }
        fPosY += 75.0f;
    }

    std::unordered_map<int, std::wstring> mapButtonLabels = {
        {0, L"7"}, {1, L"8"}, {2, L"9"}, {3, L"/"},
        {4, L"4"}, {5, L"5"}, {6, L"6"}, {7, L"*"},
        {8, L"1"}, {9, L"2"}, {10, L"3"}, {11, L"-"},
        {12, L"0"}, {13, L"."}, {14, L"←"}, {15, L" + "},
        {16, L"="}, {17, L"("}, {18, L")"}, {19, L"AC"}
    };

    fPosY = 100.0f;
    int iCount = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            sf::Text txtButton(fntMain);
            txtButton.setCharacterSize(18);
            txtButton.setStyle(sf::Text::Regular);
            txtButton.setFillColor(colButtonText);
            txtButton.setString(mapButtonLabels[iCount]);
            txtButton.setPosition(sf::Vector2f(
                vec2Padding.x + j * (vec2ButtonSize.x + vec2Padding.x) + vec2ButtonSize.x / 2 - 5,
                fPosY + vec2Padding.y + vec2ButtonSize.y / 2 - 10));
            vecButtonTexts.push_back(txtButton);
            iCount++;
        }
        fPosY += 75.0f;
    }

    vecButtonTexts[15].setPosition(sf::Vector2f(vecButtonTexts[15].getPosition().x - 7.5f, vecButtonTexts[15].getPosition().y));
    vecButtonTexts[19].setPosition(sf::Vector2f(vecButtonTexts[19].getPosition().x - 7.5f, vecButtonTexts[19].getPosition().y));

    while (rwWindow.isOpen()) {
        while (const std::optional event = rwWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                rwWindow.close();
        }

        txtCalc.setString(strCalcText);

        fnHandleButton(rwWindow, vecButtons[0], { sf::Keyboard::Key::Num7, sf::Keyboard::Key::Numpad7 }, colCalcText, colBtn, strCalcText, "7");
        fnHandleButton(rwWindow, vecButtons[1], { sf::Keyboard::Key::Num8, sf::Keyboard::Key::Numpad8 }, colCalcText, colBtn, strCalcText, "8");
        fnHandleButton(rwWindow, vecButtons[2], { sf::Keyboard::Key::Num9, sf::Keyboard::Key::Numpad9 }, colCalcText, colBtn, strCalcText, "9");
        fnHandleButton(rwWindow, vecButtons[3], { sf::Keyboard::Key::Slash }, colCalcText, colSecondaryBtn, strCalcText, "/");
        fnHandleButton(rwWindow, vecButtons[4], { sf::Keyboard::Key::Num4, sf::Keyboard::Key::Numpad4 }, colCalcText, colBtn, strCalcText, "4");
        fnHandleButton(rwWindow, vecButtons[5], { sf::Keyboard::Key::Num5, sf::Keyboard::Key::Numpad5 }, colCalcText, colBtn, strCalcText, "5");
        fnHandleButton(rwWindow, vecButtons[6], { sf::Keyboard::Key::Num6, sf::Keyboard::Key::Numpad6 }, colCalcText, colBtn, strCalcText, "6");
        fnHandleButton(rwWindow, vecButtons[7], { sf::Keyboard::Key::Multiply }, colCalcText, colSecondaryBtn, strCalcText, "*");
        fnHandleButton(rwWindow, vecButtons[8], { sf::Keyboard::Key::Num1, sf::Keyboard::Key::Numpad1 }, colCalcText, colBtn, strCalcText, "1");
        fnHandleButton(rwWindow, vecButtons[9], { sf::Keyboard::Key::Num2, sf::Keyboard::Key::Numpad2 }, colCalcText, colBtn, strCalcText, "2");
        fnHandleButton(rwWindow, vecButtons[10], { sf::Keyboard::Key::Num3, sf::Keyboard::Key::Numpad3 }, colCalcText, colBtn, strCalcText, "3");
        fnHandleButton(rwWindow, vecButtons[11], { sf::Keyboard::Key::Hyphen, sf::Keyboard::Key::Subtract }, colCalcText, colSecondaryBtn, strCalcText, "-");
        fnHandleButton(rwWindow, vecButtons[12], { sf::Keyboard::Key::Num0, sf::Keyboard::Key::Numpad0 }, colCalcText, colBtn, strCalcText, "0");
        fnHandleButton(rwWindow, vecButtons[13], { sf::Keyboard::Key::Period, sf::Keyboard::Key::Comma }, colCalcText, colBtn, strCalcText, ".");
        fnHandleButton(rwWindow, vecButtons[14], { sf::Keyboard::Key::Backspace }, colCalcText, colBtn, strCalcText, "←");
        fnHandleButton(rwWindow, vecButtons[15], { sf::Keyboard::Key::Add }, colCalcText, colSecondaryBtn, strCalcText, "+");
        fnHandleButton(rwWindow, vecButtons[16], { sf::Keyboard::Key::Enter, sf::Keyboard::Key::Equal }, colCalcText, colBtn, strCalcText, "=");
        fnHandleButton(rwWindow, vecButtons[17], { sf::Keyboard::Key::RBracket }, colCalcText, colBtn, strCalcText, "(");
        fnHandleButton(rwWindow, vecButtons[18], { sf::Keyboard::Key::LBracket }, colCalcText, colBtn, strCalcText, ")");
        fnHandleButton(rwWindow, vecButtons[19], {}, colCalcText, colSecondaryBtn, strCalcText, "AC");

        rwWindow.clear();
        rwWindow.draw(rsBackground);

        fnTextWrap(txtCalc);
        rwWindow.draw(txtCalc);

        for (const auto& rsButton : vecButtons)
            rwWindow.draw(rsButton);

        for (const auto& txtButton : vecButtonTexts)
            rwWindow.draw(txtButton);

        rwWindow.display();
    }
}
