cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(Tristeon LANGUAGES CXX)

set (Tristeon_VERSION_MAJOR 1)
set (Tristeon_VERSION_MINOR 1)	
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Tristeon build options
OPTION(TRISTEON_BUILD_EDITOR ON)
SET(TRISTEON_BUILD_TESTS OFF CACHE BOOL "")
SET(TRISTEON_BUILD_EXE ON CACHE BOOL "")

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

set(CMAKE_CONFIGURATION_TYPES Debug Release)
add_definitions(-DTRISTEON_LOGENABLED)

#Check if submodules are downloaded
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/box2d/CMakeLists.txt")
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

if (UNIX)
	find_package(X11 REQUIRED)
	include_directories(${X11_X11_INCLUDE_PATH})

	# Check for XRandR (modern resolution switching and gamma control)
	if (NOT X11_Xrandr_INCLUDE_PATH)
		message(FATAL_ERROR "RandR headers not found; install libxrandr development package")
	endif()
	message(STATUS "Found X11")
endif()

#Include threading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

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

#Run build/editor specific cmake commands
IF (TRISTEON_BUILD_EDITOR)
    include("EditorCMakeLists.cmake")
ELSE()
    include("BuildCMakeLists.cmake")
ENDIF()

if (MSVC)
	#Set target output directory
	if (TRISTEON_BUILD_EXE)
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

		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Tristeon)
	endif()

    target_compile_options(box2d PUBLIC /MP)
    target_compile_options(glad PUBLIC /MP)
    target_compile_options(OpenAL PUBLIC /MP)

    add_definitions(/MP)
endif(MSVC)

#Link libraries
if (TRISTEON_BUILD_EXE)
	target_link_libraries(Tristeon PRIVATE glad)
	target_link_libraries(Tristeon PRIVATE magic_enum)
	target_link_libraries(Tristeon PRIVATE OpenAL)
	target_link_libraries(Tristeon PRIVATE box2d)
	target_link_libraries(Tristeon PRIVATE Threads::Threads)
	target_link_libraries(Tristeon PRIVATE ${CMAKE_DL_LIBS})

	target_link_libraries(Tristeon PRIVATE "$<$<CXX_COMPILER_ID:GNU>:-pthread>$<$<CXX_COMPILER_ID:Clang>:-pthreads>")

	if (UNIX)
		target_link_libraries(Tristeon PRIVATE X11)
		target_link_libraries(Tristeon PRIVATE Xrandr)
	endif()
endif()

if(TRISTEON_BUILD_TESTS)
	target_link_libraries(Tristeon_Lib PRIVATE glad)
	target_link_libraries(Tristeon_Lib PRIVATE magic_enum)
	target_link_libraries(Tristeon_Lib PRIVATE OpenAL)
	target_link_libraries(Tristeon_Lib PRIVATE box2d)
	target_link_libraries(Tristeon_Lib PRIVATE Threads::Threads)
	target_link_libraries(Tristeon_Lib PRIVATE ${CMAKE_DL_LIBS})

	target_link_libraries(Tristeon_Lib PRIVATE "$<$<CXX_COMPILER_ID:GNU>:-pthread>$<$<CXX_COMPILER_ID:Clang>:-pthreads>")
	
	if (UNIX)
		target_link_libraries(Tristeon_Lib PRIVATE X11)
		target_link_libraries(Tristeon_Lib PRIVATE Xrandr)
	endif()

	#link tristeon lib to test build
	add_subdirectory(tests)
	add_dependencies(Tristeon_Test Tristeon_Lib)

	if (MSVC)
		if (TRISTEON_BUILD_TESTS AND NOT TRISTEON_BUILD_EXE)
			set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Tristeon_Test)
		endif()
	endif()
endif()