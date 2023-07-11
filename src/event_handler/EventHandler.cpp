#include "EventHandler.h"
#include "Constants.h"
#include <algorithm>

void EventHandler::HandleEventsOfTheDay(std::vector<std::string>& outputData, Error& error,
                                        std::vector<Table>& tables)
{
    if (!isCorrectInputFileData(error))
        return;

    tables.resize(_inputFileData.computerClubFeatures.numberOfTables);
    for (auto& table : tables)
        table = {{ 0, 0 }, { 0, 0 }, 0, false};

    outputData.push_back(_inputFileData.computerClubFeatures.openingTime.toString());

    size_t counter = 0;
    for (const auto& event: _inputFileData.computerClubEvents) {
        switch (event.eventId) {
            case IncomingEventID::ClientHasCome:
                outputData.push_back(_inputFileData.initialEvents[counter]);
                handleFirstEvent(event, outputData);
                break;
            case IncomingEventID::ClientHasSatDownAtTheTable:
                outputData.push_back(_inputFileData.initialEvents[counter]);
                handleSecondEvent(event, outputData, tables);
                break;
            case IncomingEventID::ClientIsWaiting:
                outputData.push_back(_inputFileData.initialEvents[counter]);
                handleThirdEvent(event, outputData, tables);
                break;
            default:
                outputData.push_back(_inputFileData.initialEvents[counter]);
                handleFourthEvent(event, outputData, tables);
                break;
        }

        counter++;
    }

    closeTheClub(outputData, tables);

    outputData.push_back(_inputFileData.computerClubFeatures.closingTime.toString());
}

bool EventHandler::isCorrectInputFileData(Error& error) {
    size_t eventNumber;
    if (!isCorrectSequenceOfEventTimes(eventNumber)) {
        error = { ErrorType::IncorrectEventTimeSequence, _inputFileData.initialEvents.at(eventNumber) };
        return false;
    }

    return true;
}

bool EventHandler::isCorrectSequenceOfEventTimes(size_t& eventNumber) {
    for (size_t i = 0; i < _inputFileData.computerClubEvents.size() - 1; i++) {
        if (!(_inputFileData.computerClubEvents.at(i + 1).eventTime >=
              _inputFileData.computerClubEvents.at(i).eventTime))
        {
            eventNumber = i + 1;
            return false;
        }
    }

    return true;
}

void EventHandler::handleFirstEvent(const Event& event, std::vector<std::string>& outputData) {
    if (_inputFileData.computerClubFeatures.openingTime.hours > event.eventTime.hours) {
        outputData.push_back(event.eventTime.toString().append(" ")
                             .append(std::to_string(static_cast<int>(OutgoingEventID::Error)))
                             .append(" ").append(ERROR1));
    } else {
        if (_clientStatuses.find(event.eventBody) != _clientStatuses.end()) {
            outputData.push_back(event.eventTime.toString().append(" ")
                                 .append(std::to_string(static_cast<int>(OutgoingEventID::Error)))
                                 .append(" ").append(ERROR2));
        }
        else {
            _clientStatuses[event.eventBody] = { IncomingEventID::ClientHasCome, 0 };
        }
    }
}

void EventHandler::handleSecondEvent(const Event& event, std::vector<std::string>& outputData,
                                     std::vector<Table>& tables)
{
    size_t pos = event.eventBody.find(WORD_DELIMITER);

    std::string clientName = event.eventBody.substr(0, pos);
    size_t tableNumber = std::stoi(event.eventBody.substr(pos + 1, event.eventBody.length() - pos - 1));

    auto it = _clientStatuses.find(clientName);

    if (!isClientKnown(event.eventTime, it, outputData)) {
        return;
    } else {
        auto& currentTable = tables.at(tableNumber - 1);

        if (currentTable.isBusy) {
            outputData.push_back(event.eventTime.toString().append(" ")
                                 .append(std::to_string(static_cast<int>(OutgoingEventID::Error)))
                                 .append(" ").append(ERROR4));
        } else {
            if (it->second.eventId == IncomingEventID::ClientHasCome)
                it->second.eventId = IncomingEventID::ClientHasSatDownAtTheTable;
            else
                freePreviousTable(event.eventTime, tables.at(it->second.numberOfBusyTable - 1));

            currentTable.isBusy = true;
            currentTable.startWorking = event.eventTime;

            it->second.numberOfBusyTable = tableNumber;
        }
    }
}

void EventHandler::handleThirdEvent(const Event& event, std::vector<std::string>& outputData,
                                    const std::vector<Table>& tables)
{
    auto it = _clientStatuses.find(event.eventBody);
    if (!isClientKnown(event.eventTime, it, outputData)) {
        return;
    } else {
        if (!isAllTablesBusy(tables)) {
            outputData.push_back(event.eventTime.toString().append(" ")
                                 .append(std::to_string(static_cast<int>(OutgoingEventID::Error)))
                                 .append(" ").append(ERROR5));
        } else {
            if (it->second.eventId == IncomingEventID::ClientHasCome) {
                if (_waitingClients.size() == _inputFileData.computerClubFeatures.numberOfTables) {
                    outputData.push_back(event.eventTime.toString().append(" ")
                                         .append(std::to_string(static_cast<int>(OutgoingEventID::ClientHasGoneAway)))
                                         .append(" ").append(event.eventBody));

                    _clientStatuses.erase(event.eventBody);
                } else {
                    _waitingClients.push_back(event.eventBody);
                    _clientStatuses[event.eventBody] = {IncomingEventID::ClientIsWaiting, 0};
                }
            } else if (it->second.eventId == IncomingEventID::ClientHasSatDownAtTheTable) {
                outputData.push_back(event.eventTime.toString().append(" ")
                                     .append(std::to_string(static_cast<int>(OutgoingEventID::Error)))
                                     .append(" ").append(ERROR6));
            } else {
                outputData.push_back(event.eventTime.toString().append(" ")
                                     .append(std::to_string(static_cast<int>(OutgoingEventID::Error)))
                                     .append(" ").append(ERROR7));
            }
        }
    }
}

