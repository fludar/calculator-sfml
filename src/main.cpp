#include "../include/utils.h"

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

        utils::fnHandleButtons(rwWindow, vecButtons, strCalcText, colCalcText, colBtn);

        rwWindow.clear();
        rwWindow.draw(rsBackground);

        utils::fnTextWrap(txtCalc);
        rwWindow.draw(txtCalc);

        for (const auto& rsButton : vecButtons)
            rwWindow.draw(rsButton);

        for (const auto& txtButton : vecButtonTexts)
            rwWindow.draw(txtButton);

        rwWindow.display();
    }
}
