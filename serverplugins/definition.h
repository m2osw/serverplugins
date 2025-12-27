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
#include    <serverplugins/utils.h>
#include    <serverplugins/version.h>       // used in SERVERPLUGINS_START()


// snapdev
//
#include    <snapdev/not_used.h>


// C++ set
//
#include    <set>



namespace serverplugins
{



typedef std::set<std::string>           string_set_t;



struct definition
{
    version_t                           f_version = version_t();
    version_t                           f_library_version = version_t();
    time_t                              f_last_modification = 0;        // uses the compilation date & time converted to a Unix date
    std::string                         f_name = std::string();
    std::string                         f_description = std::string();
    std::string                         f_help_uri = std::string();
    std::string                         f_icon = std::string();
    string_set_t                        f_categorization_tags = string_set_t();
    string_set_t                        f_dependencies = string_set_t();
    string_set_t                        f_conflicts = string_set_t();
    string_set_t                        f_suggestions = string_set_t();
    std::string                         f_settings_path = std::string();
};





template<typename T>
class definition_value
{
public:
    typedef T   value_t;

    constexpr definition_value(T const v)
        : f_value(v)
    {
    }

    constexpr value_t get() const
    {
        return f_value;
    }

private:
    value_t     f_value = value_t();
};




class plugin_version
    : public definition_value<version_t>
{
public:
    constexpr plugin_version()
        : definition_value<version_t>(version_t())
    {
    }

    constexpr plugin_version(version_t version)
        : definition_value<version_t>(version)
    {
    }
};


class library_version
    : public definition_value<version_t>
{
public:
    constexpr library_version()
        : definition_value<version_t>(version_t())
    {
    }

    constexpr library_version(version_t version)
        : definition_value<version_t>(version)
    {
    }
};


class last_modification
    : public definition_value<time_t>
{
public:
    constexpr last_modification()
        : definition_value<time_t>(time_t())
    {
    }

    constexpr last_modification(time_t const t)
        : definition_value<time_t>(validate_date(t))
    {
    }
};


class plugin_name
    : public definition_value<char const *>
{
public:
    constexpr plugin_name()
        : definition_value<char const *>(nullptr)
    {
    }

    template<int N>
    constexpr plugin_name(char const (&name)[N])
        : definition_value<char const *>(validate_name(name))
    {
    }
};


class description
    : public definition_value<char const *>
{
public:
    constexpr description()
        : definition_value<char const *>("")
    {
    }

    constexpr description(char const * d)
        : definition_value<char const *>(d)
    {
    }
};


class help_uri
    : public definition_value<char const *>
{
public:
    constexpr help_uri()
        : definition_value<char const *>("")
    {
    }

    constexpr help_uri(char const * uri)
        : definition_value<char const *>(uri)
    {
    }
};


class icon
    : public definition_value<char const *>
{
public:
    constexpr icon()
        : definition_value<char const *>("")
    {
    }

    constexpr icon(char const * name)
        : definition_value<char const *>(name)
    {
    }
};


class categorization_tag
    : public definition_value<char const *>
{
public:
    template<int N>
    constexpr categorization_tag(char const (&tag)[N])
        : definition_value<char const *>(validate_name(tag))
    {
    }
};


class dependency
    : public definition_value<char const *>
{
public:
    template<int N>
    constexpr dependency(char const (&dep)[N])
        : definition_value<char const *>(validate_name(dep))
    {
    }
};


class conflict
    : public definition_value<char const *>
{
public:
    template<int N>
    constexpr conflict(char const (&c)[N])
        : definition_value<char const *>(validate_name(c))
    {
    }
};


class suggestion
    : public definition_value<char const *>
{
public:
    template<int N>
    constexpr suggestion(char const (&s)[N])
        : definition_value<char const *>(validate_name(s))
    {
    }
};


class settings_path
    : public definition_value<char const *>
{
public:
    constexpr settings_path()
        : definition_value<char const *>("")
    {
    }

    template<int N>
    constexpr settings_path(char const (&path)[N])
        : definition_value<char const *>(validate_name(path))
    {
    }
};






template<typename T, typename F, class ...ARGS>
constexpr typename std::enable_if<std::is_same<T, F>::value, typename T::value_t>::type find_plugin_information(F first, ARGS ...args)
{
    snapdev::NOT_USED(args...);
    return first.get();
}


template<typename T, typename F, class ...ARGS>
constexpr typename std::enable_if<!std::is_same<T, F>::value, typename T::value_t>::type find_plugin_information(F first, ARGS ...args)
{
    snapdev::NOT_USED(first);
    return find_plugin_information<T>(args...);
}





template<typename T, typename F>
typename std::enable_if<std::is_same<T, F>::value, string_set_t>::type find_plugin_set(F first)
{
    string_set_t s;
    s.insert(first.get());
    return s;
}


template<typename T, typename F>
typename std::enable_if<!std::is_same<T, F>::value, string_set_t>::type find_plugin_set(F first)
{
    snapdev::NOT_USED(first);
    return string_set_t();
}


template<typename T, typename F, class ...ARGS>
typename std::enable_if<std::is_same<T, F>::value && (sizeof...(ARGS) > 0), string_set_t>::type find_plugin_set(F first, ARGS ...args)
{
    string_set_t s(find_plugin_set<T>(args...));
    s.insert(first.get());
    return s;
}


template<typename T, typename F, class ...ARGS>
typename std::enable_if<!std::is_same<T, F>::value && (sizeof...(ARGS) > 0), string_set_t>::type find_plugin_set(F first, ARGS ...args)
{
    snapdev::NOT_USED(first);
    return find_plugin_set<T>(args...);
}



template<class ...ARGS>
constexpr definition define_plugin(ARGS ...args)
{
    definition def =
    {
        .f_version =                find_plugin_information<plugin_version>(args...),        // no default, must be defined
        .f_library_version =        find_plugin_information<library_version>(args...),       // no default, must be defined
        .f_last_modification =      find_plugin_information<last_modification>(args...),     // no default, must be defined
        .f_name =                   find_plugin_information<plugin_name>(args...),                  // no default, must be defined
        .f_description =            find_plugin_information<description>(args..., description()),
        .f_help_uri =               find_plugin_information<help_uri>(args..., help_uri()),
        .f_icon =                   find_plugin_information<icon>(args..., icon()),
        .f_categorization_tags =    find_plugin_set<categorization_tag>(args...),
        .f_dependencies =           find_plugin_set<dependency>(args...),
        .f_conflicts =              find_plugin_set<conflict>(args...),
        .f_suggestions =            find_plugin_set<suggestion>(args...),
        .f_settings_path =          find_plugin_information<settings_path>(args..., settings_path()),
    };

    // TODO: add verifications to make sure parameters are consistent

    return def;
}


#define SERVERPLUGINS_VERSION(name, major, minor) \
    constexpr ::serverplugins::version_t::number_t g_##name##_version_major = major; \
    constexpr ::serverplugins::version_t::number_t g_##name##_version_minor = minor;


// helper macros to create a plugin definition structure
//
#define SERVERPLUGINS_START(name) \
    ::serverplugins::definition const g_##name##_definition = ::serverplugins::define_plugin( \
          ::serverplugins::plugin_version(::serverplugins::version_t(g_##name##_version_major, g_##name##_version_minor, 0)) \
        , ::serverplugins::library_version(::serverplugins::version_t(SERVERPLUGINS_VERSION_MAJOR, SERVERPLUGINS_VERSION_MINOR, SERVERPLUGINS_VERSION_PATCH)) \
        , ::serverplugins::last_modification(UTC_BUILD_TIME_STAMP) \
        , ::serverplugins::plugin_name(#name)


#define SERVERPLUGINS_END(name) \
    ); \
    class plugin_##name##_factory : public ::serverplugins::factory { \
    public: plugin_##name##_factory() \
        : factory(g_##name##_definition, std::make_shared<name>(*this)) \
        { register_plugin(#name, get_plugin()); } \
    plugin_##name##_factory(plugin_##name##_factory const &) = delete; \
    plugin_##name##_factory & operator = (plugin_##name##_factory const &) = delete; \
    } g_##name##_factory; \
    name::name(::serverplugins::factory const & f) : plugin(f) {} \
    name::~name() {}

    //name::pointer_t name::instance() { return std::static_pointer_cast<name>(g_##name##_factory.instance()); }


// the following are very similar, but used to initialize the definitions
// of a server, which is also a plugin, but we have a chicken and the egg
// issue where we cannot have the factory and create the server "at the
// same time" because we do not know what the server constructor will
// requireo
//
// this also means you need to _manually_ pass the factory to the server()
// constructor and later save the server shared pointer in the factory
// so that way the plugin is properly initialized
//
#define SERVERPLUGINS_START_SERVER(name) SERVERPLUGINS_START(name)


#define SERVERPLUGINS_END_SERVER(name) \
    ); \
    class server_##name##_factory : public ::serverplugins::factory { \
    public: server_##name##_factory() \
        : factory(g_##name##_definition, nullptr) {} \
    server_##name##_factory(server_##name##_factory const &) = delete; \
    server_##name##_factory & operator = (server_##name##_factory const &) = delete; \
    } g_##name##_factory;



} // namespace serverplugins
// vim: ts=4 sw=4 et
