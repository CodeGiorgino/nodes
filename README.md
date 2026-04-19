# nodes
Todo program to make node graphs and keep track of jobs

Written in Cpp23 with the usage of [raylib](https://github.com/raysan5/raylib)
to draw the UI.

## Compilation

> [!IMPORTANT]
> Requirements:
> - [CMake](https://cmake.org/)

```bash
git clone https://github.com/codegiorgino/nodes
cd nodes

cmake . -B build \
    &&  cmake --build build --parallel $(($(nproc) - 1)) \
    && ./build/nodes
```
