#include "FileParser.h"
#include "Constants.h"
#include <fstream>
#include <algorithm>

void FileParser::Parse(const std::string& fileName, InputFileData& inputFileData, Error& error) {
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos == std::string::npos || fileName.substr(dotPos + 1, fileName.length() - dotPos - 1) != "txt") {
        error = { ErrorType::IncorrectFileExtension, "The file has an incorrect extension!" };
        return;
    }

    std::ifstream txtFile(fileName);

    if (txtFile.is_open()) {
        std::string numberOfTables;
        std::getline(txtFile, numberOfTables);

        if (!isPositiveNumber(numberOfTables)) {
            error = { ErrorType::IncorrectNumberOfTables, numberOfTables };
            return;
        }

        inputFileData.computerClubFeatures.numberOfTables = stoi(numberOfTables);

        std::string workingHours;
        ErrorType errorType;
        Time opening{}, closing{};

        std::getline(txtFile, workingHours);
        if (!isCorrectWorkingHoursFormat(workingHours, errorType, opening, closing)) {
            error = { errorType, workingHours};
            return;
        }

        if ((opening.hours > closing.hours) ||
            (opening.hours == closing.hours && opening.minutes >= closing.minutes))
        {
            error = { ErrorType::OpeningTimeIsNotLessThanClosingTime, workingHours};
            return;
        }

        inputFileData.computerClubFeatures.openingTime = opening;
        inputFileData.computerClubFeatures.closingTime = closing;

        std::string costPerHour;
        std::getline(txtFile, costPerHour);

        if (!isPositiveNumber(costPerHour)) {
            error = { ErrorType::IncorrectCostPerHour, costPerHour};
            return;
        }

        inputFileData.computerClubFeatures.costPerHour = stoi(costPerHour);

        parseEvents(std::move(txtFile), inputFileData, error);
    } else {
        error.errorType = ErrorType::FileIsNotOpen;
    }
}

bool FileParser::isPositiveNumber(const std::string& data) {
    if (!data.empty() && data.at(0) >= '1' && data.at(0) <= '9') {
        return std::ranges::all_of(data.cbegin() + 1, data.cend(), [](char symbol) {
            return symbol >= '0' && symbol <= '9';
        });
    }

    return false;
}

bool FileParser::isCorrectWorkingHoursFormat(const std::string& data, ErrorType& errorType,
                                             Time& opening, Time& closing)
{
    size_t pos = data.find(WORD_DELIMITER);
    if (pos == std::string::npos) {
        errorType = ErrorType::IncorrectWorkingHoursFormat;
        return false;
    }

    std::string openingTime = data.substr(0, pos);
    if (!isCorrectTimeFormat(openingTime, opening)) {
        errorType = ErrorType::IncorrectOpeningTime;
        return false;
    }

    std::string closingTime = data.substr(pos + 1, data.length() - pos - 1);
    if (!isCorrectTimeFormat(closingTime, closing)) {
        errorType = ErrorType::IncorrectClosingTime;
        return false;
    }

    return true;
}

bool FileParser::isCorrectTimeFormat(const std::string& data, Time& time) {
    size_t pos = data.find(TIME_DELIMITER);
    if (pos == std::string::npos)
        return false;

    std::string hours = data.substr(0, pos);
    if (hours.length() != 2 || !isCorrectHourFormat(hours))
        return false;

    std::string minutes = data.substr(pos + 1, data.length() - pos - 1);
    if (minutes.length() != 2 || !isCorrectMinuteFormat(minutes))
        return false;

    time.hours = stoi(hours);
    time.minutes = stoi(minutes);

    return true;
}

bool FileParser::isCorrectHourFormat(const std::string& data) {
    if (data.at(0) == '0' || data.at(0) == '1')
        return (data.at(1) >= '0' && data.at(1) <= '9');
    else if (data.at(0) == '2')
        return (data.at(1) >= '0' && data.at(1) <= '3');
    else
        return false;
}

