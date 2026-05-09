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
#include "NFRG/util/beatmap_set.hpp"
using namespace nfrg;
using namespace util;

#include "NFRG/platform/platform.hpp"

#include <bit>

template <typename... Args>
void read_tuple(std::ifstream& ifs, std::tuple<Args&...> t) {
    constexpr auto process = [](std::ifstream& ifs, auto& arg) -> void {
        ifs.read(reinterpret_cast<char*>(&arg), sizeof(arg));
        if constexpr (std::endian::native == std::endian::big) {
            return;
        }
        using T = std::remove_reference_t<decltype(arg)>;
        if constexpr (std::is_integral_v<T>) {
            arg = std::byteswap(arg);
        } else if constexpr (std::is_floating_point_v<T>) {
            if constexpr (sizeof(T) == 4) {
                auto bits = std::bit_cast<Uint32>(arg);
                bits = std::byteswap(bits);
                arg = std::bit_cast<T>(bits);
            } else if constexpr (sizeof(T) == 8) {
                auto bits = std::bit_cast<Uint64>(arg);
                bits = std::byteswap(bits);
                arg = std::bit_cast<T>(bits);
            }
        }
    };
    std::apply([&](auto&... arg) -> auto {
        (process(ifs, arg), ...);
    },
        t);
}

struct BeatmapSetFileHeader {
    std::array<char, 4> magic{ 'N', 'F', 'R', 'G' };
    Uint16 bpm;
    Uint16 num_beatmap;
    Uint32 bgm_offset;
    Uint32 bgm_size;
    Uint32 bgm_title_offset;
    Uint32 bgm_artist_offset;
    Uint32 bgm_preview_offset;
    Uint32 bgi_offset;
    Uint32 bgi_size;

    auto make_tuple() -> auto {
        return std::tie(magic, bpm, num_beatmap, bgm_offset, bgm_size,
            bgm_title_offset, bgm_artist_offset, bgm_preview_offset, bgi_offset, bgi_size);
    }
};

struct BeatmapFieldHeader {
    Uint8 num_tracks;
    Uint32 caption_offset;
    Uint32 bmf_offset;
    Uint32 bmf_size;
    Uint32 beatmap_delay;

    auto make_tuple() -> auto {
        return std::tie(num_tracks, caption_offset, bmf_offset, bmf_size, beatmap_delay);
    }
};

auto read_string(std::ifstream& ifs, Uint32 string_offset) -> std::string {
    std::string string;
    auto tellg = ifs.tellg();
    ifs.seekg(string_offset);
    for (char c = static_cast<char>(ifs.get()); c != '\0'; ifs.get(c)) {
        string.push_back(c);
    }
    ifs.seekg(tellg);
    return string;
}

