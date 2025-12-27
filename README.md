
<p align="center">
<img alt="serverplugins" title="C++ library to easily manage thread and thread pools including communication and mutexes."
src="https://raw.githubusercontent.com/m2osw/serverplugins/master/doc/serverplugins.png" width="191" height="248"/>
</p>

# Introduction

The serverplugins started based on the functionality offered by the
`snap_plugins.cpp/.h` from the libsnapwebsites.

It includes support for one or more collections of plugins. One plugin can
be shared between multiple collections.


# Features

The library offers a very easy way to load .so files at runtime to extend
your servers and tools without having to recompile the entire project each
time. Only the plugin can be updated, reload the service and voila! new
functionality is instantly available.


# Exceptions

The library includes its own set of exceptions. Note that exceptions can be
used in a plugin just like any other function in your software.


# Example Usage

## Create a Server

The server is the one that loads the plugins on startup and in most cases
also the object that emits signals.

This class must derive the serverplugins `server` class and we generally
define the version in the header file:

    SERVERPLUGINS_VERSION(my_server, 1, 0)

    class my_server
        : public serverplugins::server
    {
    ...
    };

**Note:** the base class of the `serverplugins::server` is
          `serverplugins::plugin`, which is how we make the server also
          a plugin like any other plugins in your collection.

The constructor has to be called with its factory so we first define the
factory and then implement the constructor:

    SERVERPLUGINS_START_SERVER(my_server)
        , ::serverplugins::description("Brief descrition of my server.")
        , ::serverplugins::help_uri("https://snapwebsites.org/help")
        , ::serverplugins::categorization_tag("server")
    SERVERPLUGINS_END_SERVER(my_server)

    my_server::my_server()
        : server(g_my_server_factory)
    {
    ...
    }

**WARNING:** the name must match the class name since we use it in various
             macros to make it all work as expected. This is true of all
             plugins.

To define the signals, use the `PLUGING_SIGNAL()` or `PLUGIN_SIGNAL_WITH_MODE()`
macros. Those create the necessary code to emit the signal.

    class my_server
        : public serverplugins::server
    {
    public:
        ...

        PLUGIN_SIGNAL_WITH_MODE(new_object, (object::pointer_t obj), (obj), NEITHER);
    };

The `NETHER` parameter is a mode used to know whether you want a start
and/or end set of functions also defined. `NEITHER` means none. The
other modes are `START`, `END`, and `BOTH`. The start and end are useful
to have a first function called before the main signal and similarly,
cleanup can happen in the end function since it gets called after the
main signal function. The start function is conditional, if any one of
them returns false, then the rest does not get called. If all of them
return true, then the main signal and all the end signal functions
do get called.

Now we are pretty much ready to initialize the server and load the plugins.
Note the call to the `complete_plugin_initialization()` function. This is
very important because the server expects different arguments than a regular
plugin so we are not able to automatically initialize the plugin pointer in
the server factory.

    int main(int argc, char * argv[])
    {
        my_server::pointer_t s(std::make_shared<my_server>(argc, argv));
        s->complete_plugin_initialization();

        // define a list of colon separated paths
        // (or call the add() function once per path)
        //
        serverplugins::paths p;
        p.add("/usr/local/lib/my_project/plugins:/usr/lib/my_project/plugins");

        // setup the plugin filenames
        //
        serverplugins::names n(p);
        n.find_plugins();

        // create a collection from plugins we found automatically
        // (opposed to setting the name from a plugins=... variable found in your daemon settings)
        //
        serverplugins::collection c(n);

        // now load the plugins
        //
        if(!c.load_plugins(s))
        {
            // ...
            std::cerr << "error: could not load all the plugins.\n";
            return 1;
        }

        return s->run();
    }

**Note 1:** At the moment I most often put the collection insider the server,
            this means I end up with a shared pointer loop since the
            collection holds two shared pointers to the server...

**Note 2:** You are free to create any number of collections. One collection
            can only have one given plugin loaded. However, separate
            collections can share the same plugin (however, the binding
            is currently specific to a server, so in all likelihood, it
            won't work unless you do not use the macros to setup the
            bindings).

**Note 3:** The above does not show any exception handling.


## Send a Signal

Once your server is ready, you can then call your signal with a very
simple C++ function call statement:

    new_object(obj);

The function does not return anything. If you need a result, you may
pass a read/write object by reference or pointer.

Keep in mind that even though it looks like a simple C++ call, in
reality, this function calls all the listeners. So if you have 100
plugins and they all are listening to this signals, the call really
generates 100 C++ calls (and if you have the start and/or end, it
could be as many as 300 calls).

Signals can be defined in plugins and called from said plugins. If
made public, then they can be called from any code that has access
to the plugins having signals.

## Create a Plugin

Similar to a server, when creating a plugin, you derive your class from
the `serverplugin::plugin` base class. You also use the
`SERVERPLUGINS_DEFAULTS()` macro to define the shared pointer, the
constructor and destructor, delete the copy constructor and assignment
operator, and declare the instance() function, which is used to retrieve
the plugin pointer (i.e. a plugin cannot be loaded more than once so it
is very similar to a singleton).

    class my_plugin
        : public serverplugins::plugin
    {
    public:
        SERVERPLUGINS_DEFAULTS(my_plugin);

        // serverplugins::plugin implementation
        //
        virtual void                    bootstrap() override;

        // signals
        //
        void                            on_new_object(object::pointer_t obj);

    ...
    }

