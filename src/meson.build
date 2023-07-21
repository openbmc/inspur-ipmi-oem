sources = [
    'inspur_oem.cpp',
    'utils.cpp',
    'sdbus_wrapper.cpp',
]

deps = [
    libipmid,
    phosphor_dbus_interfaces,
    phosphor_logging,
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
