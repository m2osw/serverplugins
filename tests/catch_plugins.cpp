// Copyright (c) 2006-2025  Made to Order Software Corp.  All Rights Reserved
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

// serverplugins
//
#include    <serverplugins/plugin.h>

#include    <serverplugins/collection.h>


// self
//
#include    "catch_main.h"

#include    "plugin_daemon.h"
#include    "plugin_testme.h"


// snapdev
//
#include    <snapdev/not_reached.h>


// C++
//
#include    <fstream>


// C
//
#include    <unistd.h>
#include    <sys/stat.h>
#include    <sys/types.h>


// last include
//
#include    <snapdev/poison.h>



CATCH_TEST_CASE("paths", "[plugins][paths]")
{
    CATCH_START_SECTION("paths: empty size/at when empty")
    {
        serverplugins::paths p;

        CATCH_REQUIRE(p.size() == 0);

        for(int idx(-10); idx <= 10; ++idx)
        {
            CATCH_REQUIRE(p.at(idx) == std::string());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("paths: canonicalize empty path")
    {
        serverplugins::paths p;

        CATCH_REQUIRE_FALSE(p.get_allow_redirects());
        CATCH_REQUIRE_THROWS_MATCHES(
                  p.canonicalize(serverplugins::paths::path_t())
                , serverplugins::invalid_error
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: path cannot be an empty string."));

        p.set_allow_redirects(true);
        CATCH_REQUIRE(p.get_allow_redirects());
        CATCH_REQUIRE_THROWS_MATCHES(
                  p.canonicalize(serverplugins::paths::path_t())
                , serverplugins::invalid_error
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: path cannot be an empty string."));

        p.set_allow_redirects(false);
        CATCH_REQUIRE_FALSE(p.get_allow_redirects());
        CATCH_REQUIRE_THROWS_MATCHES(
                  p.canonicalize(serverplugins::paths::path_t())
                , serverplugins::invalid_error
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: path cannot be an empty string."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("paths: canonicalize base root path")
    {
        for(int idx(1); idx <= 10; ++idx)
        {
            serverplugins::paths p;

            serverplugins::paths::path_t root(idx, '/');
            CATCH_REQUIRE(p.canonicalize(root) == "/");
            p.set_allow_redirects(true);
            CATCH_REQUIRE(p.canonicalize(root) == "/");
            p.set_allow_redirects(false);
            CATCH_REQUIRE(p.canonicalize(root) == "/");
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("paths: canonicalize root path with one \"..\"")
    {
        std::vector<serverplugins::paths::path_t> paths =
        {
            "this",
            "long",
            "..",
            "short",
            "root",
            "path",
        };

        for(serverplugins::paths::path_t::size_type l(1); l < paths.size(); ++l)
        {
            serverplugins::paths p;
            serverplugins::paths::path_t expected;
            int count(rand() % 10 + 1);
            serverplugins::paths::path_t path(count , '/');
            for(serverplugins::paths::path_t::size_type c(0); c < l; ++c)
            {
                if(paths[c] != ".."
                && (c + 1 >= l || paths[c + 1] != ".."))
                {
                    expected += '/';
                    expected += paths[c];
                }

                path += paths[c];

                count = rand() % 10 + 1;
                serverplugins::paths::path_t sep(count , '/');
                path += sep;
            }

//std::cerr << "+++ expected = [" << expected << "] and path [" << path << "]\n";

            // verify that we got expected as a canonicalize path
            //
            CATCH_CHECK(p.canonicalize(expected) == expected);
            CATCH_REQUIRE(p.canonicalize(path) == expected);

            p.set_allow_redirects(true);
            CATCH_CHECK(p.canonicalize(expected) == expected);
            CATCH_REQUIRE(p.canonicalize(path) == expected);

            p.set_allow_redirects(false);
            CATCH_CHECK(p.canonicalize(expected) == expected);
            CATCH_REQUIRE(p.canonicalize(path) == expected);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("paths: canonicalize root path with too many \"..\"")
    {
        serverplugins::paths p;

        p.set_allow_redirects(false);
        CATCH_REQUIRE(p.canonicalize("/this/long/../../../..//") == "/");
        p.set_allow_redirects(true);
        CATCH_REQUIRE(p.canonicalize("/this/long/../../../..//") == "/");

        p.set_allow_redirects(false);
        CATCH_REQUIRE(p.canonicalize("/this//long/../../../../root/home/path/") == "/root/home/path");
        p.set_allow_redirects(true);
        CATCH_REQUIRE(p.canonicalize("/this//long/../../../../root/home/path/") == "/root/home/path");
        p.set_allow_redirects(false);
        CATCH_REQUIRE(p.canonicalize("/this/long/../..//./../root/home/path/") == "/root/home/path");
        p.set_allow_redirects(true);
        CATCH_REQUIRE(p.canonicalize("/this/long/../..//./../root/home/path/") == "/root/home/path");
        p.set_allow_redirects(false);
        CATCH_REQUIRE(p.canonicalize("/this/long/.././../../root//home/path/") == "/root/home/path");
        p.set_allow_redirects(true);
        CATCH_REQUIRE(p.canonicalize("/this/long/.././../../root//home/path/") == "/root/home/path");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("paths: canonicalize relative path with \".\" and \"..\"")
    {
        serverplugins::paths p;

        CATCH_REQUIRE(p.canonicalize("this/./relative/./angle/.././path//cleaned/up") == "this/relative/path/cleaned/up");
        p.set_allow_redirects(true);
        CATCH_REQUIRE(p.canonicalize("this/./relative/./angle/.././path//cleaned/up") == "this/relative/path/cleaned/up");
        p.set_allow_redirects(false);
        CATCH_REQUIRE(p.canonicalize("this/./relative/./angle/.././path//cleaned/up") == "this/relative/path/cleaned/up");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("paths: canonicalize relative path with too many \"..\"")
    {
        serverplugins::paths p;

        p.set_allow_redirects(true);
        CATCH_REQUIRE(p.canonicalize("this/long/../../../..//") == "../..");
        p.set_allow_redirects(false);
        CATCH_REQUIRE_THROWS_MATCHES(
                  p.canonicalize("this/long/../../../..//")
                , serverplugins::invalid_error
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: the path \"this/long/../../../..//\" going outside of the allowed range."));

        p.set_allow_redirects(true);
        CATCH_REQUIRE(p.canonicalize("this//long/../../../../root/home/path/") == "../../root/home/path");
        p.set_allow_redirects(false);
        CATCH_REQUIRE_THROWS_MATCHES(
                  p.canonicalize("this//long/../../../../root/home/path/")
                , serverplugins::invalid_error
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: the path \"this//long/../../../../root/home/path/\" going outside of the allowed range."));

        p.set_allow_redirects(true);
        CATCH_REQUIRE(p.canonicalize("this/long/..//./../../root/home/path/") == "../root/home/path");
        p.set_allow_redirects(false);
        CATCH_REQUIRE_THROWS_MATCHES(
                  p.canonicalize("this/long/..//./../../root/home/path/")
                , serverplugins::invalid_error
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: the path \"this/long/..//./../../root/home/path/\" going outside of the allowed range."));

        p.set_allow_redirects(true);
        CATCH_REQUIRE(p.canonicalize("this/long/../.././../root//home//path//") == "../root/home/path");
        p.set_allow_redirects(false);
        CATCH_REQUIRE_THROWS_MATCHES(
                  p.canonicalize("this/long/../.././../root//home//path//")
                , serverplugins::invalid_error
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: the path \"this/long/../.././../root//home//path//\" going outside of the allowed range."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("paths: push the same paths")
    {
        serverplugins::paths p;

        p.push("path/one");
        p.push("path/two");
        p.push("path/three");
        p.push("path/two");
        p.push("path/one");

        CATCH_REQUIRE(p.size() == 3);
        CATCH_REQUIRE(p.at(0) == "path/one");
        CATCH_REQUIRE(p.at(1) == "path/two");
        CATCH_REQUIRE(p.at(2) == "path/three");
        CATCH_REQUIRE(p.at(3) == "");

        p.erase("path/four");

        CATCH_REQUIRE(p.size() == 3);
        CATCH_REQUIRE(p.at(0) == "path/one");
        CATCH_REQUIRE(p.at(1) == "path/two");
        CATCH_REQUIRE(p.at(2) == "path/three");
        CATCH_REQUIRE(p.at(3) == "");

        p.erase("path/two");

        CATCH_REQUIRE(p.size() == 2);
        CATCH_REQUIRE(p.at(0) == "path/one");
        CATCH_REQUIRE(p.at(1) == "path/three");
        CATCH_REQUIRE(p.at(2) == "");
        CATCH_REQUIRE(p.at(3) == "");

        p.erase("path/one");

        CATCH_REQUIRE(p.size() == 1);
        CATCH_REQUIRE(p.at(0) == "path/three");
        CATCH_REQUIRE(p.at(1) == "");
        CATCH_REQUIRE(p.at(2) == "");
        CATCH_REQUIRE(p.at(3) == "");

        p.erase("path/three");

        CATCH_REQUIRE(p.size() == 0);
        CATCH_REQUIRE(p.at(0) == "");
        CATCH_REQUIRE(p.at(1) == "");
        CATCH_REQUIRE(p.at(2) == "");
        CATCH_REQUIRE(p.at(3) == "");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("paths: push invalid path")
    {
        serverplugins::paths p;

        CATCH_REQUIRE_THROWS_MATCHES(
                  p.canonicalize("this/long/../.././../root//home//path//")
                , serverplugins::invalid_error
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: the path \"this/long/../.././../root//home//path//\" going outside of the allowed range."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("paths: add paths")
    {
        serverplugins::paths p;

        p.set_allow_redirects(true);
        p.add("this/long/../../../..//"
             ":this//long/../../../../root/home/path/"
             ":this/long/..//./../../root/home/path/"
             ":this/long/../.././..//home/user/path//");
        CATCH_REQUIRE(p.size() == 4);
        CATCH_REQUIRE(p.at(0) == "../..");
        CATCH_REQUIRE(p.at(1) == "../../root/home/path");
        CATCH_REQUIRE(p.at(2) == "../root/home/path");
        CATCH_REQUIRE(p.at(3) == "../home/user/path");
    }
    CATCH_END_SECTION()
}



CATCH_TEST_CASE("names", "[plugins][names]")
{
    CATCH_START_SECTION("names: empty by default")
    {
        serverplugins::paths p;
        p.add("/usr/local/lib/snaplogger/plugins:/usr/lib/snaplogger/plugins");

        serverplugins::names n(p);

        CATCH_REQUIRE(n.map().empty());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("names: validate 'names'")
    {
        serverplugins::paths p;
        p.add("/usr/local/lib/snaplogger/plugins:/usr/lib/snaplogger/plugins");

        serverplugins::names n(p);

        CATCH_CHECK(n.validate("_"));
        CATCH_CHECK(n.validate("_valid"));
        CATCH_CHECK(n.validate("_identifier6"));
        CATCH_CHECK(n.validate("_9"));

        CATCH_CHECK_FALSE(n.validate(""));
        CATCH_CHECK_FALSE(n.validate(serverplugins::names::name_t()));
        CATCH_CHECK_FALSE(n.validate("0"));
        CATCH_CHECK_FALSE(n.validate("9_"));
        CATCH_CHECK_FALSE(n.validate("dotted.word"));
        CATCH_CHECK_FALSE(n.validate(".dot"));
        CATCH_CHECK_FALSE(n.validate("dashed-word"));
        CATCH_CHECK_FALSE(n.validate("-dash"));

        for(int c('a'); c <= 'z'; ++c)
        {
            std::string word;
            word += c;
            CATCH_CHECK(n.validate(word));
        }

        for(int c('A'); c <= 'Z'; ++c)
        {
            std::string word;
            word += c;
            CATCH_CHECK(n.validate(word));
        }

        for(int c('0'); c <= '9'; ++c)
        {
            std::string word;
            word += '_';
            word += c;
            CATCH_CHECK(n.validate(word));
        }

        for(int c(1); c <= 0x7F; ++c)
        {
            if(c == '_'
            || (c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9'))
            {
                continue;
            }
            std::string word;
            word += '_';
            word += c;
            CATCH_CHECK_FALSE(n.validate(word));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("names: validate non-script names")
    {
        serverplugins::paths p;
        p.add("/usr/local/lib/snaplogger/plugins:/usr/lib/snaplogger/plugins");

        serverplugins::names n(p, true);

        CATCH_CHECK(n.validate("_"));
        CATCH_CHECK(n.validate("_valid"));
        CATCH_CHECK(n.validate("_identifier6"));
        CATCH_CHECK(n.validate("_9"));

        CATCH_CHECK_FALSE(n.validate(""));
        CATCH_CHECK_FALSE(n.validate(serverplugins::names::name_t()));
        CATCH_CHECK_FALSE(n.validate("0"));
        CATCH_CHECK_FALSE(n.validate("9_"));
        CATCH_CHECK_FALSE(n.validate("dotted.word"));
        CATCH_CHECK_FALSE(n.validate(".dot"));
        CATCH_CHECK_FALSE(n.validate("dashed-word"));
        CATCH_CHECK_FALSE(n.validate("-dash"));

        for(int c('a'); c <= 'z'; ++c)
        {
            std::string word;
            word += c;
            CATCH_CHECK(n.validate(word));
        }

        for(int c('A'); c <= 'Z'; ++c)
        {
            std::string word;
            word += c;
            CATCH_CHECK(n.validate(word));
        }

        for(int c('0'); c <= '9'; ++c)
        {
            std::string word;
            word += '_';
            word += c;
            CATCH_CHECK(n.validate(word));
        }

        for(int c(1); c <= 0x7F; ++c)
        {
            if(c == '_'
            || (c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9'))
            {
                continue;
            }
            std::string word;
            word += '_';
            word += c;
            CATCH_CHECK_FALSE(n.validate(word));
        }

        std::vector<std::string> const reserved_keywords = {
              "await"
            , "break"
            , "case"
            , "catch"
            , "class"
            , "const"
            , "continue"
            , "debugger"
            , "default"
            , "delete"
            , "do"
            , "else"
            , "enum"
            , "export"
            , "extends"
            , "false"
            , "finally"
            , "for"
            , "function"
            , "if"
            , "import"
            , "in"
            , "instanceof"
            , "new"
            , "null"
            , "return"
            , "super"
            , "switch"
            , "this"
            , "throw"
            , "true"
            , "try"
            , "typeof"
            , "var"
            , "void"
            , "while"
            , "with"
            , "yield"
        };
        for(auto k : reserved_keywords)
        {
            CATCH_CHECK_FALSE(n.validate(k));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("names: add one unknown and one known name")
    {
        serverplugins::paths p;
        p.add(CMAKE_BINARY_DIR "/tests:/usr/local/lib/snaplogger/plugins:/usr/lib/snaplogger/plugins");

        serverplugins::names n(p, true);

        // test with an obviously unexistant plugin
        //
        serverplugins::names::filename_t filename(n.to_filename("unknown"));
        CATCH_REQUIRE(filename.empty());

        // test with the real thing
        //
        filename = n.to_filename("testme");
        CATCH_REQUIRE(filename == CMAKE_BINARY_DIR "/tests/libtestme.so");

        // the following creates fake plugins so we can test all possible
        // cases (there are 4 of them)
        //
        {       // <name>.so
            std::string fake(CMAKE_BINARY_DIR "/tests/fake.so");
            unlink(fake.c_str());
            {
                std::ofstream out(fake);
                out << "fake plugin 1\n";
            }
            filename = n.to_filename("fake");
            CATCH_REQUIRE(filename == fake);
            unlink(fake.c_str());
        }
        {       // lib<name>.so
            std::string fake(CMAKE_BINARY_DIR "/tests/libfake.so");
            unlink(fake.c_str());
            {
                std::ofstream out(fake);
                out << "fake plugin 2\n";
            }
            filename = n.to_filename("fake");
            CATCH_REQUIRE(filename == fake);
            unlink(fake.c_str());
        }
        {       // <name>/<name>.so
            std::string subdir(CMAKE_BINARY_DIR "/tests/fake");
            mkdir(subdir.c_str(), 0750);
            {
                std::string fake(CMAKE_BINARY_DIR "/tests/fake/fake.so");
                unlink(fake.c_str());
                {
                    std::ofstream out(fake);
                    out << "fake plugin 3\n";
                }
                filename = n.to_filename("fake");
                CATCH_REQUIRE(filename == fake);
                unlink(fake.c_str());
            }
            {
                std::string fake(CMAKE_BINARY_DIR "/tests/fake/libfake.so");
                unlink(fake.c_str());
                {
                    std::ofstream out(fake);
                    out << "fake plugin 4\n";
                }
                filename = n.to_filename("fake");
                CATCH_REQUIRE(filename == fake);
                unlink(fake.c_str());
            }
            rmdir(subdir.c_str());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("names: test filename without any paths")
    {
        serverplugins::paths p;
        serverplugins::names n(p);

        // test with an obviously unexistant plugin
        //
        serverplugins::names::filename_t filename(n.to_filename("unknown"));
        CATCH_REQUIRE(filename.empty());

        // test with the real thing
        //
        filename = n.to_filename("testme");
        CATCH_REQUIRE(filename.empty());

        // the following creates fake plugins so we can test all possible
        // cases (there are 4 of them)
        //
        {       // <name>.so
            std::string fake("fake.so");
            unlink(fake.c_str());
            {
                std::ofstream out(fake);
                out << "fake plugin 1\n";
            }
            filename = n.to_filename("fake");
            CATCH_REQUIRE(filename == "./" + fake);
            unlink(fake.c_str());
        }
        {       // lib<name>.so
            std::string fake("libfake.so");
            unlink(fake.c_str());
            {
                std::ofstream out(fake);
                out << "fake plugin 2\n";
            }
            filename = n.to_filename("fake");
            CATCH_REQUIRE(filename == "./" + fake);
            unlink(fake.c_str());
        }
        {       // <name>/<name>.so
            std::string subdir("fake");
            mkdir(subdir.c_str(), 0750);
            {
                std::string fake("fake/fake.so");
                unlink(fake.c_str());
                {
                    std::ofstream out(fake);
                    out << "fake plugin 3\n";
                }
                filename = n.to_filename("fake");
                CATCH_REQUIRE(filename == "./" + fake);
                unlink(fake.c_str());
            }
            {
                std::string fake("fake/libfake.so");
                unlink(fake.c_str());
                {
                    std::ofstream out(fake);
                    out << "fake plugin 4\n";
                }
                filename = n.to_filename("fake");
                CATCH_REQUIRE(filename == "./" + fake);
                unlink(fake.c_str());
            }
            rmdir(subdir.c_str());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("names: add a new 'names' by hand (push)")
    {
        serverplugins::paths p;
        p.add(CMAKE_BINARY_DIR "/tests:/usr/local/lib/snaplogger/plugins:/usr/lib/snaplogger/plugins");
        serverplugins::names n(p);

        // test with an obviously unexistant plugin
        //
        n.push("testme");
        CATCH_REQUIRE(n.map().size() == 1);
        CATCH_REQUIRE(n.map().begin()->second == CMAKE_BINARY_DIR "/tests/libtestme.so");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("names: add a new 'names' by hand (add)")
    {
        serverplugins::paths p;
        p.add(CMAKE_BINARY_DIR "/tests:/usr/local/lib/snaplogger/plugins:/usr/lib/snaplogger/plugins");
        serverplugins::names n(p);

        // TODO: look into having multiple plugins because that would allow
        //       us to test ':'
        //
        n.add("testme");
        CATCH_REQUIRE(n.map().size() == 1);
        CATCH_REQUIRE(n.map().begin()->second == CMAKE_BINARY_DIR "/tests/libtestme.so");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("names: add a 'names' through the find_plugins() function")
    {
        serverplugins::paths p;
        p.add(CMAKE_BINARY_DIR "/tests:/usr/local/lib/snaplogger/plugins:/usr/lib/snaplogger/plugins");
        serverplugins::names n(p);
        n.find_plugins();
        CATCH_REQUIRE(n.map().size() == 1);
        CATCH_REQUIRE(n.map().begin()->second == CMAKE_BINARY_DIR "/tests/libtestme.so");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("names: add invalid 'names'")
    {
        serverplugins::paths p;
        p.add(CMAKE_BINARY_DIR "/tests:/usr/local/lib/snaplogger/plugins:/usr/lib/snaplogger/plugins");
        serverplugins::names n(p);

        CATCH_REQUIRE_THROWS_MATCHES(
                  n.push("invalid-name")
                , serverplugins::invalid_error
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: invalid plugin name in \"invalid-name\"."));

        CATCH_REQUIRE_THROWS_MATCHES(
                  n.push("non_existant")
                , serverplugins::not_found
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: plugin named \"non_existant\" not found in any of the specified paths."));

        CATCH_REQUIRE_THROWS_MATCHES(
                  n.push("./libserver.so")
                , serverplugins::invalid_error
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: the name \"server\" is reserved for the main running process."));

        CATCH_REQUIRE_THROWS_MATCHES(
                  n.push("./libjuju1.23.so")
                , serverplugins::invalid_error
                , Catch::Matchers::ExceptionMessage(
                          "serverplugins_exception: invalid plugin name in \"juju1.23\" (from path \"./libjuju1.23.so\")."));
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("collection", "[plugins][collection]")
{
    CATCH_START_SECTION("collection: load the plugin")
    {
        char const * argv[] = { "/usr/sbin/daemon", nullptr };
        optional_namespace::daemon::pointer_t d(std::make_shared<optional_namespace::daemon>(1, const_cast<char **>(argv)));

        serverplugins::paths p;
        p.add(CMAKE_BINARY_DIR "/tests:/usr/local/lib/snaplogger/plugins:/usr/lib/snaplogger/plugins");
        serverplugins::names n(p);
        n.find_plugins();
        serverplugins::collection c(n);
        bool const loaded(c.load_plugins(d));
        CATCH_REQUIRE(loaded);
        optional_namespace::testme::pointer_t r(c.get_plugin_by_name<optional_namespace::testme>("testme"));
        CATCH_REQUIRE(r != nullptr);

        serverplugins::version_t const v(r->version());
        CATCH_CHECK(v.f_major == 5);
        CATCH_CHECK(v.f_minor == 3);
        CATCH_CHECK(v.f_patch == 0);

        //time_t m(last_modification());

        CATCH_REQUIRE(r->name() == "testme");
        CATCH_REQUIRE(r->filename() == CMAKE_BINARY_DIR "/tests/libtestme.so");
        CATCH_REQUIRE(r->description() == "a test plugin to make sure it all works.");
        CATCH_REQUIRE(r->help_uri() == "https://snapwebsites.org/");
        CATCH_REQUIRE(r->icon() == "cute.ico");

        serverplugins::string_set_t tags(r->categorization_tags());
        CATCH_REQUIRE(tags.size() == 3);
        CATCH_REQUIRE(tags.find("test") != tags.end());
        CATCH_REQUIRE(tags.find("powerful") != tags.end());
        CATCH_REQUIRE(tags.find("software") != tags.end());
        CATCH_REQUIRE(tags.find("undefined") == tags.end());

        // at this time we have a single plugins so not dependencies
        //
        serverplugins::string_set_t dependencies(r->dependencies());
        CATCH_REQUIRE(dependencies.empty());

        serverplugins::string_set_t conflicts(r->conflicts());
        CATCH_REQUIRE(conflicts.size() == 3);
        CATCH_REQUIRE(conflicts.find("other_test") != conflicts.end());
        CATCH_REQUIRE(conflicts.find("power_test") != conflicts.end());
        CATCH_REQUIRE(conflicts.find("unknown") != conflicts.end());
        CATCH_REQUIRE(conflicts.find("undefined") == conflicts.end());

        serverplugins::string_set_t suggestions(r->suggestions());

        // WARNING: for it_worked() to compile, it needs to be a virtual
        //
        std::string const msg(r->it_worked());
        CATCH_CHECK(msg == "testme:plugin: it worked, it was called!");
    }
    CATCH_END_SECTION()
}




// vim: ts=4 sw=4 et
