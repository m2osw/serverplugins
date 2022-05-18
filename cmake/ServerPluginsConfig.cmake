# - Find Server Plugins
#
# SERVERPLUGINS_FOUND        - System has Server Plugins
# SERVERPLUGINS_INCLUDE_DIRS - The Server Plugins include directories
# SERVERPLUGINS_LIBRARIES    - The libraries needed to use Server Plugins
# SERVERPLUGINS_DEFINITIONS  - Compiler switches required for using Server Plugins
#
# License:
#
# Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
#
# https://snapwebsites.org/project/serverplugins
# contact@m2osw.com
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

find_path(
    SERVERPLUGINS_INCLUDE_DIR
        serverplugins/plugin.h

    PATHS
        ENV SERVERPLUGINS_INCLUDE_DIR
)

find_library(
    SERVERPLUGINS_LIBRARY
        serverplugins

    PATHS
        ${SERVERPLUGINS_LIBRARY_DIR}
        ENV SERVERPLUGINS_LIBRARY
)

mark_as_advanced(
    SERVERPLUGINS_INCLUDE_DIR
    SERVERPLUGINS_LIBRARY
)

set(SERVERPLUGINS_INCLUDE_DIRS ${SERVERPLUGINS_INCLUDE_DIR})
set(SERVERPLUGINS_LIBRARIES    ${SERVERPLUGINS_LIBRARY})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    CppThread
    REQUIRED_VARS
        SERVERPLUGINS_INCLUDE_DIR
        SERVERPLUGINS_LIBRARY
)

# vim: ts=4 sw=4 et
