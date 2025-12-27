// Copyright (c) 2013-2025  Made to Order Software Corp.  All Rights Reserved
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

/** \file
 * \brief Define a server.
 *
 * A server is a plugin which also does manage the collection of plugins.
 * This is really just a shell definition, although there are differences
 * on how to initialize the server.
 *
 * In most cases, you want to define a .cpp file with your server.
 * For example, the communicatord is a server that derives from this
 * class:
 *
 * \code
 * class communicatord
 *     : public serverplugins::server
 * {
 *     communicatord(int argc, char * argv[]);
 *     ...
 * \endcode
 *
 * The constructor of the communicatord class accepts the argc/argv
 * parameters passed to the main() function. This is not compatible
 * with the default creation of a plugin and its factory.
 *
 * So this is why we have a separate class, to make it cleaner. But
 * this class, at the moment, is really just a shell.
 *
 * The implementation first includes the server definition like so:
 *
 * \code
 * SERVERPLUGINS_START_SERVER(communicatord)
 *     , ::serverplugins::description("The main server in the communicator daemon.")
 *     , ::serverplugins::help_uri("https://snapwebsites.org/help")
 *     , ::serverplugins::categorization_tag("server")
 * SERVERPLUGINS_END_SERVER(communicatord)
 * \endcode
 *
 * It looks very similar to a plugin definition, only it uses the
 * SERVERPLUGINS_START_SERVER() and SERVERPLUGINS_END_SERVER() macros.
 * This creates the factory without a pointer to the plugin.
 *
 * When you create your server, you then can pass the factory to
 * the server constructor:
 *
 * \code
 * communicatord::communicatord(int argc, char * argv[])
 *     : server(g_communicatord_factory)
 *     , ...
 * {
 *     ...
 * \endcode
 *
 * Finally, we want to finish up the creation of the factory by calling
 * the complete_plugin_initialization() function. This saves the plugin
 * shared pointer in the factory. It needs to be done before you call
 * the load_plugins(). Until this is called, the factory::get_plugin()
 * function returns a null pointer which is likely to crash your
 * application if not initialized early on.
 *
 * Here is an example in the communicator daemon implementation
 * (in the daemon/main.cpp file):
 *
 * \code
 *     communicator_daemon::communicatord::pointer_t server(std::make_shared<communicator_daemon::communicatord>(argc, argv));
 *     server->complete_plugin_initialization();
 *     return server->run();
 * \endcode
 */

// self
//
#include    <serverplugins/factory.h>
#include    <serverplugins/plugin.h>



namespace serverplugins
{



class server
    : public plugin
{
public:
    // Do not use the SERVERPLUGINS_DEFAULTS() because at this point
    // we do not have access to the factory
    //
    typedef std::shared_ptr<server> pointer_t;

                            server(factory const & f);
                            server(server const &) = delete;
    virtual                 ~server();
    server &                operator = (server const &) = delete;

    //static pointer_t        instance(id_t id);
    //id_t                    get_server_id() const;

//private:
//    id_t                    f_id = NULL_ID;
};



//namespace detail
//{
//
//class server_plugin_factory
//    : public factory
//{
//public:
//    server_plugin_factory(server::pointer_t s);
//    server_plugin_factory(server_plugin_factory const &) = delete;
//    server_plugin_factory & operator = (server_plugin_factory const &) = delete;
//};
//
//extern server_plugin_factory * g_server_plugin_factory[MAX_ID];
//
//} // namespace detail



} // namespace serverplugins
// vim: ts=4 sw=4 et
