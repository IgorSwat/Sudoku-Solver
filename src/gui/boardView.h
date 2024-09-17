#pragma once

#include "tile.h"
#include "grid.h"
#include "../logic/board.h"
#include <array>
#include <optional>
#include <tuple>


namespace GUI {

    // ------------------
    // Board (view) class
    // ------------------

    class Board : public sf::Drawable
    {
    public:
        Board(float tileSize, unsigned fontSize, 
              sf::Color innerGridColor, sf::Color outerGridColor, 
              sf::Color tileHighlightColor1, sf::Color tileHighlightColor2);

        void loadNumbers(const Sudoku::Board& board);

        // Might return a (row, col, number) tuple representing the entered number
        std::optional<std::tuple<int, int, int>> update(const sf::Event& event);
        
        void setPosition(sf::Vector2f pos) { position = pos; alignElements(); }
        sf::Vector2f getPosition() const { return position; }
        sf::FloatRect getGlobalBounds() const { return sf::FloatRect(outerGrid.getPosition(), sf::Vector2f(boardSize, boardSize)); }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        bool isAnyTileSelected() const { return selectedTile.first >= 0; }
        void alignElements();
        void updateTileVisibilities();

        // Graphic content
        std::array<std::array<Tile, Sudoku::BOARD_SIZE>, Sudoku::BOARD_SIZE> tiles;
        Grid innerGrid;
        Grid outerGrid;

        // Board parameters & logic
        sf::Vector2f position = {0, 0};
        std::pair<int, int> selectedTile = {-1, -1};

        const float tileSize;
        const float boardSize;
        const sf::Color tileHighlightColor1;
        const sf::Color tileHighlightColor2;
    };

}