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

// self
//
#include    <serverplugins/names.h>
#include    <serverplugins/definition.h>


// C++
//
#include    <memory>



namespace serverplugins
{



class collection;
class factory;

namespace detail
{
class repository;
} // namespace detail

class plugin
{
public:
    typedef std::shared_ptr<plugin>     pointer_t;
    typedef std::vector<pointer_t>      vector_t;       // sorted by dependencies & then by name
    typedef std::map<std::string, pointer_t>
                                        map_t;          // sorted by name

                                        plugin();       // for the server
                                        plugin(factory const & f);
                                        plugin(plugin const &) = delete;
    virtual                             ~plugin();
    plugin &                            operator = (plugin const &) = delete;

    version_t                           version() const;
    time_t                              last_modification() const;
    names::name_t                       name() const;
    names::filename_t                   filename() const;
    std::string                         description() const;
    std::string                         help_uri() const;
    std::string                         icon() const;
    string_set_t                        categorization_tags() const;
    string_set_t                        dependencies() const;
    string_set_t                        conflicts() const;
    string_set_t                        suggestions() const;
    std::string                         settings_path() const;
    collection *                        plugins() const;

    virtual void                        bootstrap();
    virtual time_t                      do_update(time_t last_updated, unsigned int phase = 0);

private:
    friend class detail::repository;
    friend class collection;
    friend class factory;

