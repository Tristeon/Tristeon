# Tristeon2D
Tristeon2D is an open-source game engine built for 2D tile-based platformers.

![logo](https://i.imgur.com/0nUq7XL.png)

## Getting started
We aim to simplify building Tristeon as much as we possibly can. This process is stil a work in progress so please bear with us for the time being.

### Supported programming environments:
- MSVC 2019 | x64

### Building the project
Tristeon uses CMake as its building software. To be able to build Tristeon, please make sure that you've got CMake 3.7 or higher installed [https://cmake.org/download/].

Tristeon's editor uses the Qt5 framework for editor tooling. Our tests are currently done with Qt 5.14.1, but other versions should work fine. Please make sure you have Qt5 installed [https://www.qt.io/download-qt-installer].

Steps:
- Use CMake to build Tristeon's source code using the CMakeLists.txt file
- Fully build the solution. Trying to run the application at this point will result in missing dll errors.
- Run CMake a second time. CMake will detect the built .exe file and will use it to import Qt dlls. 
After this, the engine should be ready to run.

## Features
(WIP)

## Releases
//TODO: Release this game engine 
