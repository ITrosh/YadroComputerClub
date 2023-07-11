#include "FileParser.h"
#include "TaskSolver.h"
#include <catch2/catch_all.hpp>
#include <source_location>
#include <filesystem>

namespace fs = std::filesystem;

constexpr const char* CURRENT_SOURCE_FILE_PATH = std::source_location::current().file_name();

constexpr const char* RESOURCE_FOLDER = "resource";
constexpr const char* TEST_OPEN_FILE_FOLDER = "test_open_file";
constexpr const char* TEST_PARSE_FILE_FOLDER = "test_parse_file";
constexpr const char* TEST_SEMANTIC_ERRORS_FOLDER = "test_semantic_errors_in_input_file_data";
constexpr const char* TEST_SUCCESS_OUTPUT_RESULT_FOLDER = "test_success_output_result";
constexpr const char* TEST_PRINT_OUTPUT_RESULT_FOLDER = "test_print_output_result";

void CheckFileParsing(fs::path& testFolderPath, const std::string& filePath, ErrorType errorType) {
    InputFileData inputFileData;
    Error error = { ErrorType::Success, "" };
    FileParser::Parse(testFolderPath.append(filePath).string(), inputFileData, error);
    REQUIRE(error.errorType == errorType);
}

TEST_CASE("OpeningTxtFile", "[YadroComputerClubTest]") {
    auto testOpenFileResourcePath = fs::path(CURRENT_SOURCE_FILE_PATH).parent_path().append(RESOURCE_FOLDER)
                                    .append(TEST_OPEN_FILE_FOLDER);

    SECTION("IncorrectFileExtension") {
        CheckFileParsing(testOpenFileResourcePath, "IncorrectFileExtension.csv", ErrorType::IncorrectFileExtension);
    }

    SECTION("FileIsNotOpen") {
        CheckFileParsing(testOpenFileResourcePath, "NonExistentTxtFile.txt", ErrorType::FileIsNotOpen);
    }
}

TEST_CASE("ParsingTxtFile", "[YadroComputerClubTest]") {
    auto testParseFileResourcePath = fs::path(CURRENT_SOURCE_FILE_PATH).parent_path().append(RESOURCE_FOLDER)
                                     .append(TEST_PARSE_FILE_FOLDER);

    SECTION("EmptyFile") {
        CheckFileParsing(testParseFileResourcePath, "EmptyFile.txt", ErrorType::IncorrectNumberOfTables);
    }

    SECTION("IncorrectNumberOfTables") {
        CheckFileParsing(testParseFileResourcePath, "IncorrectNumberOfTables.txt", ErrorType::IncorrectNumberOfTables);
    }

    SECTION("IncorrectWorkingHoursFormat") {
        CheckFileParsing(testParseFileResourcePath, "IncorrectWorkingHoursFormat.txt",
                         ErrorType::IncorrectWorkingHoursFormat);
    }

    SECTION("IncorrectOpeningTimeFormat") {
        CheckFileParsing(testParseFileResourcePath, "IncorrectOpeningTimeFormat.txt", ErrorType::IncorrectOpeningTime);
    }

    SECTION("IncorrectClosingTimeFormat") {
        CheckFileParsing(testParseFileResourcePath, "IncorrectClosingTimeFormat.txt", ErrorType::IncorrectClosingTime);
    }

    SECTION("IncorrectCostPerHour") {
        CheckFileParsing(testParseFileResourcePath, "IncorrectCostPerHour.txt", ErrorType::IncorrectCostPerHour);
    }

    SECTION("NoDelimitersInEventDescription") {
        CheckFileParsing(testParseFileResourcePath, "NoDelimitersInEventDescription.txt", ErrorType::NoDelimiters);
    }

    SECTION("OneDelimiterInEventDescription") {
        CheckFileParsing(testParseFileResourcePath, "OneDelimiterInEventDescription.txt",
                         ErrorType::TooLittleDelimiters);
    }

    SECTION("IncorrectEventTime") {
        CheckFileParsing(testParseFileResourcePath, "IncorrectEventTime.txt", ErrorType::IncorrectEventTime);
    }

    SECTION("IncorrectEventID") {
        CheckFileParsing(testParseFileResourcePath, "IncorrectEventID.txt", ErrorType::IncorrectEventID);
    }

    SECTION("IncorrectEventBodyForSecondID") {
        CheckFileParsing(testParseFileResourcePath, "IncorrectEventBodyForSecondID.txt",
                         ErrorType::IncorrectBodyEventWithSecondID);
    }

    SECTION("IncorrectClientName") {
        CheckFileParsing(testParseFileResourcePath, "IncorrectClientName.txt", ErrorType::IncorrectClientName);
    }

    SECTION("IncorrectTableNumber") {
        CheckFileParsing(testParseFileResourcePath, "IncorrectTableNumber.txt", ErrorType::IncorrectTableNumber);
    }

    // Example from the test task definition
    SECTION("CorrectParsingFile") {
        CheckFileParsing(testParseFileResourcePath, "CorrectParsingFile.txt", ErrorType::Success);
    }
}

