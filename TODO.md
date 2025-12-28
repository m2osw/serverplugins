

* Look at using templated functions to handle the signals instead of macros

  I think that the listen macros could be written using templated functions
  with compile time tests to know which one to select.

  The listen have optional arguments and priority (4 macros). The main
  problem I have had is that the `emitter_class` is a type and the signal
  name is concatenated, so those two are not arguments we can pass to a
  function. The `emitter_class` could be the first template parameter.

      #define SERVERPLUGINS_LISTEN(name, emitter_class, signal, args...) \
          do { emitter_class::pointer_t plugin_pointer(plugins()->get_plugin<emitter_class>(::serverplugins::name_without_namespace(#emitter_class))); \
              if(plugin_pointer != nullptr) plugin_pointer->signal_listen_##signal( \
                              std::bind(&name::on_##signal, this, ##args)); } while(false)

  The current version definitely requires us to have macros to concatenate
  the name of the signal and access the two functions:

  * `signal_listen_##signal()`
  * `name::on_##signal()`

