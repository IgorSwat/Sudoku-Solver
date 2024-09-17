#include "boardView.h"


namespace GUI {

    // Customizable parameters
    constexpr float INNER_GRID_THICKNESS_RATIO = 0.04f;
    constexpr float OUTER_GRID_THICKNESS_RATIO = 0.10f;

    const sf::Color TILE_DEFAULT_COLOR = sf::Color::White;


    // -------------------------------------
    // Board (view) methods - initialization
    // -------------------------------------

    Board::Board(float tileSize, unsigned fontSize,
                 sf::Color innerGridColor, sf::Color outerGridColor,
                 sf::Color tileHighlightColor1, sf::Color tileHighlightColor2)
        : innerGrid(tileSize * Sudoku::BOARD_SIZE, tileSize, INNER_GRID_THICKNESS_RATIO * tileSize, innerGridColor),
          outerGrid(tileSize * Sudoku::BOARD_SIZE, tileSize * 3, OUTER_GRID_THICKNESS_RATIO * tileSize, outerGridColor),
          tileSize(tileSize), boardSize(tileSize * Sudoku::BOARD_SIZE),
          tileHighlightColor1(tileHighlightColor1), tileHighlightColor2(tileHighlightColor2)
    {
        for (auto& row : tiles) {
            for (auto& tile : row)
                tile = Tile(tileSize, fontSize);
        }

        alignElements();
    }


    // -------------------------------------
    // Board (view) methods - event handling
    // -------------------------------------

    std::optional<std::tuple<int, int, int>> Board::update(const sf::Event& event)
    {
        // Case 1 - left mouse click
        // Activates new tile
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::FloatRect bounds = getGlobalBounds();
            sf::Vector2f mousePos = {static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)};

            // Find the clicked tile (if any was clicked)
            int tr = bounds.contains(mousePos) ? static_cast<int>((mousePos.x - bounds.left) / tileSize) : -1;
            int tc = bounds.contains(mousePos) ? static_cast<int>((mousePos.y - bounds.top) / tileSize) : -1;

            // Update clicked tile and colors
            if (tr != selectedTile.first || tc != selectedTile.second)
                selectedTile = {tr, tc};
            else
                selectedTile = {-1, -1};
            updateTileVisibilities();
        }
        // Case 2 - entering a number
        // Change text of the clicked tile or do nothing if no tile has been clicked
        else if (event.type == sf::Event::TextEntered && event.text.unicode >= 49 && event.text.unicode <= 57 && isAnyTileSelected()) {
            int num = event.text.unicode - 48;
            auto result = std::make_tuple(selectedTile.first, selectedTile.second, num);

            // Update visualities
            tiles[selectedTile.first][selectedTile.second].setNumber(num);
            updateTileVisibilities();

            return std::make_optional(result);
        }
        // Case 3 - other keys clicked
        // Either Chnges the selected tile (after arrow click) or remove the number from selected tile (after backspace or delete)
        else if (event.type == sf::Event::KeyPressed && isAnyTileSelected()) {
            auto result = std::make_tuple(-1, -1, -1);

            // Update selected tile
            switch (event.key.code) {
                case sf::Keyboard::Left:
                    selectedTile = {std::max(selectedTile.first - 1, 0), selectedTile.second};
                    break;
                case sf::Keyboard::Up:
                    selectedTile = {selectedTile.first, std::max(selectedTile.second - 1, 0)};
                    break;
                case sf::Keyboard::Right:
                    selectedTile = {std::min(selectedTile.first + 1, Sudoku::BOARD_SIZE - 1), selectedTile.second};
                    break;
                case sf::Keyboard::Down:
                    selectedTile = {selectedTile.first, std::min(selectedTile.second + 1, Sudoku::BOARD_SIZE - 1)};
                    break;
                case sf::Keyboard::BackSpace:
                case sf::Keyboard::Delete:
                    result = std::make_tuple(selectedTile.first, selectedTile.second, 0);
                    tiles[selectedTile.first][selectedTile.second].removeNumber();
                    break;
                default:
                    break;
            }

            // Update visualities
            updateTileVisibilities();

            if (std::get<0>(result) >= 0)
                return std::make_optional(result);
        }

        return std::nullopt;
    }


    // -------------------------------------
    // Board (view) methods - element update
    // -------------------------------------

    void Board::loadNumbers(const Sudoku::Board& board)
    {
        for (int r = 0; r < Sudoku::BOARD_SIZE; r++) {
            for (int c = 0; c < Sudoku::BOARD_SIZE; c++)
                tiles[r][c].setNumber(board.getNumber(r, c));
        }
    }

    void Board::alignElements()
    {
        // Align grid
        innerGrid.setPosition(position);
        outerGrid.setPosition(position);

        // Align tiles
        for (int r = 0; r < Sudoku::BOARD_SIZE; r++) {
            for (int c = 0; c < Sudoku::BOARD_SIZE; c++)
                tiles[r][c].setPosition(sf::Vector2f(position.x + tileSize * r, position.y + tileSize * c));
        }
    }

    void Board::updateTileVisibilities()
    {
        int is = selectedTile.first >= 0 ? Sudoku::Board::innerSquare(selectedTile.first, selectedTile.second) : -1;

        for (int r = 0; r < Sudoku::BOARD_SIZE; r++) {
            for (int c = 0; c < Sudoku::BOARD_SIZE; c++) {
                if (r == selectedTile.first && c == selectedTile.second)
                    tiles[r][c].setBackgroundColor(tileHighlightColor1);
                else if (r == selectedTile.first || c == selectedTile.second || is == Sudoku::Board::innerSquare(r, c))
                    tiles[r][c].setBackgroundColor(tileHighlightColor2);
                else
                    tiles[r][c].setBackgroundColor(TILE_DEFAULT_COLOR);
            }
        }
    }


    // ------------------------------------
    // Board (view) methods - visualization
    // ------------------------------------

    void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // z-index 1: tiles
        for (const auto& row : tiles) {
            for (const auto& tile : row)
                target.draw(tile, states);
        }

        // z-index 2: grids
        target.draw(innerGrid, states);
        target.draw(outerGrid, states);
    }

}