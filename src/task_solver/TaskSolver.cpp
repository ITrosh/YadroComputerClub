#include "TaskSolver.h"
#include "FileParser.h"
#include "EventHandler.h"
#include <iostream>
#include <sstream>

int TaskSolver::run(const std::string& fileName) {
    FileParser::Parse(fileName, _inputFileData, _error);

    if (_error.errorType != ErrorType::Success) {
        switch (_error.errorType) {
            case ErrorType::IncorrectFileExtension:
                std::cout << fileName << std::endl;
                std::cout << "The input file has an incorrect extension!" << std::endl;
                return static_cast<int>(ErrorType::IncorrectFileExtension);

            case ErrorType::FileIsNotOpen:
                std::cout << fileName << std::endl;
                std::cout << "The file is not open!" << std::endl;
                return static_cast<int>(ErrorType::FileIsNotOpen);

            case ErrorType::IncorrectNumberOfTables:
                std::cout << _error.errorStr << std::endl;
                std::cout << "The number of tables is incorrect!" << std::endl;
                return static_cast<int>(ErrorType::IncorrectNumberOfTables);

            case ErrorType::IncorrectWorkingHoursFormat:
                std::cout << _error.errorStr << std::endl;
                std::cout << "There is an error in delimiter symbol in working hours format!" << std::endl;
                return static_cast<int>(ErrorType::IncorrectWorkingHoursFormat);

            case ErrorType::IncorrectOpeningTime:
                std::cout << _error.errorStr << std::endl;
                std::cout << "The opening time format is incorrect!" << std::endl;
                return static_cast<int>(ErrorType::IncorrectOpeningTime);

            case ErrorType::IncorrectClosingTime:
                std::cout << _error.errorStr << std::endl;
                std::cout << "The closing time format is incorrect!" << std::endl;
                return static_cast<int>(ErrorType::IncorrectClosingTime);

            case ErrorType::OpeningTimeIsNotLessThanClosingTime:
                std::cout << _error.errorStr << std::endl;
                std::cout << "The opening time is not less than the closing time!" << std::endl;
                return static_cast<int>(ErrorType::OpeningTimeIsNotLessThanClosingTime);

            case ErrorType::IncorrectCostPerHour:
                std::cout << _error.errorStr << std::endl;
                std::cout << "The cost per hour in computer club is incorrect!" << std::endl;
                return static_cast<int>(ErrorType::IncorrectCostPerHour);

            case ErrorType::NoDelimiters:
                std::cout << _error.errorStr << std::endl;
                std::cout << "There is no delimiters in event description!" << std::endl;
                return static_cast<int>(ErrorType::NoDelimiters);

            case ErrorType::TooLittleDelimiters:
                std::cout << _error.errorStr << std::endl;
                std::cout << "There is only one delimiter in event description!" << std::endl;
                return static_cast<int>(ErrorType::TooLittleDelimiters);

            case ErrorType::IncorrectEventTime:
                std::cout << _error.errorStr << std::endl;
                std::cout << "The event time format is incorrect!" << std::endl;
                return static_cast<int>(ErrorType::IncorrectEventTime);

            case ErrorType::EventTimeIsNotLessThanClosingTime:
                std::cout << _error.errorStr << std::endl;
                std::cout << "The event time is not less than closing time!" << std::endl;
                return static_cast<int>(ErrorType::EventTimeIsNotLessThanClosingTime);

            case ErrorType::IncorrectEventID:
                std::cout << _error.errorStr << std::endl;
                std::cout << "The event ID format is incorrect!" << std::endl;
                return static_cast<int>(ErrorType::IncorrectEventID);

            case ErrorType::IncorrectBodyEventWithSecondID:
                std::cout << _error.errorStr << std::endl;
                std::cout << "There is no delimiter for event body with second ID!" << std::endl;
                return static_cast<int>(ErrorType::IncorrectBodyEventWithSecondID);

            case ErrorType::IncorrectClientName:
                std::cout << _error.errorStr << std::endl;
                std::cout << "The client name included in event body is incorrect!" << std::endl;
                return static_cast<int>(ErrorType::IncorrectClientName);

            case ErrorType::IncorrectTableNumber:
                std::cout << _error.errorStr << std::endl;
                std::cout << "The table number included in event body is incorrect!" << std::endl;
                return static_cast<int>(ErrorType::IncorrectTableNumber);

            default:
                std::cout << _error.errorStr << std::endl;
                std::cout << "The table number more than number of tables!" << std::endl;
                return static_cast<int>(ErrorType::TableNumberMoreThanNumberOfTables);
        }
    }

    EventHandler eventHandler(std::move(_inputFileData));
    eventHandler.HandleEventsOfTheDay(_outputData, _error, _tables);

    if (_error.errorType == ErrorType::IncorrectEventTimeSequence) {
        std::cout << _error.errorStr << std::endl;
        std::cout << "The time sequence of events is incorrect!" << std::endl;
        return static_cast<int>(ErrorType::IncorrectEventTimeSequence);
    }

    writeResultInfo();
    collectResultInfo();

    std::cout << _result << std::endl;

    return static_cast<int>(_error.errorType);
}

void TaskSolver::writeResultInfo() {
    size_t tableNumber = 1;
    for (const auto& table: _tables) {
        _outputData.push_back(std::to_string(tableNumber++).append(" ").append(std::to_string(table.income))
                              .append(" ").append(table.entireWorkingTime.toString()));
    }
}

void TaskSolver::collectResultInfo() {
    std::ostringstream oss;

    for (const auto& outputLine : _outputData) {
        oss << outputLine << '\n';
    }

    _result = oss.str();
}

// Artificial method for testing
const Time& TaskSolver::getEntireWorkingTimeOfTable(size_t tableNumber) const {
    return _tables.at(tableNumber - 1).entireWorkingTime;
}

// Artificial method for testing
size_t TaskSolver::getTableIncome(size_t tableNumber) const {
    return _tables.at(tableNumber -1).income;
}