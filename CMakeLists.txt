cmake_minimum_required(VERSION 3.20)
project(Data-structure-and-algorithm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(data_struct INTERFACE)
target_include_directories(data_struct INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/data_struct/include
    ${CMAKE_CURRENT_SOURCE_DIR}/data_struct/include/data_struct
    ${CMAKE_CURRENT_SOURCE_DIR}/data_struct/tests
)

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/data_struct/tests/*.cpp")

foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} PRIVATE data_struct)

    if(WIN32)
        target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -g)
    else()
        target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -g -O1 -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(${TEST_NAME} PRIVATE -fsanitize=address)
    endif()
endforeach()