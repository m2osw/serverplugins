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

#include    "plugin_daemon.h"


// last include
//
#include    <snapdev/poison.h>


namespace optional_namespace
{

daemon::daemon(int argc, char * argv[])
    : server(serverplugins::get_id("daemon"))
{
    // this is where we would parse argc/argv
    CATCH_REQUIRE(argc == 1);
    CATCH_REQUIRE(strcmp(argv[0], "/usr/sbin/daemon") == 0);
    CATCH_REQUIRE(argv[1] == nullptr);
}

} // namespace optional_namespace
// vim: ts=4 sw=4 et
