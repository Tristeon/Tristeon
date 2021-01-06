set (Tristeon_VERSION_MAJOR 1)
set (Tristeon_VERSION_MINOR 1)	
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#CMake lookup path for modules (find_package)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Output/compilation
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#Including our own src folder for ease of use
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src")

#Add source files recursively by looking through the folder structure
macro(read_directory curdir)
	file(GLOB children RELATIVE ${curdir} ${curdir}/*)
	
	set(groupList "")
	
	foreach(child ${children})
		if (NOT (IS_DIRECTORY ${curdir}/${child}))
			list(APPEND tristeonSRC ${curdir}/${child})
			list(APPEND groupList ${curdir}/${child})
		endif()
	endforeach()
	
	list(LENGTH groupList length)
	if (length GREATER 0)
		string(REPLACE ${PROJECT_SOURCE_DIR} "" GROUPNAME ${curdir})
		string(REPLACE "/" "\\" GROUPNAME ${GROUPNAME})
		source_group(${GROUPNAME} FILES ${groupList})
	endif()
	
	foreach(child ${children})
		if (IS_DIRECTORY ${curdir}/${child})
			read_directory(${curdir}/${child})
		endif()
	endforeach()
endmacro()
read_directory(${PROJECT_SOURCE_DIR}/src)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
add_definitions(-DTRISTEON_LOGENABLED -DTRISTEON_EDITOR)

#Check if submodules are downloaded
if(	NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt" OR
NOT EXISTS "${PROJECT_SOURCE_DIR}/external/box2d/CMakeLists.txt" OR
NOT EXISTS "${PROJECT_SOURCE_DIR}/external/magic_enum/CMakeLists.txt" OR
NOT EXISTS "${PROJECT_SOURCE_DIR}/external/openal-soft/CMakeLists.txt")
	message(STATUS "The git submodules were not updated, attempting to do so now")
	find_package(Git)
	if (NOT GIT_FOUND)
		message(FATAL_ERROR "Couldn't find Git executable thus the git submodules weren't downloaded. To build Tristeon, you'll need to download git and try again, or run git submodule update --init --recursive yourself.")
	else()
		message(STATUS "Git found, updating submodules")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
						RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update failed with ${GIT_SUBMOD_RESULT}")
		endif()
	endif()
endif()

#External libraries
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "")
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "")
add_subdirectory(external/box2d)

add_subdirectory(external/glad)

add_subdirectory(external/magic_enum)

set(ALSOFT_EXAMPLES OFF CACHE BOOL "")
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "")
set(ALSOFT_UTILS OFF CACHE BOOL "")
add_subdirectory(external/openal-soft)
include_directories(external/openal-soft/include)

set(BUILD_STATIC ON CACHE BOOL "")
set(BUILD_EXAMPLES OFF CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")
set(OPENAL_INCLUDE_DIR "external/openal-soft/include/al/" CACHE STRING "")
set(OPENAL_LIBRARY "OpenAL" CACHE STRING "")

if (UNIX)
	find_package(X11 REQUIRED)

	# Set up library and include paths
	include_directories(${X11_X11_INCLUDE_PATH})

	# Check for XRandR (modern resolution switching and gamma control)
	if (NOT X11_Xrandr_INCLUDE_PATH)
		message(FATAL_ERROR "RandR headers not found; install libxrandr development package")
	endif()
	message(STATUS "Found X11")
endif()

#include Qt
if (NOT DEFINED Qt5_DIR)
	message("Qt5_DIR not set, attempting to find it.")
	include(${CMAKE_MODULE_PATH}/QtLocator.cmake)
else()
	message("Qt5_DIR set, using dir directly.")
endif()

find_package(Qt5 COMPONENTS Core Widgets OpenGL UiTools Qml Gamepad REQUIRED)
set(QT_USE_QTOPENGL TRUE)

#Include threading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#Libraries
macro(link_libs targetname)
	target_link_libraries(${targetname} PRIVATE glad)
	target_link_libraries(${targetname} PRIVATE magic_enum)
	target_link_libraries(${targetname} PRIVATE OpenAL)
	target_link_libraries(${targetname} PRIVATE box2d)
	target_link_libraries(${targetname} PRIVATE Threads::Threads)
	target_link_libraries(${targetname} PRIVATE ${CMAKE_DL_LIBS})

	target_link_libraries(${targetname} PRIVATE Qt5::Widgets)
	target_link_libraries(${targetname} PRIVATE Qt5::UiTools)
	target_link_libraries(${targetname} PRIVATE Qt5::Qml)
	target_link_libraries(${targetname} PRIVATE Qt5::Gamepad)

	target_link_libraries(${targetname} PRIVATE "$<$<CXX_COMPILER_ID:GNU>:-pthread>$<$<CXX_COMPILER_ID:Clang>:-pthreads>")

	if (UNIX)
		target_link_libraries(${targetname} PRIVATE X11)
		target_link_libraries(${targetname} PRIVATE Xrandr)
	endif()
endmacro()

if (MSVC)
	add_definitions(/MP)
	add_executable(Tristeon ${tristeonSRC})
	#Set target output directory
	set_target_properties(Tristeon
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
		LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
		
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
		LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
	)

	#Set working directory to build in MSVC
	file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/Tristeon.vcxproj.user" 
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>     \
		<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
		<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">
			<LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
			<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
		</PropertyGroup>
		<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">
			<LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
			<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
		</PropertyGroup>
		</Project>"
	)
	#Set Tristeon as the start project
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Tristeon)

	link_libs(Tristeon)
	
else(MSVC)
	add_executable(Tristeon ${tristeonSRC})
	link_libs(Tristeon)
endif(MSVC)

#Add Qt binaries to build
execute_process(COMMAND ${QT_PATH}/bin/windeployqt.exe --compiler-runtime --pdb .
				WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
