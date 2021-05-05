# compiletime-string-to-bytearray
For some reason you want to use strings for byte arrays? Well, here is the most performant way to do it.

# How does this look decompiled?
<p align="center">
    <img width="780" height="240" src="resources/in_ide.png">
</p>
<p align="center">
    <img width="287" height="431" src="resources/disassembled.png">
</p>

# Compilers
Tested and works with:
- MSVC (std:c++latest)
- LLVM Clang ((MS)std:c++latest)
- gcc version 9.3.0 (Ubuntu 9.3.0-17ubuntu1~20.04) (-std=c++2a)
