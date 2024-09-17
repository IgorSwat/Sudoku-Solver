#include "board.h"
#include <algorithm>
#include <cctype>
#include <iterator>
#include <sstream>


namespace Sudoku {

    // -------------------------------------
    // Board methods - global state handlers
    // -------------------------------------

    void Board::clear()
    {
        this->board = {};
    }

    void Board::load(const std::string& setup)
    {
        // Warning - no support for boards bigger than 9x9

        clear();

        int r = 0, c = 0;
        for (char sym : setup) {
            if (sym == '/') {
                r++;
                c = 0;
            }
            else if (sym == 'n')    // 'n' stands for null or none (empty field)
                c++;
            else if (std::isdigit(sym)) {
                setNumber(r, c, static_cast<int>(sym - '0'));
                c++;
            }
        }
    }


    // ----------------------------------
    // Board methods - correctness issues
    // ----------------------------------

    bool Board::isCorrect() const
    {
        std::pair<int, std::set<int>> rows[BOARD_SIZE] = {}, cols[BOARD_SIZE] = {}, boxes[BOARD_SIZE] = {};

        // Loop over the entire board
        for (int r = 0; r < BOARD_SIZE; r++) {
            for (int c = 0; c < BOARD_SIZE; c++)  {
                if (!isEmpty(r, c)) {
                    rows[r].first++;
                    rows[r].second.insert(board[r][c]);
                    cols[c].first++;
                    cols[c].second.insert(board[r][c]);
                    boxes[innerSquare(r, c)].first++;
                    boxes[innerSquare(r, c)].second.insert(board[r][c]);
                }
            }
        }
            
        // std::set does not allow duplicated values, so if every value is unique, they should have size() equal to BOARD_SIZE
        for (int i = 0 ; i < BOARD_SIZE; i++) {
            if (rows[i].second.size() < rows[i].first || 
                cols[i].second.size() < cols[i].first || 
                boxes[i].second.size() < boxes[i].first)
                return false;
        }

        return true;
    }

    bool Board::isCorrect(int row, int col) const
    {
        // Since an empty square cannot violate sudoku rules, we can end immediately in this case
        if (isEmpty(row, col))
            return true;
        
        std::set<int> options = availableNumbers(row, col);
        return options.find(board[row][col]) != options.end();
    }

    std::set<int> Board::availableNumbers(int row, int col) const
    {
        if (!isEmpty(row, col))
            return {};

        // Warning: needs to be corrected to generalize for all sudoku board sizes
        std::set<int> allOptions = {1, 2, 3, 4, 5, 6, 7, 8, 9}, illegalOptions = {};

        // Rows and columns
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (i != col)
                illegalOptions.insert(board[row][i]);
            if (i != row)
                illegalOptions.insert(board[i][col]);
        }

        // Boxes
        auto [x0, y0] = innerSquareTopLeft(innerSquare(row, col));
        for (int i = 0; i < INNER_SQUARE_SIZE; i++) {
            for (int j = 0; j < INNER_SQUARE_SIZE; j++) {
                int r = x0 + i, c = y0 + j;
                if (r != row || c != col)
                    illegalOptions.insert(board[r][c]);
            }
        }

        // Calculate and return the difference
        std::set<int> diff;
        std::set_difference(allOptions.begin(), allOptions.end(), illegalOptions.begin(), illegalOptions.end(), 
                            std::inserter(diff, diff.begin()));
        
        return diff;
    }


    // -------------
    // Miscellaneous
    // -------------

    std::ostream& operator<<(std::ostream& os, const Board& board)
    {
        for (int r = 0; r < BOARD_SIZE; r++) {
            for (int k = 0; k < BOARD_SIZE; k++) {
                os << board.board[r][k] << " ";
                if (k % INNER_SQUARE_SIZE == INNER_SQUARE_SIZE - 1 && k != BOARD_SIZE - 1)
                    os << "| ";
            }
            os << "\n";
            if (r % INNER_SQUARE_SIZE == INNER_SQUARE_SIZE - 1 && r != BOARD_SIZE - 1)
                os << std::string((BOARD_SIZE + INNER_SQUARE_SIZE - 1) * 2, '-') << "\n";
        }

        return os;
    }

}