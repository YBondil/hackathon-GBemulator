# Tests unitaires — autonomes, sans raylib.
#
# Utilisation :
#   cmake -S tests -B build-tests
#   cmake --build build-tests
#   ctest --test-dir build-tests --output-on-failure
#
# (peut aussi être inclus depuis le CMakeLists principal via add_subdirectory)

cmake_minimum_required(VERSION 3.16)

# Permet de configurer ce dossier seul OU en sous-répertoire du projet.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(gbemu_tests CXX)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

enable_testing()

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Noyau testable sans CPU ni raylib (registres + timer).
add_library(gbcore STATIC
    ${ROOT}/src/registers.cpp
    ${ROOT}/src/timer.cpp
)
target_include_directories(gbcore PUBLIC
    ${ROOT}/lib ${ROOT}/lib/utils ${ROOT}/lib/CPU ${CMAKE_CURRENT_SOURCE_DIR})

# --- test_sanity : bitwise / registres / timer ---
add_executable(test_sanity test_sanity.cpp)
target_link_libraries(test_sanity PRIVATE gbcore)
add_test(NAME sanity COMMAND test_sanity)

# --- test_fakememory : routage MMIO + IF ---
add_executable(test_fakememory test_fakememory.cpp)
target_link_libraries(test_fakememory PRIVATE gbcore)
add_test(NAME fakememory COMMAND test_fakememory)

# --- test_cpu : stack / interruptions / opcodes (Phase 3) ---
# NB: test_cpu.cpp fournit un CPU::run_opcode stub tant que le vrai n'existe pas.
add_executable(test_cpu
    test_cpu.cpp
    ${ROOT}/src/CPU/CPU.cpp
    ${ROOT}/src/CPU/CPU_instructions.cpp
)
target_link_libraries(test_cpu PRIVATE gbcore)
add_test(NAME cpu COMMAND test_cpu)
