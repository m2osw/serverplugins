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

// self
//
#include    "serverplugins/paths.h"

#include    "serverplugins/exception.h"


// snapdev
//
#include    <snapdev/join_strings.h>
#include    <snapdev/pathinfo.h>
#include    <snapdev/tokenize_string.h>


// last include
//
#include    <snapdev/poison.h>



namespace serverplugins
{



/** \class paths
 * \brief The list of paths.
 *
 * The paths holds a list of paths that are used to search the
 * plugins. By default this list is empty which is viewed as a list
 * of having just "." as the path by the names::find_plugins().
 *
 * To add plugin paths, use the push(), add(), and set() functions to
 * see how to add new paths. In most cases, the set() function is ideal
 * if you read a list of paths from a configuration file.
 *
 * The set() function accepts a list of paths separated by colon (:)
 * characters. It is often used from an environment variable or a
 * parameter in a configuration file.
 */



/** \brief Get the number of paths defined in this set of paths.
 *
 * This function returns the number of paths this set has.
 *
 * \return The number of paths.
 */
std::size_t paths::size() const
{
    return f_paths.size();
}


/** \brief Get the path at the specified index.
 *
 * This function retrieves the path defined at \p idx.
 *
 * \param[in] idx  The index of the path you are trying to retrieve.
 *
 * \return The path defined at index \p idx. If \p idx is too large, then
 * this function returns an empty string.
 */
std::string paths::at(std::size_t idx) const
{
    if(idx >= f_paths.size())
    {
        return std::string();
    }

    return f_paths[idx];
}


/** \brief Change the allow-redirect flag.
 *
 * This function is used to switch the allow-redirect flag to true or false.
 * By default the flag is false.
 *
 * Setting the flag to true means that a user can define a relative path
 * outside of the current path (i.e. which starts with "../").
 *
 * \remarks
 * Most often, paths for plugin locations are full root paths so this flag
 * doesn't apply to those. Also, the canonicalization checks in memory
 * strings only. It will not verify that the path is not going outside of
 * the current path through softlink files.
 *
 * \param[in] allow  Whether to allow redirects in paths.
 *
 * \sa get_allow_redirects()
 * \sa canonicalize()
 */
void paths::set_allow_redirects(bool allow)
{
    f_allow_redirects = allow;
}


/** \brief Check whether redirects are allowed or not.
 *
 * This function returns true if redirects are allowed, false otherwise.
 *
 * When canonicalizing a path, a ".." outside of the current directory
 * is viewed as a redirect. These are not allowed by default for obvious
 * security reasons. When false and such a path is detected, the
 * canonicalize() function throws an error.
 *
 * \return true when redirects are allowed.
 *
 * \sa set_allow_redirects()
 */
bool paths::get_allow_redirects() const
{
    return f_allow_redirects;
}


/** \brief Canonicalize the input path.
 *
 * This function canonicalize the input path so that two paths referencing
 * the same files can easily be compared against each other.
 *
 * \note
 * We do not test the current local system (for one reason, the path may
 * not refer to a local file), so we will not detect soft links and relative
 * versus full path equivalents.
 *
 * \exception serverplugins_invalid_error
 * The input \p path cannot be an empty string. Also, when the
 * allow-redirects flag (see the set_allow_redirects() function) is
 * set to false (the default), then the exception is raised if the path
 * starts with "../".
 *
 * \param[in] path  The path to be converted.
 *
 * \return The canonicalized path.
 *
 * \sa push()
 */
paths::path_t paths::canonicalize(path_t const & path)
{
    if(path.empty())
    {
        throw invalid_error("path cannot be an empty string.");
    }

    std::string const p(snapdev::pathinfo::canonicalize(path, std::string()));

    // by default we do not allow relative paths that start with "../"
    //
    // TODO:
    // but if you use a full path, it will pass just fine... i.e. we need to
    // test whether it is under a certain folder instead
    //
    if(!f_allow_redirects)
    {
        if(p.length() >= 2
        && p[0] == '.'
        && p[1] == '.')
        {
            if(p.length() < 3
            || p[2] == '/')
            {
                throw invalid_error(
                      "the path \""
                    + path
                    + "\" going outside of the allowed range.");
            }
        }
    }

    return p;
}


/** \brief Add one path to this set of paths.
 *
 * This function is used to add a path to this set of paths.
 *
 * Before adding the new path, we make sure that it is not already defined
 * in the existing set. Adding the same path more than once is not useful.
 * Only the first instance would be useful and the second would generate
 * a waste of time.
 *
 * In many cases, you will want to use the add() function instead as it
 * is capable to add many paths separated by colons all at once.
 *
 * \note
 * The function calls canonicalize() on the input path. If the path is
 * considered invalid, then an exception is raised.
 *
 * \param[in] path  The path to be added.
 *
 * \sa add()
 * \sa canonicalize()
 */
void paths::push(path_t const & path)
{
    path_t const canonicalized(canonicalize(path));
    auto it(std::find(f_paths.begin(), f_paths.end(), canonicalized));
    if(it == f_paths.end())
    {
        f_paths.push_back(canonicalized);
    }
}


/** \brief Add one set of paths to this set of paths.
 *
 * This function is used to add a set of colon separated paths defined in
 * one string. The function automatically separate each path at the colon
 * and adds the resulting paths to this object using the add() function.
 *
 * \note
 * The function further removes blanks (space, tab, newline, carriage
 * return) at the start and end of each path. Such characters should not
 * be supported at those locations by any sensible file systems anyway.
 *
 * \param[in] set  The set of colon or spaces separated paths to be added.
 *
 * \sa erase()
 */
void paths::add(std::string const & set)
{
    std::vector<std::string> p;
    snapdev::tokenize_string(p, set, ":", true, {' ', '\t', '\r', '\n'});
    for(auto const & path : p)
    {
        push(path);
    }
}


/** \brief Erase the specified path from this list.
 *
 * This function searches for the specified \p path and remove it from the
 * list. If not present in the list, then nothing happens.
 *
 * \param[in] path  The path to be removed.
 */
void paths::erase(std::string const & path)
{
    auto it(std::find(f_paths.begin(), f_paths.end(), path));
    if(it != f_paths.end())
    {
        f_paths.erase(it);
    }
}



} // namespace serverplugins
// vim: ts=4 sw=4 et
