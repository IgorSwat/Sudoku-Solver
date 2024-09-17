#pragma once

#include <array>
#include <iostream>
#include <set>
#include <string>
#include <utility>


namespace Sudoku {

    // --------------
    // Helper defines
    // --------------

    constexpr int BOARD_SIZE = 9;
    constexpr int INNER_SQUARE_SIZE = 3;

    enum LineType : int { ROW, COL };


    // -----------
    // Board class
    // -----------

    // A simple sudoku board representation
    class Board
    {
    public:
        Board() = default;
        Board(const Board& other) = default;
        Board& operator=(const Board& other) = default;

        // Global state handlers
        void clear();
        void load(const std::string& setup);

        // Local state handlers
        void setNumber(int row, int col, int number) { board[row][col] = number; }
        int getNumber(int row, int col) const { return board[row][col]; }           // Returns a single number
        bool isEmpty(int row, int col) const { return board[row][col] == 0; }

        // Correctness checks
        bool isCorrect() const;                     // Entire board check
        bool isCorrect(int row, int col) const;     // Just a one number check (inside it's column, row, and box)
        std::set<int> availableNumbers(int row, int col) const;

        // Inner square calculations
        static int innerSquare(int row, int col) { return row - row % INNER_SQUARE_SIZE + col / INNER_SQUARE_SIZE; }
        static std::pair<int, int> innerSquareTopLeft(int isq) { return { isq - isq % INNER_SQUARE_SIZE,
                                                                         (isq % INNER_SQUARE_SIZE) * INNER_SQUARE_SIZE }; }

        // Helper functions
        friend std::ostream& operator<<(std::ostream& os, const Board& board);

    private:
        std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> board = { };
    };

}