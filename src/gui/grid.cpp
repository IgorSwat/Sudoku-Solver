#include "grid.h"


namespace GUI {

    // ------------
    // Grid methods
    // ------------

    Grid::Grid(float gridSize, float cellSize, float thickness, sf::Color color)
        : gridSize(gridSize), cellSize(cellSize), thickness(thickness),
          noLines(static_cast<int>(gridSize / cellSize) + 1)
    {
        // Create vertical lines
        for (int i = 0; i < noLines; i++) {
            lines.emplace_back(sf::Vector2f(thickness, gridSize + thickness));
            lines.back().setFillColor(color);
        }

        // Create horizontal lines
        for (int i = 0; i < noLines; i++) {
            lines.emplace_back(sf::Vector2f(gridSize + thickness, thickness));
            lines.back().setFillColor(color);
        }
    }

    void Grid::arrangeLines()
    {
        for (int i = 0; i < noLines; i++) {
            // Vertical line
            lines[i].setPosition(position.x + cellSize * i - thickness / 2, position.y - thickness / 2);
            // Horizontal line
            lines[noLines + i].setPosition(position.x - thickness / 2, position.y + cellSize * i - thickness / 2);
        }
    }

    void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for (const sf::RectangleShape& line : lines)
            target.draw(line, states);
    }

}