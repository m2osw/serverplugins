# The following variables are defined with the C++ Thread library parameters:
#
# SERVERPLUGINS_FOUND        - System has CppThread
# SERVERPLUGINS_INCLUDE_DIRS - The CppThread include directories
# SERVERPLUGINS_LIBRARIES    - The libraries needed to use CppThread (none)
# SERVERPLUGINS_DEFINITIONS  - Compiler switches required for using CppThread (none)
#
# License:
#   Copyright (c) 2013-2022  Made to Order Software Corp.  All Rights Reserved
#
#   https://snapwebsites.org/project/serverplugins
#   contact@m2osw.com
# 
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program; if not, write to the Free Software Foundation, Inc.,
#   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

find_path(
    SERVERPLUGINS_INCLUDE_DIR
        serverplugins/plugin.h

    PATHS
        $ENV{SERVERPLUGINS_INCLUDE_DIR}
)

find_library(
    SERVERPLUGINS_LIBRARY
        serverplugins

    PATHS
        $ENV{SERVERPLUGINS_LIBRARY}
)

mark_as_advanced(
    SERVERPLUGINS_INCLUDE_DIR
    SERVERPLUGINS_LIBRARY
)

set(SERVERPLUGINS_INCLUDE_DIRS ${SERVERPLUGINS_INCLUDE_DIR})
set(SERVERPLUGINS_LIBRARIES    ${SERVERPLUGINS_LIBRARY}    )

include( FindPackageHandleStandardArgs )

# handle the QUIETLY and REQUIRED arguments and set SERVERPLUGINS_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    CppThread
    DEFAULT_MSG
    SERVERPLUGINS_INCLUDE_DIR
    SERVERPLUGINS_LIBRARY
)

# vim: ts=4 sw=4 et
