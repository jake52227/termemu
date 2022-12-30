project('termemu', ['cpp', 'c'],
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

glfw = dependency('glfw3')
freetype = dependency('freetype2')
opengl = dependency('GL')

subdir('src')

exe = executable('termemu', sources,
  install : true,
  dependencies: [glfw, freetype, opengl])

test('basic', exe)
