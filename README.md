# README.md - 20170326 - 20141214

Windows does NOT have a dirent.h header.

This is a small windows port of dirent.h in direct.c into a simple 
static dirent.lib.

Build is using cmake to generate native build files.

A convenient build-me.bat is in the 'build' folder to build the 
32-bit library. It will be installed into X:\3rdParty.

And if you can build Win64 64-bit, then there is a convenient build-me.bat 
in the 'build.x64' folder to build the 64-bit library. It will be 
installed in X:\3rdParty.x64

And if you need to 'find' dirent.h and dirent.lib then there is a simple
FindDirent.cmake in the CMake folder to add to your project to find and 
using this small static library and header.

Have fun.
Geoff McLane
email: reports _AT_ geoffair _DOT_ info

; eof
