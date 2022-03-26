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
#include    "serverplugins/definition.h"


// C++ set
//
#include    <memory>



namespace serverplugins
{



class plugin;


class plugin_factory
{
public:
                                    plugin_factory(plugin_definition const & definition, std::shared_ptr<plugin> instance);
                                    ~plugin_factory();
                                    plugin_factory(plugin_factory const &) = delete;
    plugin_factory &                operator = (plugin_factory const &) = delete;

    plugin_definition const &       definition() const;
    std::shared_ptr<plugin>         instance() const;
    void                            register_plugin(char const * name, std::shared_ptr<plugin> p);

protected:
    void                            save_factory_in_plugin(plugin * p);

private:
    plugin_definition const &       f_definition;
    std::shared_ptr<plugin>         f_plugin = std::shared_ptr<plugin>();
};



} // namespace serverplugins
// vim: ts=4 sw=4 et
