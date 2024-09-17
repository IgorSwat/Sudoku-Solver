#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


namespace GUI {

    // ----------
    // Grid class
    // ----------

    class Grid : public sf::Drawable
    {
    public:
        Grid(float gridSize, float cellSize, float thickness, sf::Color color = sf::Color::Black);

        void setPosition(sf::Vector2f pos) { position = pos; arrangeLines(); }
        sf::Vector2f getPosition() const { return position; }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        void arrangeLines();

        // Graphic content
        std::vector<sf::RectangleShape> lines;

        // Grid parameters
        sf::Vector2f position = {0, 0};      // Top left corner of grid
        const float gridSize;
        const float cellSize;
        const float thickness;
        const int noLines;                   // Number of lines in one dimension. Total number of lines equals to (2 * noLines).
    };

}