bool FileParser::isCorrectMinuteFormat(const std::string& data) {
    return (data.at(0) >= '0' && data.at(0) <= '5') && (data.at(1) >= '0' && data.at(1) <= '9');
}

void FileParser::parseEvents(std::ifstream&& txtFile, InputFileData& inputFileData, Error& error) {
    std::string line;

    while (getline(txtFile, line)) {
        size_t firstPos = line.find(WORD_DELIMITER);

        if (firstPos == std::string::npos) {
            error = { ErrorType::NoDelimiters, line};
            return;
        }

        size_t secondPos = line.find(WORD_DELIMITER, firstPos + 1);
        if (secondPos == std::string::npos) {
            error = { ErrorType::TooLittleDelimiters, line};
            return;
        }

        std::string time = line.substr(0, firstPos);
        std::string id = line.substr(firstPos + 1, secondPos - firstPos - 1);
        std::string body = line.substr(secondPos + 1, line.length() - secondPos - 1);

        Time eventTime{};
        if (!isCorrectTimeFormat(time, eventTime)) {
            error = { ErrorType::IncorrectEventTime, line };
            return;
        }

        if (eventTime >= inputFileData.computerClubFeatures.closingTime) {
            error = { ErrorType::EventTimeIsNotLessThanClosingTime, line };
            return;
        }

        IncomingEventID eventId;
        ErrorType errorType;
        if (!isCorrectEventId(id, errorType, eventId)) {
            error = { errorType, line };
            return;
        }

        if (!isCorrectEventBody(body, eventId, inputFileData.computerClubFeatures.numberOfTables, errorType)) {
            error = { errorType, line };
            return;
        }

        inputFileData.computerClubEvents.push_back({eventTime, eventId, body});
        inputFileData.initialEvents.push_back(line);
        line.clear();
    }
}

bool FileParser::isCorrectEventId(const std::string& data, ErrorType& errorType, IncomingEventID& eventId) {
    if (data.length() == 1 && data.at(0) >= '1' && data.at(0) <= '4') {
        switch (data.at(0)) {
            case '1':
                eventId = IncomingEventID::ClientHasCome;
                break;
            case '2':
                eventId = IncomingEventID::ClientHasSatDownAtTheTable;
                break;
            case '3':
                eventId = IncomingEventID::ClientIsWaiting;
                break;
            default:
                eventId = IncomingEventID::ClientHasGoneAway;
                break;
        }
        return true;
    } else {
        errorType = ErrorType::IncorrectEventID;
        return false;
    }
}

bool FileParser::isCorrectEventBody(const std::string& data, IncomingEventID eventId,
                                    size_t numberOfTables, ErrorType& errorType)
{
    if (eventId == IncomingEventID::ClientHasSatDownAtTheTable) {
        size_t delimiterPos = data.find(WORD_DELIMITER);

        if (delimiterPos == std::string::npos) {
            errorType = ErrorType::IncorrectBodyEventWithSecondID;
            return false;
        } else {
            std::string clientName = data.substr(0, delimiterPos);
            std::string tableNumber = data.substr(delimiterPos + 1, data.length() - delimiterPos - 1);

            if (!isCorrectClientName(clientName)) {
                errorType = ErrorType::IncorrectClientName;
                return false;
            }

            if (!isPositiveNumber(tableNumber)) {
                errorType = ErrorType::IncorrectTableNumber;
                return false;
            }

            if (stoi(tableNumber) > numberOfTables) {
                errorType = ErrorType::TableNumberMoreThanNumberOfTables;
                return false;
            }
        }
    } else {
        if (!isCorrectClientName(data)) {
            errorType = ErrorType::IncorrectClientName;
            return false;
        }
    }

    return true;
}

bool FileParser::isCorrectClientName(const std::string& data) {
    if (!data.empty()) {
        return std::ranges::all_of(data, [](char symbol) {
            return symbol >= '0' && symbol <= '9' || symbol >= 'a' && symbol <= 'z' || symbol == '_' || symbol == '-';
        });
    }

    return false;
}
