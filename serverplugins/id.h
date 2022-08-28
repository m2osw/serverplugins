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

// C++
//
//#include    <cstddef>
//#include    <cstdint>
#include    <limits>
#include    <map>
#include    <string>



namespace serverplugins
{



typedef std::uint8_t        id_t;

constexpr id_t              NULL_ID = 0;

id_t                        get_id(std::string const & name);
id_t                        find_id(std::string const & name);
std::string                 get_name(id_t id);

constexpr std::size_t       MIN_ID = static_cast<id_t>(1);
constexpr std::size_t       MAX_ID = std::numeric_limits<id_t>::max();



} // namespace serverplugins
// vim: ts=4 sw=4 et
