cmake_minimum_required(VERSION 3.16)
project(sonarphonyd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0100 NEW)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
add_compile_options(-Wall -Wextra -Werror -Wno-error=address-of-packed-member)

find_package(Qt6 REQUIRED COMPONENTS Network Core)

qt_add_executable(sonarphonyd
	daemon.hh
	daemon.cc
	main.cc
	)

target_link_libraries(sonarphonyd PRIVATE Qt6::Network Qt6::Core libsonarphony)

set_target_properties(sonarphonyd PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)


