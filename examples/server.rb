require 'vixr'

# Login into the Server 
host = VixR.connect(:hostname => "http://my-vmware-server:8222/sdk", :user => "root", :password => "sekrit")

# open_vmx() loads up a Virtual Machine description file, 
# you can use the returned object to perform nearly any
# operation that the UI allows. Note the '[standard]' 
# datastore notation on teh string
vm = host.open_vmx("[standard] Ubuntu/Ubuntu.vmx")

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
