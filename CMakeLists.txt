#                           libcli
#
# Copyright 2019 Aleksandar Radivojevic
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#	 http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.10)

project(libcli VERSION 0.1.0 LANGUAGES C CXX)


add_library(libcli INTERFACE)

target_compile_features (libcli INTERFACE cxx_std_17)

target_include_directories(libcli INTERFACE ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(examples)
