Calculadora simples em C com GTK4.

Dependências
- Compilador C (GCC ou Clang)
- CMake 3.16+
- pkg-config
- GTK4
- Instalação das dependências

> **Debian/Ubuntu:**
> ```bash
> sudo apt install build-essential cmake pkg-config libgtk-4-dev
> ```

> **Windows 11 (terminal MSYS2 UCRT64):**
> ```bash
> pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-pkgconf mingw-w64-ucrt-x86_64-gtk4
> ```

how to compile:
> ```bash
> cmake -B build
> cmake --build build
> ./build/calc
> ```