BeatmapSet::BeatmapSet(std::string& beatmapset_path) {
#ifdef WIN32
    beatmapset_path = platform::utf8_to_ansi(beatmapset_path);
#endif

    // 打开谱面集文件
    ifs_.open(beatmapset_path, std::ios::binary | std::ios::in);

    // 读取文件头
    BeatmapSetFileHeader file_header;
    read_tuple(ifs_, file_header.make_tuple());
    bpm_ = file_header.bpm;
    beatmaps_.reserve(file_header.num_beatmap);
    bgm_size_ = file_header.bgm_size;
    bgm_offset_ = file_header.bgm_offset;
    bgm_preview_offset_ = file_header.bgm_preview_offset;
    bgi_size_ = file_header.bgi_size;
    bgi_offset_ = file_header.bgi_offset;

    // 读取曲名和艺术家
    bgm_title_ = read_string(ifs_, file_header.bgm_title_offset);
    bgm_artist_ = read_string(ifs_, file_header.bgm_artist_offset);

    // 构造谱面
    beatmaps_.reserve(file_header.num_beatmap);
    for (Uint16 i = 0; i < file_header.num_beatmap; ++i) {
        // 读取谱面头
        BeatmapFieldHeader field_header;
        read_tuple(ifs_, field_header.make_tuple());

        // 读取名称
        std::string caption = read_string(ifs_, field_header.caption_offset);
        beatmaps_.emplace_back(field_header.num_tracks, std::move(caption),
            field_header.bmf_offset, field_header.bmf_size, field_header.beatmap_delay);
    }

    // 准备读取音频文件
    bgm_.get_filelen = [](void* user_data) -> sf_count_t {
        auto& beatmapset = *static_cast<BeatmapSet*>(user_data);
        return beatmapset.bgm_size_;
    };
    bgm_.seek = [](sf_count_t offset, int whence, void* user_data) -> sf_count_t {
        auto& beatmapset = *static_cast<BeatmapSet*>(user_data);
        Sint64 pos;
        switch (whence) {
        default:
        case SF_SEEK_SET: {
            pos = beatmapset.bgm_offset_ + offset;
            break;
        }
        case SF_SEEK_CUR: {
            pos = beatmapset.ifs_.tellg() + offset;
            break;
        }
        case SF_SEEK_END: {
            pos = beatmapset.bgm_offset_ + beatmapset.bgm_size_ + offset;
            break;
        }
        }
        if (pos < beatmapset.bgm_offset_ || pos > beatmapset.bgm_offset_ + beatmapset.bgm_size_) {
            beatmapset.ifs_.setstate(std::ios::failbit);
        } else {
            beatmapset.ifs_.seekg(pos, std::ios::beg);
        }
        return static_cast<Sint64>(beatmapset.ifs_.tellg()) - beatmapset.bgm_offset_;
    };
    bgm_.read = [](void* ptr, sf_count_t count, void* user_data) -> sf_count_t {
        auto& beatmapset = *static_cast<BeatmapSet*>(user_data);
        sf_count_t read_size = beatmapset.ifs_.read(static_cast<char*>(ptr), count).gcount();
        return read_size;
    };
    bgm_.write = nullptr;  // 不需要写入功能
    bgm_.tell = [](void* user_data) -> sf_count_t {
        auto& beatmapset = *static_cast<BeatmapSet*>(user_data);
        return static_cast<sf_count_t>(beatmapset.ifs_.tellg()) - beatmapset.bgm_offset_;
    };

    // 准备读取图像文件
    SDL_INIT_INTERFACE(&bgi_iface);
    bgi_iface.size = [](void* userdata) -> Sint64 {
        auto& beatmapset = *static_cast<BeatmapSet*>(userdata);
        return static_cast<Sint64>(beatmapset.bgi_size_);
    };
    bgi_iface.seek = [](void* userdata, Sint64 offset, SDL_IOWhence whence) -> Sint64 {
        auto& beatmapset = *static_cast<BeatmapSet*>(userdata);
        // SDL_Log("Seek %lld in %d, originally at %zu", offset, whence, static_cast<size_t>(beatmapset.ifs_.tellg()));
        Sint64 pos;
        switch (whence) {
        default:
        case SDL_IO_SEEK_SET: {
            pos = beatmapset.bgi_offset_ + offset;
            break;
        }
        case SDL_IO_SEEK_CUR: {
            pos = beatmapset.ifs_.tellg() + offset;
            break;
        }
        case SDL_IO_SEEK_END: {
            pos = beatmapset.bgi_offset_ + beatmapset.bgi_size_ + offset;
            break;
        }
        }
        if (pos < beatmapset.bgi_offset_ || pos > beatmapset.bgi_offset_ + beatmapset.bgi_size_) {
            beatmapset.ifs_.setstate(std::ios::failbit);
        } else {
            beatmapset.ifs_.seekg(pos, std::ios::beg);
        }
        return static_cast<Sint64>(beatmapset.ifs_.tellg()) - beatmapset.bgi_offset_;
    };
    bgi_iface.read = [](void* userdata, void* ptr, size_t size, SDL_IOStatus* status) -> size_t {
        auto& beatmapset = *static_cast<BeatmapSet*>(userdata);
        // SDL_Log("Read %zu bytes from %zu", size, static_cast<size_t>(beatmapset.ifs_.tellg()));
        if (beatmapset.ifs_.fail()) {
            if (status) *status = SDL_IO_STATUS_ERROR;
        }
        beatmapset.ifs_.read(static_cast<char*>(ptr), size);
        size_t read_bytes = beatmapset.ifs_.gcount();
        if (read_bytes < size) {
            if (status) *status = SDL_IO_STATUS_EOF;
        }
        return read_bytes;
    };
    bgi_iface.write = nullptr;  // 只读
    bgi_iface.flush = [](void*, SDL_IOStatus*) -> bool { return true; };
    bgi_iface.close = [](void* userdata) -> bool {
        auto& beatmapset = *static_cast<BeatmapSet*>(userdata);
        if (beatmapset.ifs_.eof()) {
            beatmapset.ifs_.clear();
        }
        return true;
    };
}