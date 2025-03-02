cmake_minimum_required(VERSION 3.20.0)

project(
    msxdasm
    VERSION 3.0
    DESCRIPTION "MSX disassembler"
    HOMEPAGE_URL "https://www.github.com/eduardoaguiar-oss/msxdasm"
    LANGUAGES CXX
)

# ---- Msxdasm ----

# add_compile_options(-Wall -Wextra -Wpedantic)
add_executable(msxdasm msxdasm.cpp cartridge.cpp symbol_table.cpp navigator.cpp disassembler.cpp)
target_compile_features(msxdasm PRIVATE cxx_std_17)
target_compile_options(msxdasm PRIVATE -Wall -Wextra -Wpedantic)

# ---- Package definition ----

include(InstallRequiredSystemLibraries)
set(CPACK_SOURCE_GENERATOR "TXZ;ZIP")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/.git/;/.vscode/;/.gitignore/;todo.txt")
include(CPack)
