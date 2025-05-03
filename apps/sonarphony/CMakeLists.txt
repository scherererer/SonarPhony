cmake_minimum_required(VERSION 3.16)
project(sonarphony LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0100 NEW)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
add_compile_options(-Wall -Wextra -Werror -Wno-error=address-of-packed-member)

find_package(Qt6 REQUIRED COMPONENTS Network Widgets)

qt_add_executable(sonarphony
	about.hh
	columnPulser.hh
	mainWindow.hh
	palette.hh
	playback.hh
	waterfall.hh

	about.cc
	columnPulser.cc
	main.cc
	mainWindow.cc
	palette.cc
	playback.cc
	waterfall.cc

	about.ui
	mainWindow.ui
	preferences.ui
	)

target_include_directories(sonarphony PRIVATE .)

target_link_libraries(sonarphony PRIVATE Qt6::Network Qt6::Widgets libsonarphony)

set_target_properties(sonarphony PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

