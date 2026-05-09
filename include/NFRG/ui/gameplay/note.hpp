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
#include "../builder/component.hpp"

namespace nfrg::ui::gameplay {

class Lane;

class Note : public Component {
    friend class Base::BuilderTemplate<Note>;

protected:
    /**
     * @brief 该音符的预期按下时间。
     */
    Uint64 hit_time_ = 0;
    /**
     * @brief 该音符的预期松开时间。
     */
    Uint64 release_time_ = 0;

    /**
     * @brief 音符长条部分。为空则代表该音符不是长条。
     */
    std::shared_ptr<Lane> lane_;

    Note() = default;

public:
    class Builder;

    /**
     * @brief 获取该音符是否有长条。
     *
     * @return 该音符是否有长条。
     */
    [[nodiscard]] auto has_lane() const -> bool;
    /**
     * @brief 获取该音符的预期按下时间。
     *
     * @return 该音符的预期按下时间。
     */
    [[nodiscard]] auto get_hit_time() const -> Uint64;
    /**
     * @brief 获取该音符的预期松开时间。
     *
     * @return 该音符的预期松开时间。
     */
    [[nodiscard]] auto get_release_time() const -> Uint64;

    [[nodiscard]] auto render() const -> bool override;
};

class Note::Builder : public Component::BuilderTemplate<Note> {
public:
    Builder() = default;
    auto set_hit_time(Uint64 hit_time) -> Builder&;
    auto set_release_time(Uint64 release_time) -> Builder&;
    auto set_width(float width) -> Builder&;
    auto set_lane(float height) -> Builder&;
};
}
