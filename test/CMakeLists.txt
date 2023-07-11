set(TEST_TARGET YadroComputerClubTest)

set(TEST_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/YadroComputerClubTest.cpp
)

add_executable(${TEST_TARGET} ${TEST_SOURCES})

target_link_libraries(${TEST_TARGET} PRIVATE Catch2::Catch2WithMain TaskSolver)