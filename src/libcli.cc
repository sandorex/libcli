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

#include "libcli/common.hh"
#include "libcli/errors.hh"

#include <vector>
#include <iostream>

#if defined(_WIN32)
#else
    #include <unistd.h>
    #include <fcntl.h>
    #include <termios.h>
#endif

using namespace libcli;

Handle::~Handle() {}

Handle Handle::from_terminal() {
#if defined(_WIN32)
    // enable unicode support
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE || handle == NULL) {
        throw Error::from_error("Could not get the handle");
    }

    return Handle(handle);
#elif defined(__linux__)
    const char* dev;
    int fd = -1;

    // try to get tty path from stdin, stdout, stderr
    dev = ttyname(STDIN_FILENO);

    if (!dev)
        dev = ttyname(STDOUT_FILENO);

    if (!dev)
        dev = ttyname(STDERR_FILENO);

    // none are valid
    if (!dev)
        throw Error::from_error("Could not get the ttyname");

    // open tty device
    // repeat if interrupted by system
    do {
        fd = open(dev, O_RDWR | O_NOCTTY);
    } while (fd == -1 && errno == EINTR);

    // file descriptor is invalid
    if (fd == -1)
        throw Error::from_error("Could not create the file descriptor");

    return Handle(fd);
#endif
}

const Handle& Handle::write(const std::string_view& data) const {
#if defined(_WIN32)
    // const auto dw_flags = MB_ERR_INVALID_CHARS;

    // // get the require buffer size
    // const auto buffer_size = MultiByteToWideChar(CP_UTF8, dw_flags, data.data(), -1, NULL, 0);

    // // allocate the buffer
    // std::vector<wchar_t> buffer(buffer_size);

    // // convert utf8 to utf16 string
    // if (MultiByteToWideChar(CP_UTF8, dw_flags, data.data(), -1, buffer.data(), buffer_size) == 0)
    //     throw Error::from_error("Could not convert input into UTF16 string");

    // NOTE it is writing unicode to console even though it is the ANSI version,
    // i am going to continue using this until i find any issue with it
    //
    // in case this breaks simply use either utf8cpp or MultiByteToWideChar code
    // above

    // successfully written, return
    if (WriteConsoleA(_handle, data.data(), static_cast<DWORD>(data.size()), NULL, NULL)) {
        return *this;
    }

    // get error from winapi
    auto error = GetLastError();

    // throw a fitting exception
    switch (error) {
        case ERROR_INVALID_HANDLE: throw ErrorInvalidHandle(error);
        default: throw Error(error);
    }
#elif defined(__linux__)
    // repeatedly try to write data (because of system interrupts)
    // NOTE ::write in this case is the C function write
    int result;
    do {
        result = ::write(_handle, data.data(), data.size());
    } while (result == -1 && errno == EINTR);

    // write failed
    if (result != data.size()) {
        throw ErrorIO::from_error();
    }
#endif

    return *this;
}