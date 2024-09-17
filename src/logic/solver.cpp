#include "solver.h"
#include <algorithm>


namespace Sudoku {

    // --------------
    // Helper defines
    // --------------

    enum CommonLine : int { NO_LINE = -1, NO_COMMON_LINE = -2};


    // ---------------------------
    // Solver methods - main solve
    // ---------------------------

    // Public part
    bool Solver::solve(Board& board)
    {
        // Check if the board is already unsolvable
        if (!board.isCorrect())
            return false;
        
        // Process data in initial position
        initialialProcessing(board);

        // Start solving
        return solve(board, 0);
    }

    // Private part
    bool Solver::solve(Board& board, int depth)
    {
        // Stage 1 - cutting the positibilities

        // We iterate over inner squares as long as there is some potential forced fill that could limit the number of possible further fills
        int is = findBestSquare();
        while (!innerSquares[is].isFilled() && !innerSquares[is].isEvaluated()) {
            auto [r0, c0] = Board::innerSquareTopLeft(is);

            // It's important to create a copy of numbers to prevent any bugs connected with changing the structure during iteration
            std::set<int> nums = innerSquares[is].numsToEvaluate;
            innerSquares[is].numsToEvaluate.clear();

            for (int num : nums) {
                // Try to find common row or column which must certainly contain the given number in the correct completion
                int cr = NO_LINE, cc = NO_LINE;
                for (int i = 0; i < INNER_SQUARE_SIZE; i++) {
                    for (int j = 0; j < INNER_SQUARE_SIZE; j++) {
                        int r = r0 + i, c = c0 + j;
                        if (possibilities[r][c].contains(num)) {
                            cr = cr != r && cr != NO_LINE ? NO_COMMON_LINE : r;
                            cc = cc != c && cc != NO_LINE ? NO_COMMON_LINE : c;
                        }
                    }
                }

                bool result = true;
                
                // Case 1 - found both common rank and common file, which means there is exactly one field possible for given number
                if (cr != NO_LINE && cr != NO_COMMON_LINE && cc != NO_COMMON_LINE)
                    result = result && setNumber(board, cr, cc, num);
                // Case 2 - found only common rank, which means the number must be filled inside this rank of processed inner square
                else if (cr != NO_LINE && cr != NO_COMMON_LINE)
                    result = result && updatePossibilities<ROW>(board, cr, is, num);
                // Case 3 - similarly to case 2, only for file instead of rank
                else if (cc != NO_LINE && cc != NO_COMMON_LINE)
                    result = result && updatePossibilities<COL>(board, cc, is, num);

                if (!result)
                    return false;
            }

            is = findBestSquare();
        }

        // Stage 2 - guess-work when no forced moves are possible
        
        auto [r, c] = findBestField();
        
        // If the field is not empty, then the whole board is completed and we can return true
        if (!board.isEmpty(r, c))
            return true;
        
        // Copy all the necessary data structures to easily restore their state in case of failure in next branch
        auto boardSave = board;
        auto innerSquaresSave = innerSquares;
        auto possibilitiesSave = possibilities;

        for (int num : possibilitiesSave[r][c]) {
            bool result = setNumber(board, r, c, num) && solve(board, depth + 1);

            if (result)
                return true;
            else {
                // Restore data before trying another branch
                board = boardSave;
                innerSquares = innerSquaresSave;
                possibilities = possibilitiesSave;
            }
        }

        return false;
    }


    // -------------------------------------
    // Solver methods - solve initialization
    // -------------------------------------

    void Solver::initialialProcessing(const Board& board)
    {
        // Calculate inner square properties
        for (int is = 0; is < BOARD_SIZE; is++) {
            innerSquares[is].emptySquares = 0;
            innerSquares[is].numsToEvaluate = {1, 2, 3, 4, 5, 6, 7, 8, 9};

            auto [r0, c0] = Board::innerSquareTopLeft(is);
            for (int i = 0; i < INNER_SQUARE_SIZE; i++) {
                for (int j = 0; j < INNER_SQUARE_SIZE; j++) {
                    int r = r0 + i, c = c0 + j;
                    if (board.isEmpty(r, c))
                        innerSquares[is].emptySquares++;
                    else
                        innerSquares[is].numsToEvaluate.erase(board.getNumber(r, c));
                }
            }
        }

        // Calculate possibility map
        for (int r = 0; r < BOARD_SIZE; r++) {
            for (int c = 0; c < BOARD_SIZE; c++)
                possibilities[r][c] = board.availableNumbers(r, c);
        }
    }


    // ----------------------------------------
    // Solver methods - data structure handlers
    // ----------------------------------------

    int Solver::findBestSquare() const
    {
        auto resultIt = std::min_element(innerSquares.begin(), innerSquares.end(), [](const InnerSquareData& a, const InnerSquareData& b) {
            return a.isEvaluated() == b.isEvaluated() ? !a.isFilled() && a.emptySquares < b.emptySquares :
                                                        int(a.isEvaluated()) < int(b.isEvaluated());
        });

        return static_cast<int>(std::distance(innerSquares.begin(), resultIt));
    }

    std::pair<int, int> Solver::findBestField() const
    {
        int bestRow = 0, bestCol = 0, bestKey = 9;
        for (int r = 0; r < BOARD_SIZE; r++) {
            for (int c = 0; c < BOARD_SIZE; c++) {
                int key = static_cast<int>(possibilities[r][c].size());
                if (key < bestKey && key != 0) {
                    bestRow = r;
                    bestCol = c;
                    bestKey = key;
                }
            }
        }

        return { bestRow, bestCol };
    }

    template <LineType lineType>
    bool Solver::updatePossibilities(const Board& board, int line, int innerSquare, int num)
    {
        for (int i = 0; i < BOARD_SIZE; i++) {
            int r = lineType == ROW ? line : i;
            int c = lineType == COL ? line : i;
            int is = board.innerSquare(r, c);
            if (is != innerSquare && board.isEmpty(r, c) && possibilities[r][c].contains(num)) {
                possibilities[r][c].erase(num);
                innerSquares[is].numsToEvaluate.insert(num);

                // An empty suare without possible fills indicates incorrent completion of board
                if (possibilities[r][c].size() == 0)
                    return false;
                
                // Only one possible choice, which definitely makes the inner square "alive" (not evluated)
                if (possibilities[r][c].size() == 1)
                    innerSquares[is].numsToEvaluate.insert(*possibilities[r][c].begin());
            }
        }

        // If no field has become impossible to fill in
        return true;
    }

    bool Solver::setNumber(Board& board, int r, int c, int num)
    {
        int is = board.innerSquare(r, c);

        // Update other inner squares
        if (!updatePossibilities<ROW>(board, r, is, num) || !updatePossibilities<COL>(board, c, is, num))
            return false;
        
        // Fill the number in
        board.setNumber(r, c, num);

        // Update the possibilities inside current inner square
        auto [r0, c0] = Board::innerSquareTopLeft(is);
        for (int i = 0; i < INNER_SQUARE_SIZE; i++) {
            for (int j = 0; j < INNER_SQUARE_SIZE; j++)
                possibilities[r0 + i][c0 + j].erase(num);
        }

        // Update current inner square
        innerSquares[is].numsToEvaluate.insert(possibilities[r][c].begin(), possibilities[r][c].end());
        innerSquares[is].numsToEvaluate.erase(num);
        innerSquares[is].emptySquares--;
        possibilities[r][c].clear();

        return true;
    }

}