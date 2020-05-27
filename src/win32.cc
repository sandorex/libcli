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

#ifdef _WIN32

#include "libcli/handle.hh"
#include "libcli/errors.hh"

using namespace libcli;

Handle::Handle()
    : _handle(INVALID_HANDLE_VALUE)
    , _original_handle(INVALID_HANDLE_VALUE)
    , _buffer_handle(INVALID_HANDLE_VALUE) {
}

Handle::Handle(const Handle& handle)
    : _handle(handle._handle)
    , _original_handle(handle._handle)
    , _buffer_handle(handle._buffer_handle) {
}

Handle::Handle(const RawHandle& handle)
    : _handle(handle)
    , _original_handle(handle)
    , _buffer_handle(INVALID_HANDLE_VALUE) {
}

Handle::~Handle() {
    if (_buffer_handle != INVALID_HANDLE_VALUE) {
        // switch the buffer back
        SetConsoleActiveScreenBuffer(_original_handle);

        // free the new buffer
        CloseHandle(_buffer_handle); // TODO error checking?!?
    }
}

const Handle& Handle::set_vt_mode(bool) const {
    DWORD mode = 0;
    if (!GetConsoleMode(_handle, &mode))
        throw Error::from_error("Unable to read console mode");

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(_handle, mode))
        throw Error::from_error("Unable to set console mode");

    return *this;
}

#endif
