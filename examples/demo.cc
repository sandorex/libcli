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
#include <string>

#include <libcli/libcli.hh>

using namespace libcli;

const std::string s = "[\u2717,✗]\n";

int main() {
    Handle handle;

    try {
       handle = Handle::from_terminal();
    } catch(const std::exception& e) {
       std::cout << e.what() << '\n';
       return 1;
    }
    try {
       handle.write(s);
    } catch(const std::exception& e) {
       std::cout << "ERROR: " << e.what() << '\n';
    }

    return 0;
}
