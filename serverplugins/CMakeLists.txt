# Copyright (c) 2013-2022  Made to Order Software Corp.  All Rights Reserved
#
# https://snapwebsites.org/project/serverplugins
# contact@m2osw.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

##
## serverplugins library
##
project(serverplugins)

# Put the version in the header file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

add_library(${PROJECT_NAME} SHARED
    collection.cpp
    factory.cpp
    names.cpp
    paths.cpp
    plugin.cpp
    repository.cpp
    server.cpp
    version.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${LIBEXCEPT_INCLUDE_DIRS}
        ${SNAPDEV_INCLUDE_DIRS}
        ${SNAPLOGGER_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${LIBEXCEPT_LIBRARIES}
    ${CPPTHREAD_LIBRARIES}
    ${SNAPLOGGER_LIBRARIES}
    dl
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION
        ${SERVERPLUGINS_VERSION_MAJOR}.${SERVERPLUGINS_VERSION_MINOR}

    SOVERSION
        ${SERVERPLUGINS_VERSION_MAJOR}
)

install(
    TARGETS
        ${PROJECT_NAME}

    RUNTIME DESTINATION
        bin

    LIBRARY DESTINATION
        lib

    ARCHIVE DESTINATION
        lib
)

install(
    FILES
        exception.h
        plugin.h
        collection.h
        definition.h
        factory.h
        names.h
        paths.h
        server.h
        signals.h
        utils.h
        ${CMAKE_CURRENT_BINARY_DIR}/version.h

    DESTINATION
        include/serverplugins
)


# vim: ts=4 sw=4 et
