# © 2020 Joseph Cameron - All Rights Reserved

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(null)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/jfc-cmake/jfclib.cmake")

option(JFC_BUILD_DOCS "Build documentation" OFF)
option(JFC_BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(thirdparty)

# GUI
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

FIND_PACKAGE(CURL REQUIRED)

jfc_project(executable
    NAME "jfctraviscicanary"
    VERSION 0.0
    DESCRIPTION "gtk sysicon, alert if error in recent build list"
    C++_STANDARD 17
    C_STANDARD 90

    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${GTK3_INCLUDE_DIRS}
        ${CURL_INCLUDE_DIR}
#       ${dependencyname_INCLUDE_DIRECTORIES} 


    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include

    LIBRARIES
        ${GTK3_LIBRARIES}
        ${CURL_LIBRARIES}
#       ${dependencyname_LIBRARIES} 

    SOURCE_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/travis-ci-32-building.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/travis-ci-32-error.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/travis-ci-32-no_connection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/travis-ci-32-ok.cpp

#   DEPENDENCIES
#       "dependencyname"
)

if (JFC_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (JFC_BUILD_DOCS)
    add_subdirectory(docs)
endif()

