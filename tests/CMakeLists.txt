cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(Tristeon_Test LANGUAGES CXX)

add_subdirectory(external/doctest)
include_directories(external/doctest)

macro(read_directory curdir)
	file(GLOB children RELATIVE ${curdir} ${curdir}/*)
	
	set(groupList "")
	
	foreach(child ${children})
		if (NOT (IS_DIRECTORY ${curdir}/${child}))
			list(APPEND tristeonTestSrc ${curdir}/${child})
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

add_executable(Tristeon_Test ${tristeonTestSrc})
target_link_libraries(Tristeon_Test PRIVATE doctest)
target_link_libraries(Tristeon_Test PRIVATE Tristeon_Lib)