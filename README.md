
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
functonality is instantly available.


# Exceptions

The library includes its own set of exception. Note that exception can be
used in a plugin just like any other function in your software.


# License

The project is covered by the GPL 2.0 license.


# Bugs

Submit bug reports and patches on
[github](https://github.com/m2osw/serverplugins/issues).


_This file is part of the [snapcpp project](https://snapwebsites.org/)._

vim: ts=4 sw=4 et
