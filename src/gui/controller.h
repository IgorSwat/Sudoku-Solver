#pragma once

#include "boardView.h"
#include "infoPanel.h"
#include "navbar.h"
#include "../logic/generators.h"


namespace GUI {

    // ----------------
    // Controller class
    // ----------------

    class Controller
    {
    public:
        Controller();

        // GUI rendering and update
		void run();

    private:
        // Backend
        Sudoku::Board board;
        Sudoku::Solver solver;
        Sudoku::PositionGenerator generator;
        std::vector<std::tuple<int, int, int, int>> listOfChanges;

        // Graphic content
        sf::Image icon;
        sf::RenderWindow window;
        Board boardView;
        InfoPanel infoPanel;
        Navbar navbar;
    };

}