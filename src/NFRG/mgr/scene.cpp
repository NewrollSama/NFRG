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
#include "NFRG/mgr/scene.hpp"
using namespace nfrg;
using namespace mgr;

#include "NFRG/scene/scene.hpp"

std::stack<std::unique_ptr<scene::Scene>> Scene::scenes;

auto Scene::ActiveScene::get() -> scene::Scene* {
    return scenes.top().get();
}

void Scene::ActiveScene::process_animators(Uint64 delta_ns) {
    scenes.top()->process_animators(delta_ns);
}

void Scene::ActiveScene::update() {
    scenes.top()->update();
}

void Scene::ActiveScene::measure() {
    scenes.top()->measure();
}

auto Scene::ActiveScene::render() -> bool {
    return scenes.top()->render();
}

void Scene::ActiveScene::handle_key_event(SDL_KeyboardEvent& event) {
    scenes.top()->handle_key_event(event);
}

void Scene::ActiveScene::quit() {
    scenes.pop();
}

void Scene::cleanup() {
    while (!scenes.empty()) {
        scenes.pop();
    }
}
