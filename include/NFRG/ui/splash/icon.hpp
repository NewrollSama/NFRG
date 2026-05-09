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
#include "../builder/group.hpp"

#include <array>

namespace nfrg::scene {
class Scene;
}

namespace nfrg::ui::splash {

class Icon : public Group {
    friend class Base::BuilderTemplate<Icon>;

protected:
    SDL_Texture* texture_;
    scene::Scene* scene_;

    float fall_speed_ = 500.0f;
    float fade_duration_ = 100.0f;

    struct LetterAnim {
        float home_y;
        float text_h;
        float current_y;
        bool disappeared;
        bool active;
        bool settled;
        float start_time;
    };
    std::array<LetterAnim, 4> anims_;

    enum class Phase : Uint8 {
        Falling,
        Appearing,
        Done,
    };
    Phase phase_ = Phase::Falling;
    std::array<int, 4> appear_order_;
    float appear_timer_ = 0.0f;

    Icon() = default;

    void trigger_appear(int index);

public:
    class Builder;

    void update(Uint64 delta);
    [[nodiscard]] auto all_settled() const -> bool;
    [[nodiscard]] auto render() const -> bool override;
};

class Icon::Builder : public Group::BuilderTemplate<Icon> {
public:
    Builder() = default;

    auto set_fall_speed(float pixels_per_second) -> Builder&;
    auto set_fade_duration(float milliseconds) -> Builder&;
    auto build() -> std::shared_ptr<Icon> override;
    auto build_and_add_to(scene::Scene* scene) -> std::shared_ptr<Icon> override;
};

}
