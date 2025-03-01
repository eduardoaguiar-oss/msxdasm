cmake_minimum_required(VERSION 3.14)

project(
    msxdasm
    VERSION 3.0
    DESCRIPTION "MSX disassembler"
    HOMEPAGE_URL "https://www.github.com/eduardoaguiar-oss/msxdasm"
    LANGUAGES CXX
)

# ---- Declare executable ----

add_executable(msxdasm_exe msxdasm.cpp cartridge.cpp symbol_table.cpp navigator.cpp disassembler.cpp)
add_executable(msxdasm::exe ALIAS msxdasm_exe)

set_property(TARGET msxdasm_exe PROPERTY OUTPUT_NAME msxdasm)

target_compile_features(msxdasm_exe PRIVATE cxx_std_17)

# ---- Developer mode ----

if(NOT msxdasm_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of msxdasm"
  )
endif()
