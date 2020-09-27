gtest = dependency('gtest', main: true, disabler: true, required: build_tests)
gmock = dependency('gmock', disabler: true, required: build_tests)

configure_file(output: 'config.h',
  configuration: cdata,
)
test_inc = include_directories('.')

test_inspur_ipmi_oem = executable(
  'test_inspur-ipmi-oem',
  '../src/inspur_oem.cpp',
  'test_inspur_ipmi_oem.cpp',
  'mocked_utils.cpp',
  'mocked_sdbus.cpp',
  include_directories: [test_inc, src_inc],
  dependencies: [
    gtest,
    gmock,
    phosphor_logging,
    phosphor_dbus_interfaces,
    sdbusplus,
  ])

test('test_inspur-ipmi-oem', test_inspur_ipmi_oem)
