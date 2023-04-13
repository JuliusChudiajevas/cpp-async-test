cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)

# set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
project(cppProgram)

set(SOURCES
    main.cpp
)

# set(YOUR_VARIABLE_DIR ${PROJECT_SOURCE_DIR}/yourPath)
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# WIN32
# add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

# install(TARGETS game DESTINATION ${PROJECT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}
)