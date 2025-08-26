# STM32F103ZET6 秒表项目

嘉庚学院单片机实验A期末课设作品，基于 STM32F103ZET6 开发的数码管秒表系统，支持基本计时和打点记圈功能。

![STM32](https://img.shields.io/badge/STM32-F103ZET6-blue.svg)
![C](https://img.shields.io/badge/language-C-orange.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## 开发环境

- Windows 10/11
- STM32CubeMX 6.8.1
- STM32Cube FW_F1 V1.8.6
- Visual Studio Code
- CMake 3.22+
- GCC-ARM-None-EABI 工具链
- MinGW-w64

## 硬件资源

- MCU: STM32F103ZET6
- 显示器: 8位数码管
- 按键: KEY0, KEY1, KEY_UP
- LED指示: LED0, LED1

## 功能特性

### 1. 基本计时

- 时间范围：00:00:00 - 99:59:59
- 精度：1秒
- LED指示运行状态

### 2. 按键功能

- KEY0：启动/暂停
- KEY1：复位清零
- KEY_UP：打点记录
- KEY0 + KEY1：切换显示记录
- KEY0 + KEY_UP：清空所有记录

### 3. 打点记圈

- 最大支持10圈记录
- 循环查看记录
- 一键清空功能

## 项目结构

```
Stop_up/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── key.h
│   │   └── seg.h
│   └── Src/
│       ├── main.c
│       ├── key.c
│       └── seg.c
├── cmake/
│   └── gcc-arm-none-eabi.cmake
├── CMakeLists.txt
└── README.md
```

## 编译指南

### 1. CMake 配置

```cmake
cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 11)
set(CMAKE_PROJECT_NAME Stop_up)
include("cmake/gcc-arm-none-eabi.cmake")
```

### 2. 编译步骤

```bash
# 创建构建目录
mkdir build
cd build

# 生成构建文件
cmake -G "MinGW Makefiles" ..

# 编译项目
mingw32-make
```

### 3. 输出文件

- `build/Stop_up.elf`：可执行文件
- `build/Stop_up.hex`：烧录文件
- `build/Stop_up.bin`：二进制文件

## 使用说明

1. **基本操作**
   - 按 KEY0 启动/暂停计时
   - 按 KEY1 复位计时器
   - LED0 指示运行状态

2. **打点记录**
   - 运行时按 KEY_UP 记录当前时间
   - 最多可记录10个时间点
   - 同时按 KEY0+KEY1 查看记录
   - 同时按 KEY0+KEY_UP 清空记录

## 注意事项

1. 确保固件版本为 V1.8.6
2. 编译前检查工具链配置
3. 需正确设置环境变量
4. 打点记录上限为10个

## 作者信息

- **学校**：嘉庚学院
- **课程**：单片机实验A
- **作者**：朱凯雨
- **学号**：EMC23048
- **邮箱**：lifelife919@qq.com

## 许可证

本项目采用 MIT 许可证开源。

## 致谢

- 感谢指导老师的指导与帮助。
- 感谢B站up主keysking
- 感谢vscode,用vscode 写代码心情都会变好呢
- 这里用到的cmake教程和Vscode 配置 Cmake编写STM32都是从up主那里学来的

---

*注：本项目为课程设计作品，仅供学习参考。*
