configure_file(output: 'config.h',
    configuration: cdata,
)

src_inc = include_directories('.')

so_version = '0.1.0'

shared_library(
  'inspur-ipmi-oem',
  'inspur_oem.cpp',
  'utils.cpp',
  dependencies: [
    phosphor_dbus_interfaces,
    phosphor_logging,
    libipmid,
  ],
  version: so_version,
  install: true,
  install_dir: get_option('libdir') / 'ipmid-providers'
  )
