#ifndef COMPUTERCLUB_EVENTHANDLER_H
#define COMPUTERCLUB_EVENTHANDLER_H

#include "InputFileData.h"
#include <map>
#include <list>

class EventHandler {
public:
    explicit EventHandler(InputFileData inputFileData)
                        : _inputFileData(std::move(inputFileData)) { }

    void HandleEventsOfTheDay(std::vector<std::string>& outputData, Error& error, std::vector<Table>& tables);

private:
    InputFileData _inputFileData;
    std::map<std::string, ClientStatus> _clientStatuses;
    std::list<std::string> _waitingClients;

    bool isCorrectInputFileData(Error& error);
    bool isCorrectSequenceOfEventTimes(size_t& eventNumber);

    void handleFirstEvent(const Event& event, std::vector<std::string>& outputData);
    void handleSecondEvent(const Event& event, std::vector<std::string>& outputData, std::vector<Table>& tables);
    void handleThirdEvent(const Event& event, std::vector<std::string>& outputData, const std::vector<Table>& tables);
    void handleFourthEvent(const Event& event, std::vector<std::string>& outputData, std::vector<Table>& tables);

    bool isClientKnown(const Time& eventTime, const std::map<std::string, ClientStatus>::iterator& clientNameIterator,
                       std::vector<std::string>& outputData);

    void freePreviousTable(const Time& eventTime, Table& previousTable);

    void calculateCurrentTableParameters(Table& table, const Time& stopWorking) const;

    static bool isAllTablesBusy(const std::vector<Table>& tables);

    void closeTheClub(std::vector<std::string>& outputFileData, std::vector<Table>& tables);
};


#endif //COMPUTERCLUB_EVENTHANDLER_H
