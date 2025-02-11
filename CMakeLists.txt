cmake_minimum_required(VERSION 3.12)

project(my-project
        VERSION 0.0.0
        DESCRIPTION "My best project"
        LANGUAGES C CXX
)

add_library(cachelib SHARED app/app.cpp)
link_directories(${CMAKE_SOURCE_DIR}/app)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O0 -g")
add_executable(lab2 Test.cpp)

# Указываем, с какими библиотеками связываемся
target_link_libraries(lab2 cachelib)

