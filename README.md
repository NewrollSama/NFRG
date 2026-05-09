# Note-Falling Rhythm Game

一个朴素的下落式音游，基于 [SDL3](https://libsdl.org/) 多媒体框架，使用 C++ 编写。

## 开放源代码许可
本项目基于 GNU 通用公共许可证 v3.0 开源，详情请参考 [COPYING](COPYING) 或者 [https://www.gnu.org/licenses/](https://www.gnu.org/licenses/)。

本项目使用了以下第三方库，它们的许可证分别是：
- SDL3：zlib
- SDL3_image：zlib
- SDL3_ttf：zlib
- libsndfile：GNU Lesser General Public License, version 2.1 and version 3
- （仅 Linux）openssl：Apache, Version 2.0

本项目使用的字体为精简版的更纱黑体，原字体基于 SIL Open Font License, Version 1.1 发布，精简版来源于 [https://github.com/ZaneL1u/TTF-Min](https://github.com/ZaneL1u/TTF-Min)。

## 构建
### Windows
使用 Visual Studio 时，确保安装了「用于 Windows 的 C++ CMake 工具」，将此项目以文件夹打开即可选择配置并运行。

或者，尝试使用以下命令手动编译并运行：
```shell
# 使用 MSVC 编译器
cmake --preset windows-msvc-release
cmake --build --preset windows-msvc-build
out/build/windows-msvc-release/src/NFRG/NFRG.exe
```

```shell
# 使用 Clang 编译器
cmake --preset windows-clang-release
cmake --build --preset windows-clang-build
out/build/windows-clang-release/src/NFRG/NFRG.exe
```

若使用 ARM 架构的设备，需要自行修改 [CMakePresets.json](CMakePresets.json) 创建对应的预设。

### Linux
以下流程仅适用于 x86_64 架构设备。

确保系统安装了以下包，如果没有，请使用发行版对应的包管理器获取这些包：
```shell
sdl3 sdl3_image sdl3_ttf libsndfile openssl
```

根据使用的编译器选择以下命令之一：
```shell
cmake --workflow appimage-clang
# 或
cmake --workflow appimage-gcc
```

将自动完成编译和打包，输出的 AppImage 包位于 `out/NFRG.AppImage`。

## 实用工具
为了简化谱面制作流程，使用 AI 编写了两个工具，部署于 GitHub Pages，可按需使用。

### 谱面编辑器
https://newrollsama.github.io/misc/Beatmap_Editor_patched.html

### 谱面集打包
https://newrollsama.github.io/misc/nfrg_packer.html
