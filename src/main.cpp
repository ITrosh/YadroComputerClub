#include "TaskSolver.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "The number of command line arguments is less or more than required!" << std::endl;
        return static_cast<int>(ErrorType::IncorrectQuantityOfArguments);
    }

    try {
        TaskSolver taskSolver;
        return taskSolver.run(argv[argc - 1]);
    } catch (const std::exception& exception) {
        std::cout << "Caught an unexpected exception: " << exception.what() << std::endl;
        return static_cast<int>(ErrorType::UnexpectedError);
    }
}
