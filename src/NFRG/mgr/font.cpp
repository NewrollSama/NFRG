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
#include "NFRG/mgr/font.hpp"
using namespace nfrg;
using namespace mgr;

#include <array>

std::unordered_map<float, TTF_Font*> Font::fonts;
TTF_TextEngine* Font::text_engine = nullptr;

void Font::open_font() {
    std::array<char, 1024> font_path;
    SDL_snprintf(font_path.data(), font_path.size(),
        "%s/fonts/SarasaGothicSC-Regular.ttf", SDL_GetBasePath());
    TTF_Font* font = TTF_OpenFont(font_path.data(), 48.F);
    fonts.insert({ 48.F, font });
    if (!TTF_SetFontLanguage(font, "zh-Hans-CN")) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Language not set");
    }
    text_engine = TTF_CreateRendererTextEngine(mgr::Video::renderer);
}

auto Font::create_texture(std::string& content, float ptsize, SDL_Color color) -> SDL_Texture* {
    TTF_Font* font = get_font(ptsize);
    SDL_Surface* surface = TTF_RenderText_Blended(font, content.c_str(), content.size(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(mgr::Video::renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}

auto Font::create_text(std::string_view content, float ptsize) -> TTF_Text* {
    TTF_Font* font = get_font(ptsize);
    TTF_Text* text = TTF_CreateText(text_engine, font, content.data(), content.size());
    if (text == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
    }
    return text;
}

auto Font::get_font(float ptsize) -> TTF_Font* {
    if (!fonts.contains(ptsize)) {
        TTF_Font* font = TTF_CopyFont(fonts[48.F]);
        TTF_SetFontSize(font, ptsize);
        fonts.insert({ ptsize, font });
        return font;
    }
    return fonts[ptsize];
}

void Font::close_font() {
    TTF_DestroyRendererTextEngine(text_engine);
    for (auto [_, font] : fonts) {
        TTF_CloseFont(font);
    }
}
