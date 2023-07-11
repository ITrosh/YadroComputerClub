add_library(Utils INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/Constants.h
    ${CMAKE_CURRENT_LIST_DIR}/ErrorTypes.h
    ${CMAKE_CURRENT_LIST_DIR}/InputFileData.h
)

target_include_directories(Utils INTERFACE ${CMAKE_CURRENT_LIST_DIR})