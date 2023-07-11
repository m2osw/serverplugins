// Copyright (c) 2013-2023  Made to Order Software Corp.  All Rights Reserved
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
#include    "serverplugins/server.h"

#include    "serverplugins/version.h"


// last include
//
#include    <snapdev/poison.h>



namespace serverplugins
{


namespace detail
{



definition const g_server_plugin_definition = define_plugin(
      plugin_version(version_t(1, 0, 0))
    , library_version(version_t(SERVERPLUGINS_VERSION_MAJOR, SERVERPLUGINS_VERSION_MINOR, SERVERPLUGINS_VERSION_PATCH))
    , last_modification(UTC_BUILD_TIME_STAMP)
    , plugin_name("server")
);

server_plugin_factory::server_plugin_factory(server::pointer_t s)
    : factory(g_server_plugin_definition, std::dynamic_pointer_cast<plugin>(s))
{
    save_factory_in_plugin(s.get());
    register_plugin("server", instance());
}


server_plugin_factory * g_server_plugin_factory[MAX_ID] = {};




} // namespace detail


server::server(id_t id)
    : f_id(id)
{
    if(f_id == NULL_ID)
    {
        throw out_of_range("NULL_ID is not a valid identifier for server::server().");
    }
}

server::~server()
{
}

server::pointer_t server::instance(id_t id)
{
    if(id == NULL_ID)
    {
        throw out_of_range("NULL_ID is not a valid identifier for server::instance().");
    }
    if(detail::g_server_plugin_factory[id] == nullptr)
    {
        throw not_found("server with identifier " + std::to_string(static_cast<unsigned int>(id)) + " is not defined. Did you call collection::load_plugins() yet?");
    }
    return std::static_pointer_cast<server>(detail::g_server_plugin_factory[id]->instance());
}


id_t server::get_server_id() const
{
    return f_id;
}



} // namespace serverplugins
// vim: ts=4 sw=4 et
