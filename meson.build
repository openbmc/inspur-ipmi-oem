project(
    'inspur-ipmi-oem',
    'cpp',
    default_options: [
        'warning_level=3',
        'werror=true',
        'cpp_std=c++17',
        'prefix=/usr',
        'b_lundef=false',
        'buildtype=debugoptimized',
    ],
    license: 'Apache-2.0',
    version: '1.0',
    meson_version: '>=0.49.0',
)

phosphor_dbus_interfaces = dependency('phosphor-dbus-interfaces')
phosphor_logging = dependency('phosphor-logging')
libipmid = dependency('libipmid')

subdir('src')
