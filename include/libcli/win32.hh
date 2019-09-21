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

#include "libcli.hh"

// removes bloat from windows.h
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace libcli::win32 {
   const COLORREF from_color(const Color& color) {
      return RGB(color.r, color.g, color.b);
   }

   const Color to_color(const COLORREF& color) {
      return Color(
         GetRValue(color),
         GetGValue(color),
         GetBValue(color)
      );
   }
}

namespace libcli {
   static HANDLE winapi_handle = nullptr;

   void init() {
      // enable unicode support
      SetConsoleOutputCP(CP_UTF8);
      SetConsoleCP(CP_UTF8);

      // get winapi handle
      if (winapi_handle == nullptr) {
         winapi_handle = GetStdHandle(STD_OUTPUT_HANDLE);

         if (winapi_handle == INVALID_HANDLE_VALUE) {
            throw InvalidHandleTerminalError();
         }
      }
   }

   void destroy() {
      // close winapi handle
      CloseHandle(winapi_handle);
   }

   void write(const std::string_view& data) {
      // successfully written, return
      if (WriteConsoleA(winapi_handle, data.data(), (DWORD)data.size(), NULL, NULL)) {
         return;
      }

      // get error from winapi
      auto error = GetLastError();

      // throw fitting exception
      switch (error) {
         case ERROR_INVALID_HANDLE: throw InvalidHandleTerminalError(error);
         default: throw WriteTerminalError(error);
      }
   }
}
