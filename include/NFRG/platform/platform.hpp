/* Copyright (C) 2026 NewrollSama (https://github.com/NewrollSama)
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

namespace nfrg::platform {

auto utf8_to_ansi(std::string& utf8_string) -> std::string;

class SHA256Calculator {
private:
    class Impl;
    std::unique_ptr<Impl> p_impl;

public:
    SHA256Calculator();
    ~SHA256Calculator();

    SHA256Calculator(const SHA256Calculator&) = delete;
    auto operator=(const SHA256Calculator&) -> SHA256Calculator& = delete;

    void update(const char* data, size_t size);

    auto finalize() -> std::string;
};

}