    factory const *              f_factory = nullptr;
    names::filename_t            f_filename = std::string();
    collection *                 f_collection = nullptr;
};


#define SERVERPLUGINS_DEFAULTS(name) \
    typedef std::shared_ptr<name> pointer_t; \
    name(::serverplugins::factory const & factory); \
    name(name const &) = delete; \
    virtual ~name(); \
    name & operator = (name const &) = delete; \
    static pointer_t instance()





/** \brief Conditionally listen to a signal.
 *
 * This function checks whether a given plugin was loaded and if so
 * listen to one of its signals.
 *
 * The macro accepts the name of the listener plugin (it must be
 * 'this'), the name of the emitter plugin, and the name of the
 * signal to listen to.
 *
 * The listener must have a function named on_\<name of signal>.
 * The emitter is expected to define the signal using the
 * PLUGIN_SIGNAL() macro so the signal is called
 * signal_listen_\<name of signal>.
 *
 * \param[in] name  The name of the plugin connecting.
 * \param[in] emitter_name  The name of the plugin emitting this signal.
 * \param[in] emitter_class  The class with qualifiers if necessary of the plugin emitting this signal.
 * \param[in] signal  The name of the signal to listen to.
 * \param[in] args  The list of arguments to that signal.
 */
#define SERVERPLUGINS_LISTEN(name, emitter_name, emitter_class, signal, args...) \
    if(plugins()->is_loaded(emitter_name)) \
        emitter_class::instance()->signal_listen_##signal( \
                    boost::bind(&name::on_##signal, this, ##args));

#define SERVERPLUGINS_LISTEN0(name, emitter_name, emitter_class, signal) \
    if(plugins()->is_loaded(emitter_name)) \
        emitter_class::instance()->signal_listen_##signal( \
                    boost::bind(&name::on_##signal, this));


/** \brief Compute the number of days in the month of February.
 * \private
 *
 * The month of February is used to adjust the date by 1 day over leap
 * years. Years are leap years when multiple of 4, but not if multiple
 * of 100, except if it is also a multiple of 400.
 *
 * The computation of a leap year is documented on Wikipedia:
 * http://www.wikipedia.org/wiki/Leap_year
 *
 * \param[in] year  The year of the date to convert.
 *
 * \return 28 or 29 depending on whether the year is a leap year
 */
#define _SERVERPLUGINS_UNIX_TIMESTAMP_FDAY(year) \
    (((year) % 400) == 0 ? 29LL : \
        (((year) % 100) == 0 ? 28LL : \
            (((year) % 4) == 0 ? 29LL : \
                28LL)))


/** \brief Compute the number of days in a year.
 * \private
 *
 * This macro returns the number of day from the beginning of the
 * year the (year, month, day) value represents.
 *
 * \param[in] year  The 4 digits year concerned.
 * \param[in] month  The month (1 to 12).
 * \param[in] day  The day of the month (1 to 31)
 *
 * \return The number of days within that year (starting at 1)
 */
#define _SERVERPLUGINS_UNIX_TIMESTAMP_YDAY(year, month, day) \
    ( \
        /* January */    static_cast<std::int64_t>(day) \
        /* February */ + ((month) >=  2 ? 31LL : 0LL) \
        /* March */    + ((month) >=  3 ? _SERVERPLUGINS_UNIX_TIMESTAMP_FDAY(year) : 0LL) \
        /* April */    + ((month) >=  4 ? 31LL : 0LL) \
        /* May */      + ((month) >=  5 ? 30LL : 0LL) \
        /* June */     + ((month) >=  6 ? 31LL : 0LL) \
        /* July */     + ((month) >=  7 ? 30LL : 0LL) \
        /* August */   + ((month) >=  8 ? 31LL : 0LL) \
        /* September */+ ((month) >=  9 ? 31LL : 0LL) \
        /* October */  + ((month) >= 10 ? 30LL : 0LL) \
        /* November */ + ((month) >= 11 ? 31LL : 0LL) \
        /* December */ + ((month) >= 12 ? 30LL : 0LL) \
    )


/** \brief Compute a Unix date from a hard coded date.
 *
 * This macro is used to compute a Unix date from a date defined as 6 numbers:
 * year, month, day, hour, minute, second. Each number is expected to be an
 * integer although it could very well be an expression. The computation takes
 * the year and month in account to compute the year day which is used by
 * the do_update() functions.
 *
 * The year is expected to be written as a 4 digit number (1998, 2012, etc.)
 *
 * Each number is expected to represent a valid date. If a number is out of
 * range, then the date is still computed. It will just represent a valid
 * date, just not exactly what you wrote down.
 *
 * The math used in this macro comes from a FreeBSD implementation of mktime
 * (http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap04.html#tag_04_15)
 *
 * \code
 * tm_sec + tm_min*60 + tm_hour*3600 + tm_yday*86400 +
 *   (tm_year-70)*31536000 + ((tm_year-69)/4)*86400 -
 *   ((tm_year-1)/100)*86400 + ((tm_year+299)/400)*86400
 * \endcode
 *
 * Note that we expect the year to be 1970 and not 0, 2000 and not 30, etc.
 * For this reason our macro subtract values from the year that are different
 * from those shown in the FreeBSD sample code.
 *
 * Also the month must be a number from 1 to 12 and not 0 to 11 as used
 * in various Unix structures.
 *
 * \param[in] year  The year representing this Unix timestamp.
 * \param[in] month  The month representing this Unix timestamp.
 * \param[in] day  The day representing this Unix timestamp.
 * \param[in] hour  The hour representing this Unix timestamp.
 * \param[in] minute  The minute representing this Unix timestamp.
 * \param[in] second  The second representing this Unix timestamp.
 */
#define SERVERPLUGINS_UNIX_TIMESTAMP(year, month, day, hour, minute, second) \
    ( /* time */ static_cast<std::int64_t>(second) \
                + static_cast<std::int64_t>(minute) * 60LL \
                + static_cast<std::int64_t>(hour) * 3600LL \
    + /* year day (month + day) */ (_SERVERPLUGINS_UNIX_TIMESTAMP_YDAY(year, month, day) - 1) * 86400LL \
    + /* year */ (static_cast<std::int64_t>(year) - 1970LL) * 31536000LL \
                + ((static_cast<std::int64_t>(year) - 1969LL) / 4LL) * 86400LL \
                - ((static_cast<std::int64_t>(year) - 1901LL) / 100LL) * 86400LL \
                + ((static_cast<std::int64_t>(year) - 1601LL) / 400LL) * 86400LL )


/** \brief Initialize the do_update() function.
 *
 * This macro is used to initialize the do_update() function by creating a
 * variable that is going to be given a date.
 */
#define SERVERPLUGINS_PLUGIN_UPDATE_INIT() time_t last_plugin_update(SERVERPLUGINS_UNIX_TIMESTAMP(1990, 1, 1, 0, 0, 0));


/** \brief Create an update entry in your on_update() signal implementation.
 *
 * This macro is used to generate the necessary code to test the latest
 * update date and the date of the specified update.
 *
 * The function is called if the last time the website was updated it
 * was before this update. The function is then called with its own
 * date in micro-seconds (usec).
 *
 * \warning
 * The parameter to the on_update() function must be named last_updated for
 * this macro to compile as expected.
 *
 * \param[in] year  The year representing this Unix timestamp.
 * \param[in] month  The month representing this Unix timestamp.
 * \param[in] day  The day representing this Unix timestamp.
 * \param[in] hour  The hour representing this Unix timestamp.
 * \param[in] minute  The minute representing this Unix timestamp.
 * \param[in] second  The second representing this Unix timestamp.
 * \param[in] function  The name of the function to call if the update is required.
 */
#define SERVERPLUGINS_PLUGIN_UPDATE(year, month, day, hour, minute, second, function) \
    if(last_plugin_update > SERVERPLUGINS_UNIX_TIMESTAMP(year, month, day, hour, minute, second)) { \
        throw ::serverplugins::invalid_order("the updates in your do_update() functions must appear in increasing order in regard to date and time"); \
    } \
    last_plugin_update = SERVERPLUGINS_UNIX_TIMESTAMP(year, month, day, hour, minute, second); \
    if(last_updated < last_plugin_update) { \
        function(last_plugin_update); \
    }

/** \brief End the plugin update function.
 *
 * Use the macro as the very last line in your plugin do_update() function.
 * It returns the lastest update of your plugin.
 *
 * This function adds a return statement so anything appearing after it
 * will be ignored (not reached.)
 */
#define SERVERPLUGINS_PLUGIN_UPDATE_EXIT() return last_plugin_update;



} // namespace serverplugins
// vim: ts=4 sw=4 et