void EventHandler::handleFourthEvent(const Event& event, std::vector<std::string>& outputData,
                                     std::vector<Table>& tables)
{
    auto it = _clientStatuses.find(event.eventBody);
    if (!isClientKnown(event.eventTime, it, outputData)) {
        return;
    } else {
        if (it->second.eventId == IncomingEventID::ClientHasSatDownAtTheTable) {
            auto& previousTable = tables.at(it->second.numberOfBusyTable - 1);
            calculateCurrentTableParameters(previousTable, event.eventTime);

            if (!_waitingClients.empty()) {
                std::string clientName = _waitingClients.front();
                _waitingClients.erase(_waitingClients.begin());

                _clientStatuses[clientName] = {IncomingEventID::ClientHasSatDownAtTheTable,
                                               it->second.numberOfBusyTable};

                previousTable.startWorking = event.eventTime;
                int temp = static_cast<int>(OutgoingEventID::ClientHasSatDownAtTheTable);

                outputData.push_back(event.eventTime.toString().append(" ")
                                     .append(std::to_string(temp)).append(" ").append(clientName).append(" ")
                                     .append(std::to_string(it->second.numberOfBusyTable)));
            } else {
                previousTable.isBusy = false;
                previousTable.startWorking = {0, 0};
            }
        } else if (it->second.eventId == IncomingEventID::ClientIsWaiting) {
            _waitingClients.erase(std::ranges::find(_waitingClients, event.eventBody));
        }

        _clientStatuses.erase(event.eventBody);
    }
}

bool EventHandler::isClientKnown(const Time& eventTime,
                                 const std::map<std::string, ClientStatus>::iterator& clientNameIterator,
                                 std::vector<std::string>& outputData)
{
    if (clientNameIterator == _clientStatuses.end()) {
        outputData.push_back(eventTime.toString().append(" ")
                             .append(std::to_string(static_cast<int>(OutgoingEventID::Error)))
                             .append(" ").append(ERROR3));
        return false;
    } else {
        return true;
    }
}

void EventHandler::freePreviousTable(const Time& eventTime, Table& previousTable) {
    calculateCurrentTableParameters(previousTable, eventTime);

    previousTable.isBusy = false;
    previousTable.startWorking = {0, 0};
}

void EventHandler::calculateCurrentTableParameters(Table& table, const Time& stopWorking) const {
    Time temp;

    if (stopWorking.hours == table.startWorking.hours) {
        temp.hours = 0;
        temp.minutes = stopWorking.minutes - table.startWorking.minutes;
    } else {
        if (stopWorking.minutes >= table.startWorking.minutes) {
            temp.hours = stopWorking.hours - table.startWorking.hours;
            temp.minutes = stopWorking.minutes - table.startWorking.minutes;
        } else {
            temp.hours = stopWorking.hours - 1 - table.startWorking.hours;
            temp.minutes = stopWorking.minutes + MINUTES_IN_HOUR - table.startWorking.minutes;
        }
    }

    if (temp.minutes + table.entireWorkingTime.minutes >= MINUTES_IN_HOUR) {
        table.entireWorkingTime.hours += temp.hours + 1;
        table.entireWorkingTime.minutes += temp.minutes - MINUTES_IN_HOUR;
    } else {
        table.entireWorkingTime.hours += temp.hours;
        table.entireWorkingTime.minutes += temp.minutes;
    }

    if (temp.minutes == 0)
        table.income += temp.hours * _inputFileData.computerClubFeatures.costPerHour;
    else
        table.income += (temp.hours + 1) * _inputFileData.computerClubFeatures.costPerHour;
}

bool EventHandler::isAllTablesBusy(const std::vector<Table>& tables) {
    return std::ranges::all_of(tables, [](const Table& table) {
        return table.isBusy;
    });
}

void EventHandler::closeTheClub(std::vector<std::string>& outputData, std::vector<Table>& tables) {
    for (const auto& client: _clientStatuses) {
        outputData.push_back(_inputFileData.computerClubFeatures.closingTime.toString().append(" ")
                             .append(std::to_string(static_cast<int>(OutgoingEventID::ClientHasGoneAway))).append(" ")
                             .append(client.first));

        if (client.second.eventId == IncomingEventID::ClientHasSatDownAtTheTable) {
            auto& table = tables.at(client.second.numberOfBusyTable - 1);
            calculateCurrentTableParameters(table, _inputFileData.computerClubFeatures.closingTime);

            table.isBusy = false;
            table.startWorking = { 0, 0 };
        }
    }
}
