#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

namespace utils {
	void fnTextWrap(sf::Text& txtCalc);
    void fnHandleButtons(
        sf::RenderWindow& window,
        std::vector<sf::RectangleShape>& buttons,
        std::string& calcText,
        const sf::Color& activeColor,
        const sf::Color& defaultColor);
}