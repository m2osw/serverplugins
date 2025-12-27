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

/** \file
 * \brief Documentation of the exception.h file.
 *
 * The exception.h file is a template so we document that template here.
 */

#error "Documentation only file, do not compile."

namespace serverplugins
{



/** \class logic_error
 * \brief The library detected an unexpected combination of events.
 *
 * This exception is raised when the library checks that things are in
 * order and detects that they are not. This is similar to a function
 * contract.
 */

/** \class out_of_range
 * \brief A value is out of range and cannot be used as is.
 *
 * This exception is raised when a value is either too small or too large.
 */

/** \class serverplugins_exception
 * \brief To catch any serverplugins exception, catch this base exception.
 *
 * This is the base serverplugins exception for all the exceptions defined
 * in the serverplugins except for the logic errors (see logic_error and
 * out_of_range) which should never happen so should also not be caught.
 *
 * You may catch this exception to catch any of the serverplugins exceptions.
 */

/** \class invalid_error
 * \brief An invalid parameter or value was detected.
 *
 * This exception is raised when a parameter or a variable member or some
 * other value is out of range or generally not valid for its purpose.
 */

/** \class invalid_order
 * \brief The do_update() dates must be defined in increasing order.
 *
 * The code detected a date not increasing as expected. The do_update()
 * would not work as expected in that situation.
 */

/** \class name_mismatch
 * \brief Two references to the same object used different names.
 *
 * This error occurs whenever an object detects two different names to
 * reference it or one of its children.
 */

/** \class not_found
 * \brief The file of a plugin was not found.
 *
 * Whenever trying to load a plugin, it has to exist on disk. If not found,
 * then this exception is raised.
 *
 * Note that it is a fatal error when a plugin cannot be loaded.
 */

/** \class plugins_already_loaded
 * \brief Calling a collection load_plugins() twice is not possible.
 *
 * When using the thread_safe object which is created on the FIFO, one
 * guarantee is that the thread actually starts. If the threads cannot
 * be started, this exception is raised.
 */



} // namespace serverplugins
// vim: ts=4 sw=4 et
