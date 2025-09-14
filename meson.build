project('GearForge', 'cpp',
  version : '0.0.1',
  default_options : ['cpp_std=c++11'])

inc = include_directories('include')

glog_dep = dependency('glog', required : true)
gtest_dep = dependency('gtest', required : true)  # For tests

sources = [
  'src/gear_calculator.cpp',
  'src/main.cpp',
  'src/ui.cpp',
  'src/user_manager.cpp',
  'src/utils.cpp'
]

test_sources = [
  'tests/main_test.cpp',
  'src/gear_calculator.cpp',
  'src/utils.cpp',
  'src/user_manager.cpp'
]

executable('gearforge',
  sources : sources,
  include_directories : inc,
  dependencies : [glog_dep],
  install : false)

test_exe = executable('tests',
  sources : test_sources,
  include_directories : inc,
  dependencies : [gtest_dep, glog_dep])

test('GearForgeTests', test_exe)
