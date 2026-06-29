cmake_minimum_required(VERSION 3.16)
project(gbemu CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(raylib QUIET)
if(NOT raylib_FOUND)
  include(FetchContent)
  FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
    )
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(raylib)
endif()

add_executable(gbemu
    src/main.cpp
    src/readfile.cpp
    src/CPU/registers.cpp
    src/CPU/CPU.cpp
)

# Headers du projet (lib/utils/*.hpp).
target_include_directories(gbemu PRIVATE lib lib/utils lib/CPU)

target_link_libraries(gbemu PRIVATE raylib)
