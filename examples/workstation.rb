require 'vixr'

# Since workstation runs as a local user, no params are needed
host = VixR.connect()

# open_vmx() loads up a Virtual Machine description file, 
# you can use the returned object to perform nearly any
# operation that the UI allows.
vm = host.open_vmx("/home/user/vmware/Ubuntu/Ubuntu.vmx")

# Start it up 
if not vm.running?
	vm.power_on
end

# Wait 300 seconds for vmware tools to start up
vm.wait_for_tools(300)

# open up vmware tools
if vm.login("root","sekrit")
	# Run a program in the VM and save results
	vm.run_prog("/bin/bash","-c 'find /root' > /tmp/rootfiles")
	# Copy results to localhost
	vm.copy_to_host("/tmp/rootfiles","/tmp/vmrootfiles")
	# Print out the file
	puts File.open("/tmp/vmrootfiles").read
end

# Power down, :fromguest means to use a shutdown script 
vm.power_off(:fromguest=>true)
