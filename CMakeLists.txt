cmake_minimum_required (VERSION 2.6)
project (WindowManager)

add_library(WindowManager STATIC
	src/CWindowManager.cpp
	)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/modules/")

find_package(SDL REQUIRED)
if(SDL_FOUND)
	message("SDL found")
	include_directories(${SDL_INCLUDE_DIR})
	target_link_libraries(WindowManager ${SDL_LIBRARY})
else(SDL_FOUND)
	message("SDL not found")
endif(SDL_FOUND)

install(FILES
	src/CWindowManager.hpp
	DESTINATION include/WindowManager
	)

install(TARGETS WindowManager
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)
