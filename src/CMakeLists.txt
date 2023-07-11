add_subdirectory(file_parser)
add_subdirectory(event_handler)
add_subdirectory(task_solver)
add_subdirectory(utils)

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE TaskSolver)
