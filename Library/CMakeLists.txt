project(ModuLinkLibrary VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(json11
    GIT_REPOSITORY https://github.com/aseprite/json11.git)
FetchContent_MakeAvailable(json11)

add_library(mavlinkcpp STATIC
    src/host_messenger.cpp)

add_executable(mavlink2modulinkd
    src/main.cpp)

target_link_libraries(mavlink2modulinkd PRIVATE mavlinkcpp json11)

target_include_directories(mavlinkcpp PRIVATE inc)
target_include_directories(mavlink2modulinkd PRIVATE inc)
