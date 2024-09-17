#include "controller.h"
#include <chrono>


namespace GUI {

    // --------------
    // GUI parameters
    // --------------

    // Board parameters
    const float BOARD_OFFSET = 3.f;
    const float TILE_SIZE = 60.f;
    const float BOARD_SIZE = TILE_SIZE * Sudoku::BOARD_SIZE;
    const unsigned BOARD_FONT_SIZE = 42;
    const sf::Color INNER_GRID_COLOR = sf::Color(120, 120, 120);
    const sf::Color OUTER_GRID_COLOR = sf::Color(70, 70, 70);
    const sf::Color TILE_HIGHLIGHT_COLOR_MAIN = sf::Color(10, 164, 255, 150);
    const sf::Color TILE_HIGHLIGHT_COLOR_SECOND = sf::Color(140, 220, 255, 100);

    // Info panel parameters
    const unsigned INFO_PANEL_TEXT_SIZE = 26;
    const float INFO_PANEL_ICON_SPACING = 14.f;
    const float INFO_PANEL_RANK_SPACING = 24.f;
    const sf::Vector2f INFO_PANEL_POS = {BOARD_SIZE + 30.f, 50.f};

    // Navbar parameters
    const float BT_SPACING = 20.f;
    const sf::Vector2f NAVBAR_POS = {BOARD_SIZE + 40.f, 330.f};

    // Button parameters
    const unsigned BT_LABEL_SIZE = 18;
    const unsigned BT_BIG_LABEL_SIZE = 22;
    const float BT_TEXT_SPACING = 15.f;
    const float BT_SIZE = 42.f;
    const float BT_BIG_SIZE = 56.f;
    const float BT_ROUND = 0.4f;
    const sf::Vector2f SOLVE_BUTTON_POS = {BOARD_SIZE + 105.f, 420.f};
    const sf::Color BT_COLOR_DEFAULT = sf::Color::White;
    const sf::Color BT_COLOR_ON_HOVER = sf::Color(201, 234, 245);
    const sf::Color BT_COLOR_ON_CLICK = sf::Color(140, 218, 245);

    // Window parameters
    const unsigned WINDOW_WIDTH = 800;
    const unsigned WINDOW_HEIGHT = 540 + static_cast<unsigned>(BOARD_OFFSET * 2);
    const sf::Color WINDOW_BACKGROUND_COLOR = sf::Color(230, 230, 230, 50);


    // ------------------
    // Controller methods
    // ------------------

    Controller::Controller()
        : generator(&solver),
          window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sudoku", sf::Style::Close | sf::Style::Titlebar,
                 sf::ContextSettings(0, 0, 4, 1, 1, 0, false)),
          boardView(TILE_SIZE, BOARD_FONT_SIZE, INNER_GRID_COLOR, OUTER_GRID_COLOR, TILE_HIGHLIGHT_COLOR_MAIN, TILE_HIGHLIGHT_COLOR_SECOND),
          infoPanel(INFO_PANEL_TEXT_SIZE, INFO_PANEL_ICON_SPACING, INFO_PANEL_RANK_SPACING),
          navbar(BT_SPACING)
    {
         // Setup window
        window.setFramerateLimit(60);
        if (icon.loadFromFile(RESOURCE_ROOT + "sudoku.png"))
            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

        // Setup board
        boardView.setPosition(sf::Vector2f(BOARD_OFFSET, BOARD_OFFSET));

        // Setup info panel
        infoPanel.setPosition(INFO_PANEL_POS);

        // Setup navbar
        navbar.setPosition(NAVBAR_POS);
        navbar.addButton(LabeledButton(ButtonType::BACK, "Back", BT_LABEL_SIZE, BT_TEXT_SPACING, BT_SIZE, BT_ROUND,
                                       BT_COLOR_DEFAULT, BT_COLOR_ON_HOVER, BT_COLOR_ON_CLICK));
        navbar.addButton(LabeledButton(ButtonType::CLEAR, "Clear", BT_LABEL_SIZE, BT_TEXT_SPACING, BT_SIZE, BT_ROUND,
                                       BT_COLOR_DEFAULT, BT_COLOR_ON_HOVER, BT_COLOR_ON_CLICK));
        navbar.addButton(LabeledButton(ButtonType::GENERATE, "Random", BT_LABEL_SIZE, BT_TEXT_SPACING, BT_SIZE, BT_ROUND,
                                       BT_COLOR_DEFAULT, BT_COLOR_ON_HOVER, BT_COLOR_ON_CLICK));
        navbar.addButton(LabeledButton(ButtonType::SOLVE, "Solve", BT_BIG_LABEL_SIZE, BT_TEXT_SPACING, BT_BIG_SIZE, BT_ROUND,
                                       BT_COLOR_DEFAULT, BT_COLOR_ON_HOVER, BT_COLOR_ON_CLICK), false, SOLVE_BUTTON_POS);
    }

    void Controller::run()
    {
        sf::Event event;

        while (window.isOpen()) {
            // Event processing
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
					window.close();
                
                // Update sudoku board view
                auto result = boardView.update(event);
                if (result.has_value()) {
                    int r = std::get<0>(result.value()), c = std::get<1>(result.value()), num = std::get<2>(result.value());
                    int oldNum = board.getNumber(r, c);

                    board.setNumber(r, c, num);

                    if (oldNum != num)
                        listOfChanges.push_back(std::make_tuple(r, c, oldNum, num));
                    infoPanel.setCorrectness(board.isCorrect());
                }

                // Update buttons
                ButtonType bType = navbar.update(event);
                switch (bType) {
                    case ButtonType::BACK:
                        if (!listOfChanges.empty()) {
                            auto lastChange = listOfChanges.back();

                            board.setNumber(std::get<0>(lastChange), std::get<1>(lastChange), std::get<2>(lastChange));
                            boardView.loadNumbers(board);
                            infoPanel.setCorrectness(board.isCorrect());

                            listOfChanges.erase(listOfChanges.end() - 1);
                        }
                        break;
                    case ButtonType::CLEAR:
                        board.clear();
                        listOfChanges.clear();
                        boardView.loadNumbers(board);
                        break;
                    case ButtonType::GENERATE:
                        generator.generate(board);
                        listOfChanges.clear();
                        boardView.loadNumbers(board);
                        infoPanel.setCorrectness(true);
                        break;
                    case ButtonType::SOLVE: {
                        auto start = std::chrono::steady_clock::now();
                        bool solveResult = solver.solve(board);
                        auto end = std::chrono::steady_clock::now();

                        listOfChanges.clear();
                        boardView.loadNumbers(board);
                        infoPanel.setCorrectness(solveResult);
                        infoPanel.setSolveTime(static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()));
                    }
                    default:
                        break;
                }
            }

            // Render
			window.clear(WINDOW_BACKGROUND_COLOR);
			window.draw(boardView);
            window.draw(infoPanel);
            window.draw(navbar);
			window.display();
        }
    }

}