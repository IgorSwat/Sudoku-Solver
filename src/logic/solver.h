#pragma once

#include "board.h"


namespace Sudoku {

    // ------------
    // Solver class
    // ------------

    class Solver
    {
    public:
        Solver() = default;

        // Main solving method
        bool solve(Board& board);   // Returns true if the board was succesfully solved or false in other case

        // -------------
        // Local defines
        
        struct InnerSquareData
        {
            int emptySquares;
            std::set<int> numsToEvaluate;

            InnerSquareData() = default;
            InnerSquareData(const InnerSquareData& other) = default;
            InnerSquareData& operator=(const InnerSquareData& other) = default;

            bool isFilled() const { return emptySquares == 0; }
            bool isEvaluated() const { return numsToEvaluate.size() == 0; }
        };

    private:
        // Helper functions - solve components
        void initialialProcessing(const Board& board);
        bool solve(Board& board, int depth);
        // Helper functions - data structure handlers
        int findBestSquare() const;     // Returns an index of inner square with least number of empty squares and numbers awaiting for eval
        std::pair<int, int> findBestField() const;  // Returns a rank and file of empty field with least number of possibilities
        template <LineType lineType>
        bool updatePossibilities(const Board& board, int line, int innerSquare, int num);   // Dynamic update of innerSquares and possibilities
        bool setNumber(Board& board, int r, int c, int num);    // Same as above, only it affects both row and column and fills the number in
        
        // Data structures
        std::array<InnerSquareData, BOARD_SIZE> innerSquares;                           // State of inner n x n squares
        std::array<std::array<std::set<int>, BOARD_SIZE>, BOARD_SIZE> possibilities;    // Map of options of how could given field be filled
    };

}