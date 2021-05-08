OPTION(TRISTEON_DISABLE_WINDEPLOY OFF)

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

add_definitions(-DTRISTEON_EDITOR)

#include Qt
if (NOT DEFINED Qt5_DIR)
	message("Qt5_DIR not set, attempting to find it.")
	include(${CMAKE_MODULE_PATH}/QtLocator.cmake)
else()
	message("Qt5_DIR set, using dir directly.")
endif()

find_package(Qt5 COMPONENTS Core Widgets OpenGL UiTools Qml Gamepad REQUIRED)
set(QT_USE_QTOPENGL TRUE)

if (TRISTEON_BUILD_EXE)
	add_executable(Tristeon ${tristeonSRC})
	target_link_libraries(Tristeon PRIVATE Qt5::Widgets)
	target_link_libraries(Tristeon PRIVATE Qt5::UiTools)
	target_link_libraries(Tristeon PRIVATE Qt5::Qml)
	target_link_libraries(Tristeon PRIVATE Qt5::Gamepad)
endif()

if (TRISTEON_BUILD_TESTS)
	add_library(Tristeon_Lib ${tristeonSRC})
	target_link_libraries(Tristeon_Lib PRIVATE Qt5::Widgets)
	target_link_libraries(Tristeon_Lib PRIVATE Qt5::UiTools)
	target_link_libraries(Tristeon_Lib PRIVATE Qt5::Qml)
	target_link_libraries(Tristeon_Lib PRIVATE Qt5::Gamepad)
endif()

if(NOT TRISTEON_DISABLE_WINDEPLOY)
	#Add windeploy as a dependant target
	add_custom_target(WinDeploy
		COMMAND ${QT_PATH}/bin/windeployqt.exe --compiler-runtime --pdb .
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/
	)
	add_dependencies(Tristeon WinDeploy)
	
	#Add Qt binaries to build
	execute_process(COMMAND ${QT_PATH}/bin/windeployqt.exe --compiler-runtime --pdb .
					WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif()

