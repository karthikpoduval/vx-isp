project('vx_isp', 'c')

# Define the sources for the C library
libisp_sources = files('vx_lib_isp.c', 'vx_demosaic.c')

# Define the sources for the C++ application
#bar_sources = files('bar.cpp', 'bar.h')

# Create a shared library target for the C library
libisp = shared_library('openvx-isp', libisp_sources, link_args : ['-lopenvx', '-lopenvx-debug'])

test_isp_sources = files('test.c')

# Create an executable target for the C++ application
test_isp = executable('test_vx_isp', test_isp_sources, link_with: libisp, link_args : ['-lopenvx', '-lopenvx-c_model'])


