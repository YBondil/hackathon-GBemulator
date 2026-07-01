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

# Sources communes (tout sauf le main), partagées par gbemu et les tests visuels.
set(GB_SOURCES
    src/readfile.cpp
    src/registers.cpp
    src/CPU/CPU.cpp
    src/CPU/CPU_instructions.cpp
    src/CPU/opcode_names.cpp
    src/timer.cpp
    src/memory.cpp
    src/video/PPU.cpp
    src/video/screen.cpp
    src/boot.cpp
)

set(GB_INCLUDES lib lib/utils lib/CPU lib/video)

# --- exécutable principal ---
add_executable(gbemu src/main.cpp ${GB_SOURCES})
target_include_directories(gbemu PRIVATE ${GB_INCLUDES})
target_link_libraries(gbemu PRIVATE raylib)

# --- test visuel du PPU (affiche les dumps de roms/exports_memoire) ---
add_executable(ppu_test tests/PPU_test.cpp ${GB_SOURCES})
target_include_directories(ppu_test PRIVATE ${GB_INCLUDES})
target_link_libraries(ppu_test PRIVATE raylib)
