# 🎮 TrollFootball

TrollFootball là trò chơi bóng đá 2D đối kháng được phát triển bằng C++ và SFML.
Game hỗ trợ hai chế độ:
- Player vs AI
- Player vs Player

## 👥 Thành viên nhóm

| Thành viên | Phụ trách |
|---|---|
| Trương Minh Quân | Vật lý game, AI, viết báo cáo |
| Nguyễn Thanh Danh | Assets, Animation, Edit video |
| Nguyễn Phú Nguyên Vũ | UI, Audio, Quay video giới thiệu game |

## 🛠️ Công nghệ sử dụng

- **C++17**
- **SFML 3.1.0**
  - SFML Graphics
  - SFML Window
  - SFML System
  - SFML Audio
- **CMake 3.28 trở lên**
- **Visual Studio 2022** hoặc compiler tương thích C++17

## 📦 Yêu cầu cài đặt

Trước khi chạy project, cần cài:

1. **Visual Studio 2022**
   - Cài workload **Desktop development with C++**
   - Bao gồm MSVC C++ compiler và Windows SDK.

2. **CMake 3.28 trở lên**

3. **SFML 3.1.0**

> Lưu ý: Project được phát triển và kiểm thử với SFML 3.1.0.

## 📁 Cấu trúc project

```text
TrollFootball/
├── assets/
│   ├── textures/
│   ├── audio/
│   └── ...
├── include/
│   ├── ai/
│   ├── core/
│   ├── entities/
│   └── ...
├── src/
│   ├── ai/
│   ├── core/
│   ├── entities/
│   └── main.cpp
├── CMakeLists.txt
├── CMakePresets.json
└── README.md
