cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(Tristeon LANGUAGES CXX)

OPTION(BUILD_EDITOR ON)

IF (BUILD_EDITOR)
    include("EditorCMakeLists.cmake")
ELSE()
    include("BuildCMakeLists.cmake")
ENDIF()