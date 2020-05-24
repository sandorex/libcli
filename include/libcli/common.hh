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

#pragma once

#ifdef _WIN32
    // removes bloat from windows.h
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#include <cstdint>
#include <variant>
#include <optional>
#include <string>

namespace libcli {
#ifdef _WIN32
   using RawHandle = HANDLE;
#elif __linux__
   using RawHandle = int;
#endif

    class Handle {
    private:
        RawHandle _handle;

    public:
        Handle()
            : _handle() {
        }
        explicit Handle(RawHandle handle)
            : _handle(handle) {
        }
        ~Handle();

        const RawHandle& operator*() const {
            return _handle;
        }

        // gets the handle from current terminal
        static Handle from_terminal();

#ifdef _WIN32
        // TODO create new terminal windows (maybe on linux too?)
#elif __linux__
        // creates handle from a file descriptor
        static Handle from_fd(int fd) {
            return Handle(fd);
        }
#endif
        const Handle& write(const std::string_view&) const;
    };

    struct Position {
        int32_t x, y;

#ifdef _WIN32
        Position(const COORD& coord)
            : x(coord.X)
            , y(coord.Y) {
        }

        operator COORD() const {
            return COORD { static_cast<SHORT>(x), static_cast<SHORT>(y) };
        }
#endif
    };

    struct Color {
        uint8_t r, g, b, a;

        Color(uint32_t rgba)
            : r((rgba >> 24) & 0xFF)
            , g((rgba >> 16) & 0xFF)
            , b((rgba >> 8) & 0xFF)
            , a(rgba & 0xFF) {
        }

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)
            : r(r)
            , g(g)
            , b(b)
            , a(a) {
        }

#ifdef _WIN32
        Color(const COLORREF& color)
            : r(GetRValue(color))
            , g(GetGValue(color))
            , b(GetBValue(color))
            , a(0xFF) {
        }

        operator COLORREF() const {
            return RGB(r, g, b);
        }
#endif
    };

    struct Cell {
        std::optional<std::variant<char, std::string_view>> character{};

        std::optional<Color> fg_color{};
        std::optional<Color> bg_color{};
    };
}
