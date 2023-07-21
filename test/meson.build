gtest_dep = dependency('gtest', main: true, disabler: true, required: build_tests)
gmock_dep = dependency('gmock', disabler: true, required: build_tests)
if not gtest_dep.found() or not gmock_dep.found()
    gtest_proj = import('cmake').subproject('googletest', required: false)
    if gtest_proj.found()
        gtest_dep = declare_dependency(
            dependencies: [
                dependency('threads'),
                gtest_proj.dependency('gtest'),
                gtest_proj.dependency('gtest_main'),
            ]
        )
        gmock_dep = gtest_proj.dependency('gmock')
  else
        assert(
            not get_option('tests').enabled(),
            'Googletest is required if tests are enabled'
        )
  endif
endif

test_deps = [
    gtest_dep,
    gmock_dep,
    phosphor_logging_dep,
    phosphor_dbus_interfaces_dep,
    sdbusplus_dep,
]

test_sources = [
    '../src/inspur_oem.cpp',
]

tests = [
    'mocked_utils.cpp',
    'mocked_sdbus.cpp',
    'test_inspur_ipmi_oem.cpp',
]
test_inspur_ipmi_oem = executable(
    'test_inspur-ipmi-oem',
    tests,
    test_sources,
    include_directories: ['..', '../src'],
    dependencies: [test_deps],
)

test('test_inspur-ipmi-oem', test_inspur_ipmi_oem)
