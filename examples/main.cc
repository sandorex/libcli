//                           libcli
//
// Copyright 2019 Aleksandar Radivojevic
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// 	 http://www.apache.org/licenses/LICENSE-2.0
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

int main() {
   try {
      init();
   } catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
      return 1;
   }

   const std::string s = u8"\u2717\n"; // ✗ 2717

   // Pixel p = { u8"\u2717" };

   // if (std::holds_alternative<char>(p.character)) {
   //    std::cout << std::get<char>(p.character) << '\n';
   // } else {
   //    std::cout << std::get<std::string_view>(p.character) << '\n';
   // }

   try
   {
      write(s);
   } catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
   }

   destroy();

   return 0;
}