As you can see above, the signals a plugin receives are simply defined as
C++ class members. You need the correct list of arguments.

I show a definition of the `bootstrap()` function because 99% of the time,
you will need to connect to at least one signal. This is the function that
does so.

## Implementing a Plugin

### Defining the Plugin

In the corresponding .cpp file, you include two main definitions. The first
one is defining the plugin itself:

    SERVERPLUGINS_START(my_plugin)
        , ::serverplugins::description(
                "My plugin offers an extension...")
        , ::serverplugins::dependency("my_server")
        , ::serverplugins::dependency("other_plugin")
        , ::serverplugins::help_uri("https://snapwebsites.org/help")
        , ::serverplugins::categorization_tag("extention")
        , ::serverplugins::categorization_tag("beautiful")
    SERVERPLUGINS_END(my_plugin)

Here we describe the plugin (useful to display in help screens), dependencies,
a URI to a web page with detailed documentation, and a set of tags. Most of
these parameters are optional.

Note that the mandatory option are added by the `SERVERPLUGINS_START()`
macro:

* Plugin version
* The serverplugins library version at time of compiling
* Last modification time of the .cpp with this macro
* The name of the plugin

Similarly, the `SERVERPLUGINS_END()` macro first closes the list of plugin
definition started with the `SERVERPLUGINS_START()` macro. Then it
adds the plugin factory and the three functions defined by the
`SERVERPLUGINS_DEFAULTS()` macro you used in the class definition.

All the available definitions:

* `::serverplugins::plugin_version()` -- defined through `SERVERPLUGINS_START()`
* `::serverplugins::library_version()` -- defined through `SERVERPLUGINS_START()`
* `::serverplugins::last_modification()` -- defined through `SERVERPLUGINS_START()`
* `::serverplugins::plugin_name()` -- defined through `SERVERPLUGINS_START()`
* `::serverplugins::description()` -- a brief description of the plugin
* `::serverplugins::help_uri()` -- a URI to a web page with a detailed description of the plugin
* `::serverplugins::icon()` -- the name of an icon; used when listing plugins
* `::serverplugins::categorization_tag()` -- the name of a tag to categorize the plugin;
  you can add any number of them
* `::serverplugins::dependency()` -- the name of a dependency (another plugin or the
  main server; although the main server is implicit, you cannot load and run the
  plugins with it)
* `::serverplugins::conflicts()` -- the name of a dependencies in conflict with this one
* `::serverplugins::suggestions()` -- the name of a other plugins to enable
  because it will enhance this plugin functionality
* `::serverplugins::settings_path()` -- a path to additional settings for this plugin

The data above correspond to functions defined in the plugin class which
gets that information from the plugin factory (which you do not usually
deal with at all).

### Connecting Signals

When a collection of plugins is initialized, it first loads all the plugins.
Once all the plugins were successfully loaded, it then calls the
`plugin::bootstrap()` function which you can derive as it is virtual.

Inside that `bootstrap()` function is where you are expected to initialize
the signal connections. This is done with the `SERVERPLUGINS_LISTEN()`
macro.

    void my_plugin::bootstrap()
    {
        SERVERPLUGINS_LISTEN(my_plugin, my_server, new_object, boost::placeholders::_1);
        ...
    }

We offer another macro in case your signal does not accept any arguments.
It would probably be possible to create a macro that understands whether
there are arguments or not, but I don't think it's worth my time at the
moment.

The first parameter, `my_plugin`, is the exact name of your plugin class.
It is used to reference the signal implementation.

The second parameter, `my_server`, is the exact name of the plugin that
emits the signal. As I mentioned, in my cases it is likely the server.
It could also be `another_plugin` which emits the signal you are interested
in.

The third parameter is the name of the signal. This is the name of the
function called by the emitter (a.k.a. `my_server::new_object()` or
`another_plugin::another_function()`).

The parameters following the name of the signal are the signal parameters.
These are boost placeholder since we use an `std::bind()` in the macro.
The `SERVERPLUGINS_LISTEN()` macro requires you to have at least one
argument. If the signal does not take any arguments, then you need to
use the `SERVERPLUGINS_LISTEN0()` macro instead. That macro takes
exactly three parameters.

### Implementing the Signal Handler

Finally, we can create the signal handler. As shown above in the plugin
class, this is just a function with the `on_` introducer prepended to the
name of the signal.

    void my_plugin::on_new_object(object::pointer_t obj)
    {
        // do something with `obj`
    }

There are no specifics here. Note that if your plugin emits a signal from
within such a function, it will be processed immediately. You want to
make sure to prevent loops by not calling the signal you are processing
while processing it.


# License

The project is covered by the GPL 2.0 license.


# Bugs

Submit bug reports and patches on
[github](https://github.com/m2osw/serverplugins/issues).


_This file is part of the [snapcpp project](https://snapwebsites.org/)._

vim: ts=4 sw=4 et
