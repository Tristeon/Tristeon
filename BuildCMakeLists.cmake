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

#Add header & source files
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
        string(FIND ${curdir}/${child} "Editor" EDITOR_DIR_FOUND)
		if (IS_DIRECTORY ${curdir}/${child} AND ${EDITOR_DIR_FOUND} EQUAL -1)
			read_directory(${curdir}/${child})
		endif()
	endforeach()
endmacro()
read_directory(${PROJECT_SOURCE_DIR}/src)

add_definitions(-DTRISTEON_LOGENABLED)

#External libraries
add_subdirectory(external/box2d)
add_subdirectory(external/glew)
add_subdirectory(external/glfw)
include_directories(${CMAKE_SOURCE_DIR}/external/glew/include)

#Libraries
macro(link_libs targetname)
    target_link_libraries(${targetname} PUBLIC box2d)
    target_link_libraries(${targetname} PUBLIC libglew_static)
    target_link_libraries(${targetname} PUBLIC glfw)
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
	#Create "build configurations" for other IDEs/compilers
	add_executable(Debug ${tristeonSRC})
	add_executable(Release ${tristeonSRC})
	
	target_compile_definitions(Debug PUBLIC ${CMAKE_CXX_FLAGS_DEBUG})
	target_compile_definitions(Release PUBLIC ${CMAKE_CXX_FLAGS_RELEASE})
	
	link_libs(Debug)
	link_libs(Release)
endif(MSVC)