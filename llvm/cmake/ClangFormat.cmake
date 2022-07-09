####################################################################
# This file is part of the tool Normalizer of the project Discover.
#
# Copyright (c) 2020-2022 Singapore Blockchain Innovation Programme.
# All rights reserved.
####################################################################

file(GLOB_RECURSE SOURCE_FILES
  ${LLVM_MAIN_SRC_DIR}/include/*/*.h
  ${LLVM_MAIN_SRC_DIR}/include/*/*.h
  ${LLVM_MAIN_SRC_DIR}/include/*/*/*.h
  ${LLVM_MAIN_SRC_DIR}/lib/*/*.h
  ${LLVM_MAIN_SRC_DIR}/lib/*/*.cpp
  ${LLVM_MAIN_SRC_DIR}/lib/*/*.h
  ${LLVM_MAIN_SRC_DIR}/lib/*/*.cpp
  ${LLVM_MAIN_SRC_DIR}/lib/*/*/*.h
  ${LLVM_MAIN_SRC_DIR}/lib/*/*/*.cpp
  ${LLVM_MAIN_SRC_DIR}/lib/*/*/*/*.h
  ${LLVM_MAIN_SRC_DIR}/lib/*/*/*/*.cpp
  )

# # debugging
# foreach (FILE ${SOURCE_FILES})
#   message (STATUS "Source file: ${FILE}")
# endforeach ()

add_custom_target(
  clangformat
  COMMAND clang-format
  -style=LLVM
  -i
  ${SOURCE_FILES})
