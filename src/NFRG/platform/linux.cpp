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
#ifdef __linux__

#include <iomanip>
#include <sstream>

#include <openssl/evp.h>

using namespace nfrg;
using namespace platform;

class SHA256Calculator::Impl {
private:
    EVP_MD_CTX* ctx = nullptr;

public:
    Impl() {
        ctx = EVP_MD_CTX_new();
        if (!ctx) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "EVP_MD_CTX_new failed");
            return;
        }
        if (!EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "EVP_DigestInit_ex failed");
            EVP_MD_CTX_free(ctx);
            ctx = nullptr;
            return;
        }
    }

    ~Impl() {
        if (ctx) EVP_MD_CTX_free(ctx);
    }

    void update(const char* data, size_t size) {
        if (!ctx) return;
        if (!EVP_DigestUpdate(ctx, data, size)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "EVP_DigestUpdate failed");
        }
    }

    auto finalize() -> std::string {
        if (!ctx) return "";

        std::array<unsigned char, 32> hash{};
        unsigned int hash_len = 0;

        if (!EVP_DigestFinal_ex(ctx, hash.data(), &hash_len)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "EVP_DigestFinal_ex failed");
            return "";
        }

        std::stringstream result;
        for (auto byte : hash) {
            result << std::hex << std::setw(2) << std::setfill('0')
                   << static_cast<int>(byte);
        }
        return result.str();
    }
};

#endif
