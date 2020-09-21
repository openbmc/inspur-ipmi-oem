src_inc = include_directories('.')

shared_library(
  'inspur-ipmi-oem',
  'inspur_oem.cpp',
  dependencies: [
    phosphor_dbus_interfaces,
    phosphor_logging,
    libipmid,
  ],
  install: true,
  install_dir: get_option('libdir') / 'ipmid-providers'
  )
