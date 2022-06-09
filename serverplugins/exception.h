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

/** \file
 * \brief Exceptions for the thread environment.
 *
 * This file includes the definitions of exceptions used by the C++ Thread
 * environment.
 */

// libexcept
//
#include    <libexcept/exception.h>


namespace serverplugins
{



DECLARE_LOGIC_ERROR(logic_error);

DECLARE_OUT_OF_RANGE(out_of_range);

DECLARE_MAIN_EXCEPTION(serverplugins_exception);
DECLARE_EXCEPTION(serverplugins_exception, invalid_error);
DECLARE_EXCEPTION(serverplugins_exception, invalid_order);
DECLARE_EXCEPTION(serverplugins_exception, name_mismatch);
DECLARE_EXCEPTION(serverplugins_exception, not_found);
DECLARE_EXCEPTION(serverplugins_exception, server_already_exists);



} // namespace serverplugins
// vim: ts=4 sw=4 et
