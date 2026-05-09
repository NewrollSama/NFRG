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
#include "NFRG/ui/image.hpp"
using namespace nfrg;
using namespace ui;

auto Image::load(std::string&& filename) -> bool {
    set_texture(IMG_LoadTexture(mgr::Video::renderer, filename.c_str()), false);
    return texture_ != nullptr;
}

auto Image::load(SDL_IOStream* io) -> bool {
    set_texture(IMG_LoadTexture_IO(mgr::Video::renderer, io, false), false);
    return texture_ != nullptr;
}

auto Image::render() const -> bool {
    SDL_FRect src;
    if (!SDL_GetTextureSize(texture_, &src.w, &src.h)) {
        SDL_Log("From Image::render() ...");
        return false;
    }

    float image_ratio = src.w / src.h, bound_ratio = bound_.w / bound_.h;
    if (image_ratio > bound_ratio) {
        float w = src.w;
        src.w = src.h * bound_ratio;
        src.x = (w - src.w) / 2;
        src.y = 0;
    } else if (image_ratio < bound_ratio) {
        float h = src.h;
        src.h = src.w / bound_ratio;
        src.x = 0;
        src.y = (h - src.h) / 2;
    } else {
        src.x = 0;
        src.y = 0;
    }

    SDL_RenderTexture(mgr::Video::renderer, texture_, &src, &bound_);
    return true;
    // return Component::render();
}

auto Image::Builder::load(std::string&& filename) -> Image::Builder& {
    if (!component_->load(std::move(filename))) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "图像加载失败");
    }
    return *this;
}

auto Image::Builder::load(SDL_IOStream* io) -> Image::Builder& {
    if (!component_->load(io)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "图像加载失败");
    }
    return *this;
}
