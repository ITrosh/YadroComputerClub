add_library(EventHandler STATIC
    ${CMAKE_CURRENT_LIST_DIR}/EventHandler.cpp
)

target_include_directories(EventHandler PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(EventHandler PUBLIC Utils)