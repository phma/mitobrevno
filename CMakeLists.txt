project(mitobrevno)
cmake_minimum_required(VERSION 3.8.0)
# Copyright 2020 Pierre Abbat.
# This file is part of Mitobrevno.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include(TestBigEndian)
include(CheckTypeSize)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(SHARE_DIR ${CMAKE_INSTALL_PREFIX}/share/mitobrevno)

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if (${FORCE_COLORED_OUTPUT})
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options (-fcolor-diagnostics)
  endif ()
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(Qt5 COMPONENTS Core Widgets Gui LinguistTools REQUIRED)
find_package(Threads)

#qt5_add_resources(lib_resources mitobrevno.qrc)
qt5_add_translation(qm_files mitobrevno_en.ts mitobrevno_es.ts)
# To update translations, run "lupdate *.cpp -ts *.ts" in the source directory.

add_executable(mitobrevno main.cpp mainwindow.cpp mitobrevno.cpp 
	       timescale.cpp zoom.cpp
	       ${lib_resources} ${qm_files})
target_link_libraries(mitobrevno ${CMAKE_THREAD_LIBS_INIT} Qt5::Widgets Qt5::Core)
add_library(libmitobrevno STATIC mitobrevno.cpp)

include(CheckIncludeFiles)

# Define NO_INSTALL when compiling for fuzzing. This avoids the error
# "The install of the mitobrevno target requires changing an RPATH", which
# occurs when using the AFL compiler wrapper with the Ninja generator.
# There is no need to install a binary built for fuzzing.
if (NOT DEFINED NO_INSTALL)
install(TARGETS mitobrevno DESTINATION bin)
install(TARGETS libmitobrevno DESTINATION lib)
install(FILES mitobrevno.h DESTINATION include)
endif ()

test_big_endian(BIGENDIAN)
check_type_size("int" INT)
check_type_size("int *" INT_POINTER)

set(MITOBREVNO_MAJOR_VERSION 0)
set(MITOBREVNO_MINOR_VERSION 1)
set(MITOBREVNO_PATCH_VERSION 0)
set(MITOBREVNO_VERSION ${MITOBREVNO_MAJOR_VERSION}.${MITOBREVNO_MINOR_VERSION}.${MITOBREVNO_PATCH_VERSION})
set(COPY_YEAR 2020)

set_target_properties(libmitobrevno PROPERTIES OUTPUT_NAME "mitobrevno" VERSION ${MITOBREVNO_VERSION})

include_directories(${PROJECT_BINARY_DIR})
configure_file (config.h.in config.h)

set(CPACK_PACKAGE_VERSION_MAJOR ${MITOBREVNO_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${MITOBREVNO_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${MITOBREVNO_PATCH_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set(CPACK_SOURCE_IGNORE_FILES /\\\\.git;.*~)
include(CPack)
