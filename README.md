# QR-Code generator with C++
(Project made by newbie)
This is the basic way to create QR-Code using cpp-httplib, libpng, zlib. Thank you very much. Without them, my project would not have happened.

# How to use
Run program
```bash
Input Your Link: https://www.example.example
Input Your Port: 8080
```

# Build
Windows
```bash
git clone --recursive https://github.com/exsycore/qr-gen-cpp.git
cd qr-gen-cpp
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release or Debug
```
