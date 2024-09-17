#pragma once

#include "solver.h"
#include <random>


namespace Sudoku {

    // -----------------------
    // PosiitonGenerator class
    // -----------------------

    // Generates a random, solvable sudoku position
    class PositionGenerator
    {
    public:
        PositionGenerator(Solver* solver);

        // Main generation method
        void generate(Board& board);
    
    private:
        Solver* solver;

        // Random number generation
        std::random_device randomDev;
        std::mt19937 randomGen;
    };

}