// Copyright (c) 2006-2025  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/serverplugins
// contact@m2osw.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

// self
//
#include    "catch_main.h"


// serverplugins
//
#include    <serverplugins/version.h>




CATCH_TEST_CASE("version", "[version]")
{
    CATCH_START_SECTION("version: verify runtime vs compile time version numbers")
    {
        CATCH_CHECK(serverplugins::get_major_version()   == SERVERPLUGINS_VERSION_MAJOR);
        CATCH_CHECK(serverplugins::get_release_version() == SERVERPLUGINS_VERSION_MINOR);
        CATCH_CHECK(serverplugins::get_patch_version()   == SERVERPLUGINS_VERSION_PATCH);
        CATCH_CHECK(strcmp(serverplugins::get_version_string(), SERVERPLUGINS_VERSION_STRING) == 0);
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
