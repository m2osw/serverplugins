// Copyright (c) 2013-2022  Made to Order Software Corp.  All Rights Reserved
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

// self
//
#include    "serverplugins/factory.h"
#include    "serverplugins/plugin.h"



namespace serverplugins
{



class server
    : public plugin
{
public:
    // Do not use the CPPTHREAD_PLUGIN_DEFAULTS() because at this point
    // we do not have access to the factory
    //
    typedef std::shared_ptr<server> pointer_t;

                            server();
                            server(server const &) = delete;
    virtual                 ~server();
    server &                operator = (server const &) = delete;
    static pointer_t        instance();
};



namespace detail
{

class server_plugin_factory
    : public factory
{
public:
    server_plugin_factory(server::pointer_t s);
    server_plugin_factory(server_plugin_factory const &) = delete;
    server_plugin_factory & operator = (server_plugin_factory const &) = delete;
};

extern server_plugin_factory * g_server_plugin_factory;

} // namespace detail



} // namespace serverplugins
// vim: ts=4 sw=4 et
