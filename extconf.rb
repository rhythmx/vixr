require 'mkmf'

$CFLAGS = (ENV['CFLAGS']||"") + "-Wall"

# allow --with-vixapi-include to set an include path
dir_config("vixapi")
# default include directory
$CFLAGS += " -I/usr/include/vmware-vix"

vix_so = "vixAllProducts"

# compile with debugging symbols
if with_config('debug')
	$CFLAGS += " -g"
	# vix_so = "vixAllProductsd" (referenced in the doc, not there on linux)
end

# Check VIX lib
if not have_library(vix_so,'VixHost_Connect')
	puts "Can't find the Vix library"
	exit(-1)
end	

# Link to vix
$LDFLAGS = (ENV["LDFLAGS"]||"")+" -l#{vix_so}"

create_makefile("vixapi")
