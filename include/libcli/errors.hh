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
#elif __linux__
    #include <errno.h>
#endif

#include <string>
#include <optional>

namespace libcli {
    class Error : public std::exception {
    protected:
        static inline uint64_t error() {
#ifdef _WIN32
            return GetLastError();
#elif __linux__
            return errno;
#endif
        }

        // corresponds to code from GetLastError() on win32 and errno on unix
        const std::optional<uint64_t> _err;
        std::string _msg;

    private:
        std::string format(const std::string& msg, uint64_t err) {
            return msg + " (" + std::to_string(err) + ")";
        }

    public:
        Error(const std::string_view& msg, std::optional<uint64_t> err = {})
            : _err(err)
            , _msg(msg) {
            if (_err.has_value())
                _msg = format(_msg, _err.value());
        }

        explicit Error(std::optional<uint64_t> err = {})
            : _err(err)
            , _msg("Terminal error") {
            if (_err.has_value())
                _msg = format(_msg, _err.value());
        }

        explicit Error(const std::string_view& msg)
            : _err({})
            , _msg(msg) {
        }

        virtual ~Error() noexcept {}

        virtual const char* what() const noexcept {
            return _msg.c_str();
        }

        std::optional<uint64_t> os_error_code() const noexcept {
            return _err;
        }

        std::optional<std::string> os_error_message() const noexcept {
            return {}; // TODO get the message of the error
        }

        static Error from_error() {
            return Error("Terminal error", error());
        }

        static Error from_error(const std::string_view& msg) {
            return Error(msg, error());
        }
    };

    class ErrorInvalidHandle : public Error {
    public:
        explicit ErrorInvalidHandle(std::optional<uint64_t> err = {})
            : Error("Invalid terminal handle", err) {
        }

        static ErrorInvalidHandle from_error() {
            return ErrorInvalidHandle(error());
        }
    };

    class ErrorIO : public Error {
    public:
        explicit ErrorIO(std::optional<uint64_t> err = {})
            : Error("Terminal I/O error", err) {
        }

        static ErrorIO from_error() {
            return ErrorIO(error());
        }
    };
}
