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
#ifdef WIN32
#include "windows.h"

#include <iomanip>
#include <sstream>

using namespace nfrg;
using namespace platform;

auto platform::utf8_to_ansi(std::string& utf8_string) -> std::string {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0,
        utf8_string.c_str(), static_cast<int>(utf8_string.size()),
        nullptr, 0);
    std::wstring wdir(size_needed, '\0');
    MultiByteToWideChar(CP_UTF8, 0,
        utf8_string.c_str(), static_cast<int>(utf8_string.size()),
        &wdir[0], size_needed);
    size_needed = WideCharToMultiByte(CP_ACP, 0,
        wdir.c_str(), static_cast<int>(wdir.size()),
        nullptr, 0, nullptr, nullptr);
    std::string ansi_string(size_needed, '\0');
    WideCharToMultiByte(CP_ACP, 0,
        wdir.c_str(), (int)wdir.size(),
        &ansi_string[0], size_needed, nullptr, nullptr);
    return ansi_string;
}

class SHA256Calculator::Impl {
private:
    HCRYPTPROV hProv = NULL;
    HCRYPTHASH hHash = NULL;

public:
    Impl() {
        if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d", GetLastError());
            return;
        } else if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d", GetLastError());
            CryptReleaseContext(hProv, 0);
            return;
        }
    };

    ~Impl() {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
    }

    void update(const void* data, size_t size) {
        if (!CryptHashData(hHash, static_cast<const BYTE*>(data), static_cast<DWORD>(size), 0)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d", GetLastError());
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            return;
        }
    }

    auto finalize() -> std::string {
        std::array<BYTE, 32> hash;
        DWORD size = static_cast<DWORD>(hash.size());

        if (!CryptGetHashParam(hHash, HP_HASHVAL, hash.data(), &size, 0)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d", GetLastError());
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
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
