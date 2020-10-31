# Copyright 2019 Aleksandar Radivojević
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required (VERSION 3.10)

project (libcli VERSION 0.1.0 LANGUAGES C CXX)

if (NOT "${PROJECT_NAME}" STREQUAL "${CMAKE_PROJECT_NAME}")
	set (libcli_SUBPROJECT ON CACHE BOOL "Disables tests and examples automatically")
endif ()

option (libcli_BUILD_EXAMPLES "Build examples" ON)

if (NOT libcli_SUBPROJECT)
	message (STATUS "libcli version ${libcli_VERSION}")
endif ()

configure_file (src/version.hh.in ${PROJECT_BINARY_DIR}/include/libcli/version.hh @ONLY)

add_library (libcli STATIC
    src/libcli.cc
    src/win32.cc
    src/linux.cc
    src/handle.cc
)
add_library (libcli::libcli ALIAS libcli)

if (UNIX)
	find_library (TERMCAP_LIBRARY ncurses)
	if (NOT TERMCAP_LIBRARY)
		find_library (TERMCAP_LIBRARY termcap)
	endif ()
	message (STATUS "Using termcap: ${TERMCAP_LIBRARY}")
endif ()

add_subdirectory (${CMAKE_SOURCE_DIR}/vendor/unibilium)
# TODO it seems that the header is not included by default??
target_include_directories (unibilium PUBLIC ${CMAKE_SOURCE_DIR}/vendor/unibilium)

# fixes utf8 for windows
if (MSVC)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif ()

set_target_properties (libcli PROPERTIES
    PREFIX ""
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)
target_compile_features (libcli PUBLIC cxx_std_17)
target_include_directories (libcli PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories (libcli PRIVATE ${PROJECT_BINARY_DIR}/include)

if (libcli_BUILD_EXAMPLES AND NOT libcli_SUBPROJECT)
    add_subdirectory  (examples)
endif ()
