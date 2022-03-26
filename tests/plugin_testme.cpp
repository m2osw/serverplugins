// Copyright (c) 2006-2022  Made to Order Software Corp.  All Rights Reserved
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
#include    "plugin_testme.h"

#include    "plugin_daemon.h"

#include    "catch_main.h"


// serverplugins
//
#include    "serverplugins/collection.h"




/** \brief In your plugins, a namespace is encourage but optional.
 *
 * You can choose to have a namespace or not. We think it is cleaner to
 * have such a namespace, but it is not mandatory at all.
 */
namespace optional_namespace
{





SERVERPLUGINS_START(testme, 5, 3)
    , ::serverplugins::plugin_description("a test plugin to make sure it all works.")
    , ::serverplugins::plugin_help_uri("https://snapwebsites.org/")
    , ::serverplugins::plugin_icon("cute.ico")
    , ::serverplugins::plugin_categorization_tag("test")
    , ::serverplugins::plugin_categorization_tag("powerful")
    , ::serverplugins::plugin_categorization_tag("software")
    , ::serverplugins::plugin_conflict("other_test")
    , ::serverplugins::plugin_conflict("power_test")
    , ::serverplugins::plugin_conflict("unknown")
    , ::serverplugins::plugin_suggestion("beautiful")
SERVERPLUGINS_END(testme)


void testme::bootstrap()
{
    //data_t * d(reinterpret_cast<data_t *>(data));
    daemon::pointer_t d(collection()->get_server<daemon>());

    // somehow, the test environment thinks we're not inside the
    // CATCH_TEST_CASE() when any functions here gets called
    //
    //CATCH_CHECK(d->f_value == 0xA987);

    if(d == nullptr)
    {
        throw std::runtime_error("testme: plugin could not find the daemon pointer");
    }

    if(d->f_value != 0xA987)
    {
        throw std::runtime_error("testme: plugin called with an unexpected data pointer.");
    }
}


std::string testme::it_worked()
{
    return std::string("testme:plugin: it worked, it was called!");
}


} // optional_namespace namespace
// vim: ts=4 sw=4 et
