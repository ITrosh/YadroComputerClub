#ifndef COMPUTERCLUB_INPUTFILEDATA_H
#define COMPUTERCLUB_INPUTFILEDATA_H

#include "ErrorTypes.h"
#include <cstdint>
#include <string>
#include <vector>

struct Time {
    uint16_t hours{};
    uint16_t minutes{};

    bool operator>= (const Time& time) const {
        return (hours > time.hours || (hours == time.hours && minutes >= time.minutes));
    }

    std::string toString() const {
        std::string result;
        if (hours < 10) {
            result.append("0").append(std::to_string(hours));
            if (minutes < 10)
                return result.append(":").append("0").append(std::to_string(minutes));
            else
                return result.append(":").append(std::to_string(minutes));
        } else {
            result.append(std::to_string(hours));
            if (minutes < 10)
                return result.append(":").append("0").append(std::to_string(minutes));
            else
                return result.append(":").append(std::to_string(minutes));
        }
    }
};

struct ComputerClubFeatures {
    size_t numberOfTables{};
    Time closingTime;
    Time openingTime;
    size_t costPerHour{};
};

enum class IncomingEventID {
    NoEvent = 0,
    ClientHasCome,
    ClientHasSatDownAtTheTable,
    ClientIsWaiting,
    ClientHasGoneAway,
};

struct Event {
    Time eventTime;
    IncomingEventID eventId;
    std::string eventBody;
};

struct Error {
    ErrorType errorType{ErrorType::Success};
    std::string errorStr;
};

struct InputFileData {
    ComputerClubFeatures computerClubFeatures;
    std::vector<Event> computerClubEvents;
    std::vector<std::string> initialEvents;
};

struct ClientStatus {
    IncomingEventID eventId{IncomingEventID::NoEvent};
    size_t numberOfBusyTable{};
};

struct Table {
    Time startWorking, entireWorkingTime;
    size_t income{};
    bool isBusy{false};
};

enum class OutgoingEventID {
    ClientHasGoneAway = 11,
    ClientHasSatDownAtTheTable,
    Error,
};

#endif //COMPUTERCLUB_INPUTFILEDATA_H
