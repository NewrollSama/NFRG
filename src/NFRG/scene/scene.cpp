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
#include "NFRG/scene/scene.hpp"
using namespace nfrg;
using namespace scene;

#include "NFRG/mgr/audio.hpp"
#include "NFRG/mgr/scene.hpp"

void Scene::add_animator(std::unique_ptr<anim::Animator>&& animator) {
    animators_.push_front(std::move(animator));
}

void Scene::process_animators(Uint64 delta_ns) {
    auto prev_it = animators_.cbefore_begin();
    for (auto it = animators_.cbegin(); it != animators_.cend(); ++it) {
        auto* cur_anim = it->get();
        if (cur_anim->process(delta_ns)) {  // 动画已经结束
            if (prev_it == animators_.cbefore_begin()) {
                animators_.pop_front();
                it = animators_.before_begin();
            } else {
                it = prev_it;
                animators_.erase_after(it);
            }
        } else {
            prev_it = it;
        }
    }
}

void Scene::handle_key_event(SDL_KeyboardEvent& event) {
    switch (event.key) {
    case SDLK_ESCAPE:
        mgr::Scene::ActiveScene::quit();
        break;
    }
}

void Scene::measure() {
    for (const auto& component : components_) {
        component->measure({ .x = 0.F, .y = 0.F });
    }
}

auto Scene::render() const -> bool {
    for (const auto& component : components_) {
        if (!component->render()) {
            return false;
        }
    }
    return true;
}

Scene::~Scene() {
    mgr::Audio::clear_bg();
    mgr::Audio::clear_se();
}
