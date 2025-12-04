cmake_minimum_required(VERSION 3.10)
project(CalcHistory)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES
    Complex.cpp
    Quaternion.cpp
    Calculator.cpp
    main.cpp
)

add_executable(calc ${SOURCES})
if (WIN32)
    target_link_libraries(calc PRIVATE user32)
endif()

add_executable(test_history test_history.cpp Complex.cpp Quaternion.cpp Calculator.cpp)
if (WIN32)
    target_link_libraries(test_history PRIVATE user32)
endif()
