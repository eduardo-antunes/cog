project('cog', 'c',
  version: '0.1.0',
  default_options: [ 'c_std=c11', 'warning_level=3' ]
)

inc_dir = include_directories('include')
sources = files(
  'src/array.c',
  'src/box.c',
  'src/compiler.c',
  'src/debug.c',
  'src/lexer.c',
  'src/main.c',
  'src/memory.c',
  'src/value.c',
  'src/vm.c',
)

executable('cog', sources, include_directories: inc_dir)
