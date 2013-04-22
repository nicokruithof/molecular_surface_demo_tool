cmake_minimum_required(VERSION 2.8)
project(SkinSurfaceViewer)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 

# Qt
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# OpenSceneGraph
find_package(OpenSceneGraph COMPONENTS osg osgViewer osgGA osgDB osgSim osgAnimation osgFX osgUtil osgText)
find_library(OSG_QT NAMES osgQt)

# Boost
find_package( Boost COMPONENTS system program_options filesystem regex)
include_directories( ${Boost_INCLUDE_DIR} )

# CGAL
#find_package(CGAL REQUIRED)
#include(${CGAL_USE_FILE})

add_subdirectory("src")
