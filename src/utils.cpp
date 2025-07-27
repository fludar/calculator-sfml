#include "../include/utils.h"
#include "../include/math.h"

void utils::fnTextWrap(sf::Text& txtCalc)
{
    unsigned int uFontSize = txtCalc.getCharacterSize();
    if (txtCalc.getLocalBounds().size.x > 250.0f && uFontSize > 10.0f) {
        txtCalc.setCharacterSize(uFontSize - 5);
    }
    else if (txtCalc.getLocalBounds().size.x < 180.0f && uFontSize < 80.0f) {
        txtCalc.setCharacterSize(uFontSize + 5);
    }
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
			math::fnShuntingYard(strCalcText);
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

void utils::fnHandleButtons(
    sf::RenderWindow& rwWindow,
    std::vector<sf::RectangleShape>& vecButtons,
    std::string& strCalcText,
    const sf::Color& colActive,
    const sf::Color& colBtn)
{
    const sf::Color colSecondaryBtn(90, 118, 132, 255);

    const std::vector<std::pair<std::vector<sf::Keyboard::Key>, std::string>> buttonDefs = {
        {{sf::Keyboard::Key::Num7, sf::Keyboard::Key::Numpad7}, "7"},
        {{sf::Keyboard::Key::Num8, sf::Keyboard::Key::Numpad8}, "8"},
        {{sf::Keyboard::Key::Num9, sf::Keyboard::Key::Numpad9}, "9"},
        {{sf::Keyboard::Key::Slash}, "/"},

        {{sf::Keyboard::Key::Num4, sf::Keyboard::Key::Numpad4}, "4"},
        {{sf::Keyboard::Key::Num5, sf::Keyboard::Key::Numpad5}, "5"},
        {{sf::Keyboard::Key::Num6, sf::Keyboard::Key::Numpad6}, "6"},
        {{sf::Keyboard::Key::Multiply}, "*"},

        {{sf::Keyboard::Key::Num1, sf::Keyboard::Key::Numpad1}, "1"},
        {{sf::Keyboard::Key::Num2, sf::Keyboard::Key::Numpad2}, "2"},
        {{sf::Keyboard::Key::Num3, sf::Keyboard::Key::Numpad3}, "3"},
        {{sf::Keyboard::Key::Hyphen, sf::Keyboard::Key::Subtract}, "-"},

        {{sf::Keyboard::Key::Num0, sf::Keyboard::Key::Numpad0}, "0"},
        {{sf::Keyboard::Key::Period, sf::Keyboard::Key::Comma}, "."},
        {{sf::Keyboard::Key::Backspace}, "←"},
        {{sf::Keyboard::Key::Add}, "+"},

        {{sf::Keyboard::Key::Enter, sf::Keyboard::Key::Equal}, "="},
        {{sf::Keyboard::Key::RBracket}, "("},
        {{sf::Keyboard::Key::LBracket}, ")"},
        {{}, "AC"}
    };

    for (size_t i = 0; i < vecButtons.size(); ++i)
    {
        const auto& [keys, label] = buttonDefs[i];
        const sf::Color& colThisDefault = (i % 4 == 3 || i == 19) ? colSecondaryBtn : colBtn;
        fnHandleButton(rwWindow, vecButtons[i], keys, colActive, colThisDefault, strCalcText, label);
    }
}
