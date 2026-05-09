cmake_minimum_required(VERSION 3.21)

project("SDL3-NFRG" LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option("USE_ASAN" "Compile with Address Sanitizer" OFF)

# 引入头文件
include_directories("include")

# 引入第三方库
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
# 引入 SDL3、SDL3_ttf
if (WIN32)  # 在 Windows 上使用预编译的 SDL3 库
	set(SDL3_DIR "${LIB_DIR}/vc/SDL3-3.4.8/cmake")
	set(SDL3_image_DIR  "${LIB_DIR}/vc/SDL3_image-3.4.4/cmake")
	set(SDL3_ttf_DIR "${LIB_DIR}/vc/SDL3_ttf-3.2.2/cmake")
endif()
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(SDL3_image REQUIRED CONFIG REQUIRED COMPONENTS SDL3_image-shared)
find_package(SDL3_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL3_ttf-shared)
# 引入音频库
if (WIN32)  # 在 Windows 上使用预编译的 libsndfile 库
    set(SndFile_DIR "${LIB_DIR}/vc/libsndfile-1.2.2-win64/cmake")
endif()
find_package(SndFile REQUIRED)

# 包含子文件夹
add_subdirectory("src/NFRG")
