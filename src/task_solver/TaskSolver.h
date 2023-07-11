#ifndef COMPUTERCLUB_TASKSOLVER_H
#define COMPUTERCLUB_TASKSOLVER_H

#include "InputFileData.h"

class TaskSolver {
public:
    int run(const std::string& fileName);

    const Time& getEntireWorkingTimeOfTable(size_t tableNumber) const;
    size_t getTableIncome(size_t tableNumber) const;
    const std::string& getOutputResult() const { return _result; }

private:
    InputFileData _inputFileData;
    Error _error;
    std::vector<Table> _tables;
    std::vector<std::string> _outputData;
    std::string _result;

    void writeResultInfo();
    void collectResultInfo();
};


#endif //COMPUTERCLUB_TASKSOLVER_H