TEST_CASE("SemanticErrorsInInputFileData", "[YadroComputerClubTest]") {
    auto testSemanticErrorsResourcePath = fs::path(CURRENT_SOURCE_FILE_PATH).parent_path().append(RESOURCE_FOLDER)
                                          .append(TEST_SEMANTIC_ERRORS_FOLDER);
    TaskSolver taskSolver;

    SECTION("OpeningTimeIsNotLessThanClosingTime") {
        CheckFileParsing(testSemanticErrorsResourcePath, "OpeningTimeIsNotLessThanClosingTime.txt",
                         ErrorType::OpeningTimeIsNotLessThanClosingTime);
    }

    SECTION("TableNumberMoreThanNumberOfTables") {
        CheckFileParsing(testSemanticErrorsResourcePath, "TableNumberMoreThanNumberOfTables.txt",
                         ErrorType::TableNumberMoreThanNumberOfTables);
    }

    SECTION("EventTimeIsNotLessThanClosingTime") {
        CheckFileParsing(testSemanticErrorsResourcePath, "EventTimeIsNotLessThanClosingTime.txt",
                         ErrorType::EventTimeIsNotLessThanClosingTime);
    }

    SECTION("IncorrectEventTimeSequence") {
        const auto taskSolverResult = taskSolver.run(testSemanticErrorsResourcePath
                                                     .append("IncorrectEventTimeSequence.txt").string());
        REQUIRE(taskSolverResult == static_cast<int>(ErrorType::IncorrectEventTimeSequence));
    }
}

