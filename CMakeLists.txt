cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

#Project
project(Tristeon LANGUAGES CXX)
set (Tristeon_VERSION_MAJOR 1)
set (Tristeon_VERSION_MINOR 1)	
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#CMake lookup path for modules (find_package)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Qt reflection
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Output/compilation
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#Including our own src folder for ease of use
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src")

#QT configuration stuff
if (MSVC)
    message("Use Visual Studio MSVC")
    # Visual Studio
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP true)
    include(InstallRequiredSystemLibraries)
    file(COPY ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION .)
endif()

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

#include Qt
include(${CMAKE_MODULE_PATH}/QtLocator.cmake)
find_package(Qt5 COMPONENTS Core Widgets OpenGL UiTools Qml Gamepad REQUIRED)
set(QT_USE_QTOPENGL TRUE)

#Config definitions
if (MSVC)
set(EDITOR_DEF -DTRISTEON_EDITOR=1)
set(LOG_DEF -DTRISTEON_LOGENABLED=1)
set(DEBUG_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})
else(MSVC)
set(EDITOR_DEF -DTRISTEON_EDITOR)
set(LOG_DEF -DTRISTEON_LOGENABLED)
set(DEBUG_FLAGS )
endif(MSVC)

#Set config flags for custom build versions
set(CMAKE_C_FLAGS_DEBUGEDITOR "${CMAKE_C_FLAGS_DEBUG} ${EDITOR_DEF} ${LOG_DEF}")
set(CMAKE_CXX_FLAGS_DEBUGEDITOR "${CMAKE_CXX_FLAGS_DEBUG} ${EDITOR_DEF} ${LOG_DEF}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUGEDITOR ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
set(CMAKE_SHARED_LINKER_FLAGS_DEBUGEDITOR ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})

set(CMAKE_C_FLAGS_EDITOR "${CMAKE_C_FLAGS_RELEASE} ${EDITOR_DEF} ${LOG_DEF}")
set(CMAKE_CXX_FLAGS_EDITOR "${EDITOR_DEF} ${LOG_DEF}")
set(CMAKE_EXE_LINKER_FLAGS_EDITOR ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
set(CMAKE_SHARED_LINKER_FLAGS_EDITOR ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${LOG_DEF}")

#Set build configs in cmake
set(CMAKE_CONFIGURATION_TYPES Debug DebugEditor Release Editor)

#Fix to allow QT to recognize debugeditor as a valid debug build
set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_DEBUGEDITOR "DEBUG")
set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_EDITOR "RELEASE")

set_target_properties(Qt5::Widgets PROPERTIES MAP_IMPORTED_CONFIG_DEBUGEDITOR "DEBUG")
set_target_properties(Qt5::Widgets PROPERTIES MAP_IMPORTED_CONFIG_EDITOR "RELEASE")

set_target_properties(Qt5::UiTools PROPERTIES MAP_IMPORTED_CONFIG_DEBUGEDITOR "DEBUG")
set_target_properties(Qt5::UiTools PROPERTIES MAP_IMPORTED_CONFIG_EDITOR "RELEASE")

set_target_properties(Qt5::OpenGL PROPERTIES MAP_IMPORTED_CONFIG_DEBUGEDITOR "DEBUG")
set_target_properties(Qt5::OpenGL PROPERTIES MAP_IMPORTED_CONFIG_EDITOR "RELEASE")

set_target_properties(Qt5::Qml PROPERTIES MAP_IMPORTED_CONFIG_DEBUGEDITOR "DEBUG")
set_target_properties(Qt5::Qml PROPERTIES MAP_IMPORTED_CONFIG_EDITOR "RELEASE")

set_target_properties(Qt5::Gamepad PROPERTIES MAP_IMPORTED_CONFIG_DEBUGEDITOR "DEBUG")
set_target_properties(Qt5::Gamepad PROPERTIES MAP_IMPORTED_CONFIG_EDITOR "RELEASE")

# ??
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

#Box2D
add_subdirectory(external/box2d)

#Libraries
macro(link_libs targetname)
	target_link_libraries(${targetname} PRIVATE Qt5::Widgets)
	target_link_libraries(${targetname} PRIVATE Qt5::UiTools)
	target_link_libraries(${targetname} PRIVATE Qt5::Qml)
	target_link_libraries(${targetname} PRIVATE Qt5::Gamepad)
	target_link_libraries(${targetname} PUBLIC box2d)
endmacro()

if (MSVC)
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
	
		ARCHIVE_OUTPUT_DIRECTORY_DEBUGEDITOR "${CMAKE_SOURCE_DIR}/bin"
		LIBRARY_OUTPUT_DIRECTORY_DEBUGEDITOR "${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY_DEBUGEDITOR "${CMAKE_SOURCE_DIR}/bin"
	
		ARCHIVE_OUTPUT_DIRECTORY_EDITOR "${CMAKE_SOURCE_DIR}/bin"
		LIBRARY_OUTPUT_DIRECTORY_EDITOR "${CMAKE_SOURCE_DIR}/bin"
		RUNTIME_OUTPUT_DIRECTORY_EDITOR "${CMAKE_SOURCE_DIR}/bin"
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
		<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='DebugEditor|x64'\">
			<LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
			<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
		</PropertyGroup>
		<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Editor|x64'\">
			<LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
			<DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
		</PropertyGroup>
		</Project>"
	)
	#Set Tristeon as the start project
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Tristeon)

	link_libs(Tristeon)
	
else(MSVC)
	#Create "build configurations" for other IDEs/compilers
	add_executable(Debug ${tristeonSRC})
	add_executable(Release ${tristeonSRC})
	add_executable(Editor ${tristeonSRC})
	add_executable(DebugEditor ${tristeonSRC})
	
	target_compile_definitions(Debug PUBLIC ${CMAKE_CXX_FLAGS_DEBUG})
	target_compile_definitions(Release PUBLIC ${CMAKE_CXX_FLAGS_RELEASE})
	target_compile_definitions(Editor PUBLIC ${CMAKE_CXX_FLAGS_EDITOR})
	target_compile_definitions(DebugEditor PUBLIC ${CMAKE_CXX_FLAGS_DEBUGEDITOR})
	
	link_libs(Debug)
	link_libs(Release)
	link_libs(Editor)
	link_libs(DebugEditor)
	
endif(MSVC)

#Add Qt binaries to build
execute_process(COMMAND ${QT_PATH}/bin/windeployqt.exe --compiler-runtime .
				WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)