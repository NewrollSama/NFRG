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
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "NFRG/mgr/audio.hpp"
#include "NFRG/mgr/font.hpp"
#include "NFRG/mgr/scene.hpp"
#include "NFRG/scene/impl/splash.hpp"

using namespace nfrg;

auto SDL_AppInit(
    [[maybe_unused]] void** appstate,
    [[maybe_unused]] int argc,
    [[maybe_unused]] char** argv) -> SDL_AppResult {
    SDL_SetHint(SDL_HINT_APP_NAME, "NFRG");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) || !TTF_Init()) {
        return SDL_APP_FAILURE;
    }
    bool r = SDL_CreateWindowAndRenderer(
        "Note-Falling Rhythm Game", mgr::Video::width, mgr::Video::height,
        SDL_WINDOW_HIDDEN, &mgr::Video::window, &mgr::Video::renderer);
    if (!r) {
        return SDL_APP_FAILURE;
    }

    mgr::Font::open_font();
    if (!mgr::Audio::open_audio()) {
        return SDL_APP_FAILURE;
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Renderer name: %s", SDL_GetRendererName(mgr::Video::renderer));
    // SDL_SetRenderLogicalPresentation(renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_STRETCH);

    mgr::Scene::create<scene::SplashScene>();

    SDL_ShowWindow(mgr::Video::window);

    *appstate = new Uint64{ SDL_GetTicksNS() };

    return SDL_APP_CONTINUE;
}

auto SDL_AppIterate([[maybe_unused]] void* appstate) -> SDL_AppResult {
    auto& last_ticks_ns = *static_cast<Uint64*>(appstate);
    auto now_ns = SDL_GetTicksNS();
    auto delta_ns = now_ns - last_ticks_ns;
    last_ticks_ns = now_ns;

    mgr::Scene::ActiveScene::process_animators(delta_ns);
    mgr::Scene::ActiveScene::update();

    mgr::Scene::ActiveScene::measure();

    SDL_SetRenderDrawColor(mgr::Video::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(mgr::Video::renderer);

    if (!mgr::Scene::ActiveScene::render()) {
        return SDL_APP_FAILURE;
    }

    SDL_RenderPresent(mgr::Video::renderer);

    return SDL_APP_CONTINUE;
}

auto SDL_AppEvent([[maybe_unused]] void* appstate, SDL_Event* event) -> SDL_AppResult {
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
        break;
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        mgr::Scene::ActiveScene::handle_key_event(event->key);
        break;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit([[maybe_unused]] void* appstate, SDL_AppResult result) {
    delete static_cast<Uint64*>(appstate);

    mgr::Scene::cleanup();
    mgr::Audio::close_audio();
    mgr::Font::close_font();

    if (result == SDL_APP_FAILURE) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
    }

    if (mgr::Video::renderer != nullptr) {
        SDL_DestroyRenderer(mgr::Video::renderer);
    }
    if (mgr::Video::window != nullptr) {
        SDL_DestroyWindow(mgr::Video::window);
    }

    TTF_Quit();
    SDL_Quit();
}
