add_library(FileParser STATIC
    ${CMAKE_CURRENT_LIST_DIR}/FileParser.cpp
)

target_include_directories(FileParser PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(FileParser PUBLIC Utils)