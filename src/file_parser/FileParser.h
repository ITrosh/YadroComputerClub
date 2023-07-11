#ifndef COMPUTERCLUB_FILEPARSER_H
#define COMPUTERCLUB_FILEPARSER_H

#include "InputFileData.h"

class FileParser {
public:
    static void Parse(const std::string& fileName, InputFileData& inputFileData, Error& error);

private:
    static bool isPositiveNumber(const std::string& data);

    static bool isCorrectWorkingHoursFormat(const std::string& data, ErrorType& errorType,
                                            Time& opening, Time& closing);
    static bool isCorrectTimeFormat(const std::string& data, Time& time);
    static bool isCorrectHourFormat(const std::string& data);
    static bool isCorrectMinuteFormat(const std::string& data);

    static void parseEvents(std::ifstream&& txtFile, InputFileData& inputFileData, Error& error);

    static bool isCorrectEventId(const std::string& data, ErrorType& errorType, IncomingEventID& eventId);
    static bool isCorrectEventBody(const std::string& data, IncomingEventID eventId,
                                   size_t numberOfTables, ErrorType& errorType);
    static bool isCorrectClientName(const std::string& data);
};


#endif //COMPUTERCLUB_FILEPARSER_H
