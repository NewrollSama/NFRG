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
#include "NFRG/ui/splash/icon.hpp"
using namespace nfrg;
using namespace ui;
using namespace splash;

#include "NFRG/anim/alpha.hpp"
#include "NFRG/ui/static_text.hpp"

#include <algorithm>
#include <array>

void Icon::trigger_appear(int index) {
    auto& a = anims_[index];
    a.active = true;
    a.current_y = 0.0f;
    auto component = std::dynamic_pointer_cast<Component>(components_[index]);
    component->set_alpha(0);
    anim::Alpha::Builder()
        .set_source(0)
        .set_destination(255)
        .set_duration(fade_duration_)
        .bind_component(std::weak_ptr<ui::Component>(component))
        .build_and_add_to(scene_)
        ->start();
}

void Icon::update(Uint64 delta) {
    if (phase_ == Phase::Falling) {
        bool all_disappeared = true;
        for (size_t i = 0; i < 4; ++i) {
            auto& a = anims_[i];
            if (a.disappeared) continue;

            float next = a.current_y + fall_speed_ * delta / 1'000'000'000;
            if (next + a.text_h / 2.0f >= bound_.h) {
                a.disappeared = true;
                std::dynamic_pointer_cast<Component>(components_[i])->set_alpha(0);
            } else {
                a.current_y = next;
                all_disappeared = false;
            }

            auto text = std::static_pointer_cast<StaticText>(components_[i]);
            auto pos = text->get_pos();
            text->set_pos({ .x = pos.x, .y = a.current_y });
        }

        if (all_disappeared) {
            for (auto& a : anims_) {
                a.current_y = 0.0f;
            }
            float max_home_y = 0.0f;
            for (const auto& a : anims_) {
                if (a.home_y > max_home_y) max_home_y = a.home_y;
            }
            float target_arrival = max_home_y / fall_speed_;
            for (size_t i = 0; i < 4; ++i) {
                int idx = appear_order_[i];
                anims_[idx].start_time = target_arrival - anims_[idx].home_y / fall_speed_;
            }
            phase_ = Phase::Appearing;
            appear_timer_ = 0.0f;
        }
        return;
    } else if (phase_ == Phase::Appearing) {
        appear_timer_ += static_cast<float>(delta) / 1'000'000'000;

        for (size_t i = 0; i < 4; ++i) {
            int idx = appear_order_[i];
            auto& a = anims_[idx];
            if (!a.active && appear_timer_ >= a.start_time) {
                trigger_appear(idx);
            }
        }

        for (size_t i = 0; i < 4; ++i) {
            auto& a = anims_[i];
            if (!a.active || a.settled) continue;

            float next = a.current_y + fall_speed_ * delta / 1'000'000'000;
            if (next >= a.home_y) {
                a.current_y = a.home_y;
                a.settled = true;
            } else {
                a.current_y = next;
            }

            auto text = std::static_pointer_cast<StaticText>(components_[i]);
            auto pos = text->get_pos();
            text->set_pos({ .x = pos.x, .y = a.current_y });
        }

        if (all_settled()) phase_ = Phase::Done;
    }
}

auto Icon::all_settled() const -> bool {
    for (const auto& a : anims_) {
        if (!a.settled) return false;
    }
    return true;
}

auto Icon::render() const -> bool {
    SDL_RenderTexture(mgr::Video::renderer, texture_, nullptr, &bound_);
    for (const auto& component : components_) {
        if (!component->render()) return false;
    }
    return true;
}

auto Icon::Builder::set_fall_speed(float pixels_per_second) -> Icon::Builder& {
    component_->fall_speed_ = pixels_per_second;
    return *this;
}

auto Icon::Builder::set_fade_duration(float milliseconds) -> Icon::Builder& {
    component_->fade_duration_ = milliseconds;
    return *this;
}

auto Icon::Builder::build() -> std::shared_ptr<Icon> {
    const float W = component_->bound_.w / 4, H = component_->bound_.h / 4;
    component_->texture_ = SDL_CreateTexture(mgr::Video::renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        static_cast<int>(component_->bound_.w), static_cast<int>(component_->bound_.h));
    SDL_SetRenderTarget(mgr::Video::renderer, component_->texture_);
    SDL_SetRenderDrawColor(mgr::Video::renderer, 255, 255, 255, 255);
    SDL_FRect rect{ .x = 0, .y = 0, .w = component_->bound_.w, .h = component_->bound_.h };
    SDL_RenderRect(mgr::Video::renderer, &rect);
    for (float x = W; x < component_->bound_.w; x += W) {
        SDL_RenderLine(mgr::Video::renderer, x, 0, x, component_->bound_.h);
    }
    SDL_SetRenderTarget(mgr::Video::renderer, nullptr);

    struct Iter {
        std::string c;
        float x;
        float y;
    };
    std::array<Iter, 4> iters = {
        Iter{ .c = "N", .x = 0, .y = H * 2 },
        Iter{ .c = "F", .x = W, .y = 0 },
        Iter{ .c = "R", .x = W * 2, .y = H * 3 },
        Iter{ .c = "G", .x = W * 3, .y = H }
    };
    for (size_t i = 0; i < iters.size(); ++i) {
        const auto& iter = iters[i];
        auto text = StaticText::Builder()
                        .set_content(iter.c.c_str(), component_->bound_.h / 8)
                        .set_pos({ .x = iter.x, .y = iter.y })
                        .build_and_add_to(component_.get());
        const auto [w, h] = text->get_size();
        auto pos = text->get_pos();
        pos.x += (W - w) / 2;
        pos.y += (H - h) / 2;
        text->set_pos(pos);
        component_->anims_[i] = { .home_y = pos.y, .text_h = h, .current_y = pos.y, .disappeared = false, .active = false, .settled = false, .start_time = 0.0f };
        component_->appear_order_[i] = static_cast<int>(i);
    }
    std::ranges::sort(component_->appear_order_, {}, [this](int idx) -> float { return component_->anims_[idx].home_y; });
    std::ranges::reverse(component_->appear_order_);
    return std::move(component_);
}

auto Icon::Builder::build_and_add_to(scene::Scene* scene) -> std::shared_ptr<Icon> {
    component_->scene_ = scene;
    return BuilderTemplate<Icon>::build_and_add_to(scene);
}
