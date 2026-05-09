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

#include <bit>
#include <fstream>

namespace nfrg::util {

class Beatmap {
protected:
    Uint8 tracks_;
    std::string caption_;
    Uint32 bmf_offset_;
    Uint32 bmf_size_;
    Uint32 beatmap_delay_;

public:
    Beatmap(Uint8 tracks, std::string&& caption, Uint32 bmf_offset, Uint32 bmf_size, Uint32 beatmap_delay)
        : tracks_(tracks), caption_(std::move(caption)), bmf_offset_(bmf_offset), bmf_size_(bmf_size),
          beatmap_delay_(beatmap_delay) {}

    [[nodiscard]] auto get_trucks() const -> Uint8 {
        return tracks_;
    }

    [[nodiscard]] auto get_caption() const -> std::string_view {
        return caption_;
    }

    [[nodiscard]] auto get_bmf_info() const -> std::pair<Uint32, Uint32> {
        return { bmf_offset_, bmf_size_ };
    }

    [[nodiscard]] auto get_beatmap_delay() const -> Uint32 {
        return beatmap_delay_;
    }

    [[nodiscard]] auto parse(std::ifstream& ifs) const -> std::vector<Uint16> {

        std::vector<Uint16> beatmap_binary;
        beatmap_binary.reserve(bmf_size_);

        auto g = ifs.tellg();

        Uint16 data;
        ifs.seekg(bmf_offset_, std::ios::beg);
        for (Uint32 i = 0; i < bmf_size_; ++i) {
            ifs.read(reinterpret_cast<char*>(&data), 2);
            if constexpr (std::endian::native == std::endian::little) {
                data = std::byteswap(data);
            }
            beatmap_binary.push_back(data);
        }

        if (ifs.eof()) {
            ifs.clear();
        }

        ifs.seekg(g, std::ios::beg);
        return beatmap_binary;
    };
};

}