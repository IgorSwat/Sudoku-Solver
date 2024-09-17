#include "generators.h"
#include <algorithm>
#include <numeric>
#include <vector>


namespace Sudoku {

    // -------------------------
    // PosiitonGenerator methods
    // -------------------------

    PositionGenerator::PositionGenerator(Solver* solver)
        : solver(solver), randomGen(randomDev())
    {
    }

    void PositionGenerator::generate(Board& board)
    {
        board.clear();

        // Fill in all the inner squares on the diagonal with random (but correct) numbers
        for (int is = 0; is < BOARD_SIZE; is += INNER_SQUARE_SIZE + 1) {
            // Create a random permutation of all possible numbers
            std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            std::shuffle(nums.begin(), nums.end(), randomGen);

            // Fill in the fields with the obtained permutation from top to bottom
            auto [r0, c0] = Board::innerSquareTopLeft(is);
            for (int i = 0; i < INNER_SQUARE_SIZE; i++) {
                for (int j = 0; j < INNER_SQUARE_SIZE; j++)
                    board.setNumber(r0 + i, c0 + j, nums[i * INNER_SQUARE_SIZE + j]);
            }
        }

        // Complete the board
        // Since solve() method is deterministic, this gives 9! * 9! * 9! different board configurations
        solver->solve(board);

        // Remove some numbers
        std::vector<int> fields(BOARD_SIZE * BOARD_SIZE, 0);
        std::iota(fields.begin(), fields.end(), 0);
        std::shuffle(fields.begin(), fields.end(), randomGen);

        std::uniform_int_distribution<> distribution(11, 61);
        int r = distribution(randomGen);

        // Remove random number of elements from board
        for (int i = 0; i < r; i++)
            board.setNumber(fields[i] / BOARD_SIZE, fields[i] % BOARD_SIZE, 0);
    }

}