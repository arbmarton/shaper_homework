cmake_minimum_required(VERSION 3.9)
project(shaper_homework)

set(GLM_TEST_ENABLE OFF)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/glm/)

set(HEADER_FILES
	include/Converter.h
	include/Enums.h
	include/I_InputParser.h
	include/I_OutWriter.h
	include/ObjParser.h
	include/StlWriter.h
	include/Utilities.h
)

set(SOURCE_FILES
	src/Converter.cpp
	src/I_InputParser.cpp
	src/I_OutWriter.cpp
	src/main.cpp
	src/ObjParser.cpp
	src/StlWriter.cpp
	src/Utilities.cpp
)

add_executable(${PROJECT_NAME}
			   ${HEADER_FILES}
			   ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
						   ${CMAKE_CURRENT_LIST_DIR}/include
						   ${CMAKE_CURRENT_LIST_DIR}/thirdparty/glm/glm/
)

target_link_libraries(${PROJECT_NAME} 
					  ${CMAKE_CURRENT_LIST_DIR}/out/thirdparty/glm/glm/$<CONFIG>/glm_static.lib
)

add_dependencies(${PROJECT_NAME} glm)

source_group("shaders" FILES ${VERTEX_SHADERS} ${FRAGMENT_SHADERS})
source_group("thirdparty" FILES ${THIRDPARTY_HEADERS})
				
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Parallel compilation and C++17
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /MP")
endif(MSVC)


# Clang format setup
# http://mariobadr.com/using-clang-format-to-enforce-style.html
find_program(CLANG_FORMAT_EXE NAMES "clang-format")
if(NOT CLANG_FORMAT_EXE)
	message(STATUS "clang-format not found.")
else()
	message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")
	set(DO_CLANG_FORMAT "${CLANG_FORMAT_EXE}" "-i -style=file")
endif()

function(prepend var prefix)
	set(listVar "")
	
	foreach(f ${ARGN})
	  list(APPEND listVar "${prefix}/${f}")
	endforeach()
	
	set(${var} "${listVar}" PARENT_SCOPE)
endfunction()

if(CLANG_FORMAT_EXE)
	prepend(FILES_TO_FORMAT ${CMAKE_CURRENT_SOURCE_DIR} ${HEADER_FILES} ${SOURCE_FILES})
	
	add_custom_target(
		CLANG_FORMAT
		COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${FILES_TO_FORMAT}
	)
endif()