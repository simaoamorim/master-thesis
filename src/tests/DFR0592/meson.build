project('DFR0592', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

# Find dependencies
i2cdep = meson.get_compiler('c').find_library('i2c', required : true)

# These arguments are only used to build the shared library
# not the executables that use the library.
lib_args = ['-DBUILDING_DFR0592']

shlib = shared_library('dfr0592', 'dfr0592.c',
  install : true,
  c_args : lib_args,
  gnu_symbol_visibility : 'hidden',
  dependencies : i2cdep,
)

test_exe = executable('dfr0592_test', 'dfr0592_test.c',
  link_with : shlib, dependencies : i2cdep)
test('dfr0592', test_exe)

# Make this library usable as a Meson subproject.
dfr0592_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with : shlib,
  dependencies : i2cdep)

# Make this library usable from the system's
# package manager.
install_headers('dfr0592.h', subdir : 'dfr0592')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : 'DFR0592',
  filebase : 'dfr0592',
  description : 'Meson sample project.',
  subdirs : 'dfr0592',
  libraries : shlib,
  version : '0.1',
)
