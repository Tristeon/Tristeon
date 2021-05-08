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
        string(FIND ${curdir}/${child} "Editor" EDITOR_DIR_FOUND)
		if (IS_DIRECTORY ${curdir}/${child} AND ${EDITOR_DIR_FOUND} EQUAL -1)
			read_directory(${curdir}/${child})
		endif()
	endforeach()
endmacro()
read_directory(${PROJECT_SOURCE_DIR}/src)

#Add GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
add_subdirectory(external/glfw)
include_directories(external/glfw/include)

if (TRISTEON_BUILD_EXE)
	add_executable(Tristeon ${tristeonSRC})
	target_link_libraries(Tristeon PRIVATE glfw)
endif()

if (TRISTEON_BUILD_TESTS)
	add_library(Tristeon_Lib ${tristeonSRC})
	target_link_libraries(Tristeon_Lib PRIVATE glfw)
endif()
