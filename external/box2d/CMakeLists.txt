cmake_minimum_required(VERSION 3.8)

project(box2d VERSION 2.4.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)

if (BOX2D_BUILD_DOCS)
    set(DOXYGEN_SKIP_DOT TRUE)
    find_package(Doxygen)
endif()

if (DOXYGEN_FOUND AND BOX2D_BUILD_DOCS)
    add_subdirectory(docs)
endif()