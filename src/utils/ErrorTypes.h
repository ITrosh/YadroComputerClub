#ifndef COMPUTERCLUB_ERRORTYPES_H
#define COMPUTERCLUB_ERRORTYPES_H

enum class ErrorType {
    Success,
    IncorrectQuantityOfArguments,
    IncorrectFileExtension,
    FileIsNotOpen,
    IncorrectNumberOfTables,
    IncorrectWorkingHoursFormat,
    IncorrectOpeningTime,
    IncorrectClosingTime,
    OpeningTimeIsNotLessThanClosingTime,
    IncorrectCostPerHour,
    NoDelimiters,
    TooLittleDelimiters,
    IncorrectEventTime,
    EventTimeIsNotLessThanClosingTime,
    IncorrectEventID,
    IncorrectBodyEventWithSecondID,
    IncorrectClientName,
    IncorrectTableNumber,
    TableNumberMoreThanNumberOfTables,
    IncorrectEventTimeSequence,
    UnexpectedError = -1,
};

#endif //COMPUTERCLUB_ERRORTYPES_H
