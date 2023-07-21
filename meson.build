project(
    'inspur-ipmi-oem',
    'cpp',
    default_options: [
        'warning_level=3',
        'werror=true',
        'cpp_std=c++23',
        'prefix=/usr',
        'b_lundef=false',
        'buildtype=debugoptimized',
    ],
    license: 'Apache-2.0',
    version: '1.0',
    meson_version: '>=1.1.1',
)

phosphor_dbus_interfaces = dependency('phosphor-dbus-interfaces')
phosphor_logging = dependency('phosphor-logging')
libipmid = dependency('libipmid')
sdbusplus = dependency('sdbusplus')

# Common configurations for src and test
cdata = configuration_data()
cdata.set_quoted('VERSION_IFACE', 'xyz.openbmc_project.Software.Version')
cdata.set_quoted('VERSION', 'Version')
cdata.set_quoted('BIOS_OBJPATH', get_option('BIOS_OBJPATH'))

subdir('src')

build_tests = get_option('tests')
if not build_tests.disabled()
  subdir('test')
endif
