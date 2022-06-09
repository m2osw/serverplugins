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

// self
//
#include    <serverplugins/plugin.h>


// cppthread
//
#include    <cppthread/mutex.h>



namespace serverplugins
{
namespace detail
{


// WARNING: this is a detail (internal) class, do not use directly
class repository
{
public:
    static repository &  instance();
    plugin::pointer_t           get_plugin(names::filename_t const & filename);
    void                        register_plugin(plugin::pointer_t p);

private:
    cppthread::mutex            f_mutex = cppthread::mutex();
    plugin::map_t               f_plugins = plugin::map_t();        // WARNING: this map is sorted by filename
    names::filename_t           f_register_filename = names::filename_t();
};



} // namespace detail
} // namespace serverplugins
// vim: ts=4 sw=4 et
