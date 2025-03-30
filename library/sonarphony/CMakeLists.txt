cmake_minimum_required(VERSION 3.16)
project(libsonarphony LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0100 NEW)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
add_compile_options(-fvisibility=hidden -Wall -Wextra -Werror -Wno-error=address-of-packed-member)

find_package(Qt6 REQUIRED COMPONENTS Core Network)
qt_standard_project_setup()

qt_add_library(libsonarphony
	../../include/sonarphony/global.hh
	../../include/sonarphony/logger.hh
	../../include/sonarphony/pingMsg.hh
	../../include/sonarphony/sonarConnection.hh
	../../include/sonarphony/sonarMsg.hh

	logger.cc
	masterCommand.cc
	masterHandshake.cc
	pingMsg.cc
	sonarConnection.cc
	sonarHandshakeMsg.cc
	sonarMsg.cc
	)

target_compile_definitions(libsonarphony PRIVATE SONARPHONY_LIBRARY)

target_include_directories(libsonarphony PUBLIC ../../include/)

target_link_libraries(libsonarphony PUBLIC Qt6::Core PRIVATE Qt6::Network)


