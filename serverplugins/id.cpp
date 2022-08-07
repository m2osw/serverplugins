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
#include    "serverplugins/id.h"

#include    "serverplugins/exception.h"


// cppthread
//
#include    <cppthread/guard.h>
#include    <cppthread/mutex.h>


// last include
//
#include    <snapdev/poison.h>



namespace serverplugins
{



namespace
{

/** \brief The list of identifiers.
 *
 * This maps registers all the identifiers by name. This is used by the
 * get_id() function to convert a name to an identifier.
 *
 * The first time get_id() is called with a new name, it allocates a
 * new identifier. It uses g_next_identifier to know what the next
 * identifier is.
 */
std::map<std::string, id_t>     g_identifiers = {};


/** \brief The next identifier.
 *
 * We allocate a new identifier each time the get_id() function is called
 * with a new name. That new identifier is generated using this global
 * variable.
 *
 * Note that the id_t type is an 8 bit value. You are limited to identifiers
 * from 1 to 255. We do not foresee a program that require that many plugin
 * collections. At the moment we have about 5 processes that make use of
 * plugins and 3 of them are separate programs. So collections used in
 * parallel are currently limited to about 2 (logger and tools that make
 * use of their own plugins). This limit may change in the future, but it
 * is never likely to ever approach 255.
 */
id_t                            g_next_identifier = NULL_ID;


/** \brief A mutex to protect the identifier variables.
 *
 * The identifier variables are globals so we need to protect them with
 * a guard when being accessed. This mutex is used when doing so. This
 * is a global mutex with a static scope so it is safe to use at any
 * time (i.e. the variable will always safely be initialized when the
 * function returns.)
 *
 * \return The identifier variables mutex.
 */
cppthread::mutex & id_mutex()
{
    static cppthread::mutex g_mutex = {};
    return g_mutex;
}



}
// no name namespace



/** \brief Determine the identifier of the specified \p id name.
 *
 * Each identifier is generated at runtime. It matches a name one to one.
 * This function converts the name to the identifier. If you already
 * called this function with that name, then the same identifier will
 * be returned. However, the identifier may change between runs. So do not
 * try to record such an identifier.
 *
 * \note
 * You are expected to use this function once and then use the returned
 * id_t for all your other calls, making things go a lot faster.
 *
 * \exception name_mismatch
 * This exception is raised if the input name is an empty string.
 *
 * \exception out_of_range
 * This exception is raised if this new name is the 256th name being
 * registered. The limit number of server identifiers is 255.
 *
 * \param[in] name  The name of the identifier as a string.
 *
 * \return The identifier number to use in the collection and plugins.
 */
id_t get_id(std::string const & name)
{
    if(name.empty())
    {
        throw name_mismatch("serverplugins: an identifier cannot be an empty string.");
    }

    cppthread::guard lock(id_mutex());

    auto it(g_identifiers.find(name));
    if(it != g_identifiers.end())
    {
        return it->second;
    }

    if(g_next_identifier == MAX_ID) // MAX_ID is inclusive
    {
        throw out_of_range("serverplugins: too many identifier created.");
    }

    ++g_next_identifier;
    g_identifiers[name] = g_next_identifier;

    return g_next_identifier;
}


/** \brief Retrieve an identifier.
 *
 * This function searches for an identifier by name. If it exists, the
 * existing identifier is returned. If it does not exist, then NULL_ID
 * is returned.
 *
 * Contrary to the get_id() which creates a new identifier if the specified
 * name is not yet registered, this function does not. It can be used to
 * check whether a name was already registered. However, make sure to not
 * do so after you create threads if you want to make this call and then
 * a call to get_id() atomic. (i.e. the function itself is thread safe,
 * but the calls to find_id() followed by get_id() would not be without
 * your own proper locking mechanism.)
 *
 * \exception name_mismatch
 * This exception is raised if the input name is an empty string.
 *
 * \param[in] name  The server name collection to search.
 *
 * \return The found identifier or NULL_ID.
 */
id_t find_id(std::string const & name)
{
    if(name.empty())
    {
        throw name_mismatch("serverplugins: an identifier cannot be an empty string.");
    }

    cppthread::guard lock(id_mutex());

    auto it(g_identifiers.find(name));
    if(it != g_identifiers.end())
    {
        return it->second;
    }

    return NULL_ID;
}


/** \brief Convert an identifier back to a string.
 *
 * This function is the converse of the get_id(). It converts an identifier
 * back to the string used to create it in the first place. This is useful
 * to print messages with the name rather than an identifier which could
 * vary between runs.
 *
 * The function returns an empty string if the specified identifier was not
 * yet registered.
 *
 * \param[in] id  The identifier to convert to a string.
 *
 * \return The name of this identifier or an empty string if not found.
 */
std::string get_name(id_t id)
{
    cppthread::guard lock(id_mutex());

    for(const auto & [key, value] : g_identifiers)
    {
        if(value == id)
        {
            return key;
        }
    }

    return std::string();
}




/** \typedef id_t
 * \brief An identifier used to define separate collections of plugins.
 *
 * When creating a collection of plugins, you need a server. Each collection
 * has its own set of plugins attached to a specific server. This happens
 * by assigning an identifier to a server and using that identifier in
 * the corresponding collection.
 *
 * You create a new identifier by calling the get_id() function. Calling
 * that function with the same identifier more than once always returns
 * the same number. The number may change between runs.
 *
 * \note
 * The identifier is limited to 255 numbers (1 to 255). It is not expected
 * that you would create more than 255 collections of plugins.
 */


} // namespace serverplugins
// vim: ts=4 sw=4 et
