#include "src/logic/board.h"
#include "src/logic/solver.h"
#include "src/gui/controller.h"
#include <chrono>
#include <memory>

using namespace Sudoku;


int main()
{
    GUI::load_resources();

    std::unique_ptr<GUI::Controller> controller = std::make_unique<GUI::Controller>();
    controller->run();

    return 0;
}