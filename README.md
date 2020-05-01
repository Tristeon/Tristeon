# Tristeon2D
Tristeon2D is an open-source game engine built for 2D tile-based platformers.

![logo](https://i.imgur.com/0nUq7XL.png)

## Getting started
The project can be build using CMake. Currently we only support visual studio 2019.
It might be possible that other builds work but are not tested.
When building for visual studio 2019 you will get the following error "Error in configuration process, project files may be invalid"
This is due to the "Qt5_DIR" not being set. This should reference your Qt directory. More specifically it needs to redirect to
`Qt/5.14.2/msvc2017_64/lib/cmake/Qt5`

## Design
Our engine is designed to benefit a variety of different users, no matter if you prefer LUA, C#, C++, OOP design or a behaviour approach, we've got it all. Our current engine architecture looks like this:
![engine uml](https://i.imgur.com/wocghCz.png)

## Features
(WIP)

## Getting started

## Releases
//TODO: Release this game engine 
