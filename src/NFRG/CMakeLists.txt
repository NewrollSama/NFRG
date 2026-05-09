add_executable(NFRG WIN32 "main.cpp"
    "anim/alpha.cpp"
    "anim/animator.cpp"
    "anim/motion.cpp"
    "anim/note_motion.cpp"
    "mgr/audio.cpp"
    "mgr/config.cpp"
    "mgr/font.cpp"
    "mgr/scene.cpp"
    "mgr/video.cpp"
    "platform/platform.cpp"
    "scene/impl/gameplay.cpp"
    "scene/impl/latency.cpp"
    "scene/impl/result.cpp"
    "scene/impl/select.cpp"
    "scene/impl/settings.cpp"
    "scene/impl/splash.cpp"
    "scene/scene.cpp"
    "ui/base.cpp"
    "ui/button.cpp"
    "ui/component.cpp"
    "ui/flex.cpp"
    "ui/gameplay/board.cpp"
    "ui/gameplay/lane.cpp"
    "ui/gameplay/note.cpp"
    "ui/group.cpp"
    "ui/image.cpp"
    "ui/list.cpp"
    "ui/latency/detection_line.cpp"
    "ui/select/beatmap_selector.cpp"
    "ui/select/beatmap_set.cpp"
    "ui/splash/icon.cpp"
    "ui/static_text.cpp"
    "ui/text.cpp"
    "util/beatmap_set.cpp"
)

if (${USE_ASAN} AND (${CMAKE_BUILD_TYPE} STREQUAL "Debug"))
    target_compile_options(NFRG PRIVATE -fsanitize=address)
    target_link_libraries(NFRG PRIVATE -fsanitize=address)
endif()

target_link_libraries(NFRG PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SndFile::sndfile
    $<$<PLATFORM_ID:Linux>:OpenSSL::SSL>
)
target_precompile_headers(NFRG PRIVATE
    <SDL3/SDL.h>
    <SDL3_image/SDL_image.h>
    <SDL3_ttf/SDL_ttf.h>
    <array>
    <memory>
    <string>
    <vector>
    "${CMAKE_SOURCE_DIR}/include/NFRG/mgr/config.hpp"
    "${CMAKE_SOURCE_DIR}/include/NFRG/mgr/video.hpp"
)

# 启用所有警告
if (MSVC)
    target_compile_options(NFRG PRIVATE /W4)
else()
    target_compile_options(NFRG PRIVATE -Wall -Wextra)
endif()

if (MSVC)  # 设定各种字符集为 UTF-8
    target_compile_options(NFRG PRIVATE "/source-charset:utf-8")
    target_compile_options(NFRG PRIVATE "/execution-charset:utf-8")
endif()

if (WIN32)  # 在 Windows 平台编译后拷贝动态链接库
    add_custom_command(TARGET NFRG POST_BUILD COMMAND "${CMAKE_COMMAND}" 
        -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:NFRG>
        VERBATIM
    )
    add_custom_command(TARGET NFRG POST_BUILD COMMAND "${CMAKE_COMMAND}" 
        -E copy $<TARGET_FILE:SDL3_image::SDL3_image-shared> $<TARGET_FILE_DIR:NFRG>
        VERBATIM
    )
    add_custom_command(TARGET NFRG POST_BUILD COMMAND "${CMAKE_COMMAND}" 
        -E copy $<TARGET_FILE:SDL3_ttf::SDL3_ttf-shared> $<TARGET_FILE_DIR:NFRG>
        VERBATIM
    )
    add_custom_command(TARGET NFRG POST_BUILD COMMAND "${CMAKE_COMMAND}" 
        -E copy $<TARGET_FILE:SndFile::sndfile> $<TARGET_FILE_DIR:NFRG>
        VERBATIM
    )
endif()

# 拷贝字体文件
add_custom_command(TARGET NFRG POST_BUILD COMMAND "${CMAKE_COMMAND}"
    -E copy_directory "${CMAKE_SOURCE_DIR}/res/fonts" "$<TARGET_FILE_DIR:NFRG>/fonts"
)

# 为 Linux 平台配置 AppImage 打包
if (LINUX)
    install(TARGETS NFRG
        RUNTIME_DEPENDENCY_SET NFRG_deps
        RUNTIME DESTINATION usr/bin
    )
    install(RUNTIME_DEPENDENCY_SET NFRG_deps
        DESTINATION usr/lib
        PRE_EXCLUDE_REGEXES
        ".*/libc\\.so.*"
        ".*/libdl\\.so.*"
        ".*/libpthread\\.so.*"
        ".*/librt\\.so.*"
        ".*/libm\\.so.*"
        ".*/libresolv\\.so.*"
        ".*/libnss_.*"
        ".*/libgcc_s\\.so.*"
        ".*/libstdc\\+\\+\\.so.*"
        POST_EXCLUDE_REGEXES
        ".*/ld-linux.*\\.so.*"
        ".*/libGL\\.so.*"
        ".*/libdrm\\.so.*"
        ".*/libX11\\.so.*"
        ".*/libxcb\\.so.*"
        ".*/nvidia/.*"
        ".*/libcuda\\.so.*"
    )
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/res/fonts DESTINATION usr/bin)
    install(FILES
        ${CMAKE_SOURCE_DIR}/res/NFRG.desktop
        ${CMAKE_SOURCE_DIR}/res/NFRG.png
        DESTINATION .
    )
    install(PROGRAMS ${CMAKE_SOURCE_DIR}/res/AppRun DESTINATION .)
    install(CODE "
        set(ENV{ARCH} x86_64)
        execute_process(COMMAND
            chmod +x ${CMAKE_SOURCE_DIR}/res/appimagetool/appimagetool-x86_64.AppImage
        )
        execute_process(COMMAND
            ${CMAKE_SOURCE_DIR}/res/appimagetool/appimagetool-x86_64.AppImage
            ${CMAKE_SOURCE_DIR}/out/install/linux-clang-release
            ${CMAKE_SOURCE_DIR}/out/NFRG.AppImage
            --runtime-file=${CMAKE_SOURCE_DIR}/res/appimagetool/runtime-x86_64
        )
    ")
endif()
