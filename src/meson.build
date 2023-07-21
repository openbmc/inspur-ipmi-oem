sources = [
    'inspur_oem.cpp',
    'utils.cpp',
    'sdbus_wrapper.cpp',
]

deps = [
    ipmid_dep,
    phosphor_dbus_interfaces_dep,
    phosphor_logging_dep,
]

shared_library(
    'inspur-ipmi-oem',
    sources,
    include_directories: ['..'],
    dependencies: [deps],
    version: meson.project_version(),
    install: true,
    install_dir: get_option('libdir') / 'ipmid-providers'
)
