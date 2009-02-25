require 'mkmf'

$CFLAGS = (ENV['CFLAGS']||"") + "-Wall"
$LDFLAGS = (ENV["LDFLAGS"]||"")+"-lvixAllProducts"

have_library('vixAllProducts','VixHost_Connect')

dir_config("vixapi","/usr/include/vmware-vix")

create_makefile("vixapi")
