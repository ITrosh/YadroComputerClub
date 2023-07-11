#ifndef COMPUTERCLUB_CONSTANTS_H
#define COMPUTERCLUB_CONSTANTS_H

#include <string_view>

inline constexpr char WORD_DELIMITER = ' ';
inline constexpr char TIME_DELIMITER = ':';

inline constexpr std::string_view ERROR1 = "NotOpenYet";
inline constexpr std::string_view ERROR2 = "YouShallNotPass";
inline constexpr std::string_view ERROR3 = "ClientUnknown";
inline constexpr std::string_view ERROR4 = "PlaceIsBusy";
inline constexpr std::string_view ERROR5 = "ICanWaitNoLonger!";
inline constexpr std::string_view ERROR6 = "ClientHasAlreadySatDownAtTheTable!";
inline constexpr std::string_view ERROR7 = "ClientIsAlreadyWaiting!";

inline constexpr uint16_t MINUTES_IN_HOUR = 60;

#endif //COMPUTERCLUB_CONSTANTS_H
