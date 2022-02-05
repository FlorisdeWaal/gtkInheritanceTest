# src/CMakeLists.txt
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

# Project name and current version
project(rascam VERSION 0.1 LANGUAGES CXX)

# Enable general warnings
# See http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread")

# Use 2014 C++ standard.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Creates the compile_commands.json used by editors for code completion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
# Must use GNUInstallDirs to install libraries into correct locations on all platforms:
include(GNUInstallDirs)

# Pkg_config is used to locate header and files for dependency libraries:
find_package(PkgConfig)

# Defines variables GTKMM_INCLUDE_DIRS, GTKMM_LIBRARY_DIRS and GTKMM_LIBRARIES.
pkg_check_modules(GTKMM gtkmm-3.0) 
link_directories( ${GTKMM_LIBRARY_DIRS} )
include_directories( ${GTKMM_INCLUDE_DIRS} )

link_directories( ${BOOST_LIBRARY_DIRS})
include_directories( ${BOOST_INCLUDE_DIRS})
# OpenCV can be linked in a more standard manner:
find_package( OpenCV REQUIRED )
find_package(Boost COMPONENTS log REQUIRED )

# Compile files:
add_executable(test
    cpp/main.cpp
    cpp/window.cpp
	cpp/drawingarea.cpp
)

# Add folder with all headers:
target_include_directories(test PRIVATE hpp)

# Link files:
target_link_libraries(test
	${GTKMM_LIBRARIES}
	${OpenCV_LIBS}
    Boost::log_setup Boost::log
	)

