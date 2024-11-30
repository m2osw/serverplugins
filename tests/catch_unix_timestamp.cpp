// Copyright (c) 2012-2023  Made to Order Software Corp.  All Rights Reserved
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

#pragma GCC diagnostic ignored "-Wformat"

// load the macros
//
#include    <serverplugins/plugin.h>


// self
//
#include    "catch_main.h"


// snapdev
//
#include    <snapdev/not_used.h>


// C++
//
#include    <iostream>


// C
//
#include    <time.h>


CATCH_TEST_CASE("unix_timestamp", "[date]")
{
    struct tm t;
    t.tm_sec = 0;
    t.tm_min = 0;
    t.tm_hour = 0;
    t.tm_mday = 1;
    t.tm_mon = 0;
    t.tm_year = 70;
    t.tm_wday = 0;
    t.tm_yday = 0;
    t.tm_isdst = -1;
    time_t localtime_timestamp = mktime(&t);

    CATCH_START_SECTION("unix_timestamp: months to seconds for one year")
    {
        for(int month = 1; month <= 12; ++month) {
            for(int day = 1; day <= 31; ++day) {
                for(int hour = 0; hour < 24; ++hour) {
                    for(int minute = 0; minute < 60; ++minute) {
                        for(int second = 0; second < 60; ++second) {
                            time_t our_timestamp = SERVERPLUGINS_UNIX_TIMESTAMP(1970, month, day, hour, minute, second);
                            struct tm t2;
                            t2.tm_sec = second;
                            t2.tm_min = minute;
                            t2.tm_hour = hour;
                            t2.tm_mday = day;
                            t2.tm_mon = month - 1;
                            t2.tm_year = 1970 - 1900;
                            t2.tm_wday = 0;
                            t2.tm_yday = 0;
                            t2.tm_isdst = 0;
                            time_t mktime_timestamp = mktime(&t2) - localtime_timestamp;
                            if(our_timestamp != mktime_timestamp)
                            {
                                std::cerr
                                    << "error: invalid conversion with "
                                    << 1970
                                    << "-"
                                    << month
                                    << "-"
                                    << day 
                                    << " "
                                    << hour
                                    << ":"
                                    << minute
                                    << ":"
                                    << second
                                    << " -> "
                                    << our_timestamp
                                    << " != "
                                    << mktime_timestamp
                                    << " (diff "
                                    << our_timestamp - mktime_timestamp
                                    << ")\n";
                            }
                            CATCH_REQUIRE(our_timestamp == mktime_timestamp);
                        }
                    }
                }
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("unix_timestamp: years to days at midnight")
    {
        for(int year = 1970; year < 2068; ++year)
        {
            for(int month = 1; month <= 12; ++month)
            {
                for(int day = 1; day <= 31; ++day)
                {
                    time_t our_timestamp = SERVERPLUGINS_UNIX_TIMESTAMP(year, month, day, 0, 0, 0);
                    struct tm t3;
                    t3.tm_sec = 0;
                    t3.tm_min = 0;
                    t3.tm_hour = 0;
                    t3.tm_mday = day;
                    t3.tm_mon = month - 1;
                    t3.tm_year = year - 1900;
                    t3.tm_wday = 0;
                    t3.tm_yday = 0;
                    t3.tm_isdst = 0;
                    time_t mktime_timestamp = mktime(&t3) - localtime_timestamp;
                    if(our_timestamp != mktime_timestamp)
                    {
                        std::cerr
                            << "error: invalid conversion with "
                            << year
                            << "-"
                            << month
                            << "-"
                            << day 
                            << " 00:00:00 -> "
                            << our_timestamp
                            << " != "
                            << mktime_timestamp
                            << " (diff "
                            << our_timestamp - mktime_timestamp
                            << ")\n";
                    }
                    CATCH_REQUIRE(our_timestamp == mktime_timestamp);
                }
            }
        }
    }
    CATCH_END_SECTION()
}

// vim: ts=4 sw=4 et
