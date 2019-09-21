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

#pragma once

#define libcli_VERSION_MAJOR 0
#define libcli_VERSION_MINOR 1
#define libcli_VERSION_PATCH 0
#define libcli_VERSION_STRING "0.1.0"

#include <string>
#include <variant>
#include <array>

namespace libcli {
   struct TerminalError : std::runtime_error {
      // corresponds to code from GetLastError() on win32
      // error is errno on unix
      int _err;

      TerminalError(const char* msg, const int err)
         : std::runtime_error(std::string(msg) + " [" + std::to_string(err) + "]") {
      }
   };

   struct InvalidHandleTerminalError : TerminalError {
      InvalidHandleTerminalError(const int err = 0)
         : TerminalError("Invalid terminal handle", err) {
      }
   };

   struct WriteTerminalError : TerminalError {
      WriteTerminalError(const int err = 0)
         : TerminalError("Error writing to the terminal", err) {
      }
   };

   struct ReadTerminalError : TerminalError {
      ReadTerminalError(const int err = 0)
         : TerminalError("Error reading from the terminal", err) {
      }
   };

   struct Position {
      int x, y;
   };

   struct Color {
      uint8_t r, g, b, a;

      Color(uint32_t rgba)
         :  r((rgba >> 24) & 0xFF),
            g((rgba >> 16) & 0xFF),
            b((rgba >> 8) & 0xFF),
            a(rgba & 0xFF) {
      }

      Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)
         :  r(r),
            g(g),
            b(b),
            a(a) {
      }
   };

   struct Pixel {
      std::variant<char, std::string_view> character = ' ';

#ifdef _WIN32
      uint8_t fg_color_index = 0;
      uint8_t bg_color_index = 0;
#else
      Color fg_color { 0xFF'FF'FF'FF };
      Color bg_color { 0x00'00'00'00 };
#endif
   };

   void init();
   void destroy();
   void write(const std::string_view& data);
}

// include platform specific stuff
#ifdef _WIN32
#include "win32.hh"
#else
#include "unix.hh"
#endif
