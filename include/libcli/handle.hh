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

#include "libcli/common.hh"

namespace libcli {
    class Handle {
    // private:
    public:
        RawHandle _handle;

#ifdef _WIN32
        // stores the original handle when the object was created
        RawHandle _original_handle;

        // stores the handle to buffer
        RawHandle _buffer_handle;
#endif

    public:
        Handle();
        Handle(const Handle& handle);
        explicit Handle(const RawHandle&);
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
        const Handle& write_at(const std::string_view&, const Position&) const;

        // swaps buffers, usually used to keep the content same after quitting a
        // TUI application
        Handle& swap_buffers();


#ifdef _WIN32
        // enables virtual terminal mode, which enables ANSI escape commands
        // like on linux with full color support
        const Handle& set_vt_mode(bool) const;

        const Handle& enable_vt_mode() const { set_vt_mode(true); return *this; }
        const Handle& disable_vt_mode() const { set_vt_mode(false); return *this; }
#elif __linux__
        // enables raw mode TODO..
        const Handle& set_raw_mode_mode(bool) const;

        const Handle& enable_raw_mode_mode(bool) const { set_raw_mode_mode(true); return *this; }
        const Handle& disable_raw_mode_mode(bool) const { set_raw_mode_mode(false); return *this; }
#endif
    };
}
