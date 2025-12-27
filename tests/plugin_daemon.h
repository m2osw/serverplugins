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
#pragma once

// serverplugins
//
#include    <serverplugins/server.h>



namespace optional_namespace
{



SERVERPLUGINS_VERSION(daemon, 1, 0)


class daemon
    : public serverplugins::server
{
public:
    // we cannot use the SERVERPLUGINS_PLUGIN_DEFAULTS(daemon); so define
    // some useful parameters _by hand_
    //
    typedef std::shared_ptr<daemon>     pointer_t;

    // in most cases our daemons are given the argc/argv parameters from
    // main() and the daemon parses those with advgetopt
    //
    daemon(int argc, char * argv[]);

    int f_value = 0xA987;
};



} // optional_namespace namespace
// vim: ts=4 sw=4 et