TEST_CASE("SuccessOutputResult", "[YadroComputerClubTest]") {
    auto testSuccessOutputResultResourcePath = fs::path(CURRENT_SOURCE_FILE_PATH).parent_path()
                                               .append(RESOURCE_FOLDER).append(TEST_SUCCESS_OUTPUT_RESULT_FOLDER);

   TaskSolver taskSolver;

    auto calculate = [&testSuccessOutputResultResourcePath, &taskSolver](const std::string& filePath) {
        const auto taskSolverResult = taskSolver.run(testSuccessOutputResultResourcePath.append(filePath).string());
        REQUIRE(taskSolverResult == static_cast<int>(ErrorType::Success));
    };

    SECTION("AllMistakesAndEventIDs") {
        calculate("AllMistakesAndEventIDs.txt");

        Time temp = taskSolver.getEntireWorkingTimeOfTable(1);
        REQUIRE(temp.hours == 10);
        REQUIRE(temp.minutes == 51);
        REQUIRE(taskSolver.getTableIncome(1) == 120);

        temp = taskSolver.getEntireWorkingTimeOfTable(2);
        REQUIRE(temp.hours == 7);
        REQUIRE(temp.minutes == 23);
        REQUIRE(taskSolver.getTableIncome(2) == 80);

        temp = taskSolver.getEntireWorkingTimeOfTable(3);
        REQUIRE(temp.hours == 11);
        REQUIRE(temp.minutes == 1);
        REQUIRE(taskSolver.getTableIncome(3) == 120);

        temp = taskSolver.getEntireWorkingTimeOfTable(4);
        REQUIRE(temp.hours == 10);
        REQUIRE(temp.minutes == 59);
        REQUIRE(taskSolver.getTableIncome(4) == 120);
    }

    SECTION("PerfectWorkingDay") {
        calculate("PerfectWorkingDay.txt");

        Time temp = taskSolver.getEntireWorkingTimeOfTable(1);
        REQUIRE(temp.hours == 9);
        REQUIRE(temp.minutes == 21);
        REQUIRE(taskSolver.getTableIncome(1) == 1000);

        temp = taskSolver.getEntireWorkingTimeOfTable(2);
        REQUIRE(temp.hours == 8);
        REQUIRE(temp.minutes == 57);
        REQUIRE(taskSolver.getTableIncome(2) == 1100);

        temp = taskSolver.getEntireWorkingTimeOfTable(3);
        REQUIRE(temp.hours == 7);
        REQUIRE(temp.minutes == 0);
        REQUIRE(taskSolver.getTableIncome(3) == 800);

        temp = taskSolver.getEntireWorkingTimeOfTable(4);
        REQUIRE(temp.hours == 9);
        REQUIRE(temp.minutes == 45);
        REQUIRE(taskSolver.getTableIncome(4) == 1100);

        temp = taskSolver.getEntireWorkingTimeOfTable(5);
        REQUIRE(temp.hours == 10);
        REQUIRE(temp.minutes == 24);
        REQUIRE(taskSolver.getTableIncome(5) == 1200);
    }

    // Example from the test task definition
    SECTION("FromTestTask") {
        calculate("FromTestTask.txt");

        REQUIRE(taskSolver.getEntireWorkingTimeOfTable(1).hours == 5);
        REQUIRE(taskSolver.getEntireWorkingTimeOfTable(1).minutes == 58);
        REQUIRE(taskSolver.getTableIncome(1) == 70);

        REQUIRE(taskSolver.getEntireWorkingTimeOfTable(2).hours == 2);
        REQUIRE(taskSolver.getEntireWorkingTimeOfTable(2).minutes == 18);
        REQUIRE(taskSolver.getTableIncome(2) == 30);

        REQUIRE(taskSolver.getEntireWorkingTimeOfTable(3).hours == 8);
        REQUIRE(taskSolver.getEntireWorkingTimeOfTable(3).minutes == 1);
        REQUIRE(taskSolver.getTableIncome(3) == 90);
    }
}

TEST_CASE("PrintOutputResult", "[YadroComputerClubTest]") {
    auto testPrintOutputResultResourcePath = fs::path(CURRENT_SOURCE_FILE_PATH).parent_path().append(RESOURCE_FOLDER)
                                             .append(TEST_PRINT_OUTPUT_RESULT_FOLDER);

    TaskSolver taskSolver;
    taskSolver.run(testPrintOutputResultResourcePath.append("FromTestTask.txt").string());

    REQUIRE(taskSolver.getOutputResult() == "09:00\n"
                                            "08:48 1 client1\n"
                                            "08:48 13 NotOpenYet\n"
                                            "09:41 1 client1\n"
                                            "09:48 1 client2\n"
                                            "09:52 3 client1\n"
                                            "09:52 13 ICanWaitNoLonger!\n"
                                            "09:54 2 client1 1\n"
                                            "10:25 2 client2 2\n"
                                            "10:58 1 client3\n"
                                            "10:59 2 client3 3\n"
                                            "11:30 1 client4\n"
                                            "11:35 2 client4 2\n"
                                            "11:35 13 PlaceIsBusy\n"
                                            "11:45 3 client4\n"
                                            "12:33 4 client1\n"
                                            "12:33 12 client4 1\n"
                                            "12:43 4 client2\n"
                                            "15:52 4 client4\n"
                                            "19:00 11 client3\n"
                                            "19:00\n"
                                            "1 70 05:58\n"
                                            "2 30 02:18\n"
                                            "3 90 08:01\n");
}
