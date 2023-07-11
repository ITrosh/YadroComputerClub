add_library(TaskSolver STATIC
    ${CMAKE_CURRENT_LIST_DIR}/TaskSolver.cpp
)

target_include_directories(TaskSolver PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(TaskSolver PUBLIC FileParser EventHandler)