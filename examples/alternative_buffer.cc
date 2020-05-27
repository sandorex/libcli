// Copyright 2020 Aleksandar Radivojević
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdlib>
#include <iostream>
#include <thread>

#include <libcli/libcli.hh>

using namespace libcli;

int main() {
    Handle handle;
    try {
        handle = Handle::from_terminal();
    } catch(const std::exception& e) {
        std::cout << "from_terminal: " << e.what() << '\n';
        return 1;
    }

    try {
        handle.write("I am going to switch buffers in 5 seconds");
    } catch(const std::exception& e) {
        std::cout << "write: " << e.what() << '\n';
        return 1;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    try {
        handle.swap_buffers();
    } catch(const std::exception& e) {
        std::cout << "swap_buffers: " << e.what() << '\n';
        return 1;
    }

    try {
        handle.write("I am going to switch buffers again in 5 seconds");
    } catch(const std::exception& e) {
        std::cout << "write: " << e.what() << '\n';
        return 1;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
