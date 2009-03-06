# Copyright (c) 2009 Sean Bradly
#
# This code is provided under the BSD license 

require 'vixapi' # vixapi.so, C extension wrappers


# The VixAPI module is defined in C by vixapi.so this is just here so 
# there can be ruby method signatures.
module VixAPI
    
    def login_in_guest(*args); raise "NOT IMPLEMENTED"; end
    def logout_from_guest(*args); raise "NOT IMPLEMENTED"; end
        
    #
    # Guest OS methods
    #

    def install_tools(*args); raise "NOT IMPLEMENTED"; end

    #
    # Guest OS File methods
    #

    def create_directory_in_guest(*args); raise "NOT IMPLEMENTED"; end
    def list_directory_in_guest(*args); raise "NOT IMPLEMENTED"; end
    def rename_file_in_guest(*args); raise "NOT IMPLEMENTED"; end

    #
    # Guest OS Process methods
    #

    def kill_process_in_guest(*args); raise "NOT IMPLEMENTED"; end
    def list_processes_in_guest(*args); raise "NOT IMPLEMENTED"; end
    def open_url_in_guest(*args); raise "NOT IMPLEMENTED"; end
    def run_program_in_guest(*args); raise "NOT IMPLEMENTED"; end
    def run_script_in_guest(*args); raise "NOT IMPLEMENTED"; end

    #
    # Guest OS Shared Folder methods
    #

    def enable_shared_folders(*args); raise "NOT IMPLEMENTED"; end
    def add_shared_folder(*args); raise "NOT IMPLEMENTED"; end
    def remove_shared_folder(*args); raise "NOT IMPLEMENTED"; end
    # this will have to be abstracted into ruby-friendlyness
    #def set_shared_folder_state(*args); raise "NOT IMPLEMENTED"; end
    #def get_shared_folder_state(*args); raise "NOT IMPLEMENTED"; end
    #def get_num_shared_folders(*args); raise "NOT IMPLEMENTED"; end

    #
    # Snapshot methods
    #
    
    # create a custom set of C functions to deal with this

    #
    # VM methods
    #
    def clone(*args); raise "NOT IMPLEMENTED"; end

    # Constants as defined in `/usr/include/vmware-vix/vix.h'
    module Const
        # Endpoint's host type (VMware Workstation, Server, Etc)
        module Provider
            Default     = 1
            Server1x    = 2
            Workstation = 3
            Server2x    = 10
        end
        # VixHandle properties types 
        module Property
            module Host
                HostType   = 50
                APIVersion = 51
            end
            module VM
                NumVCPUs          = 101
                VMXPathname       = 103
                TeamPathname      = 104
                MemorySize        = 106
                ReadOnly          = 107
                InVMTeam          = 128
                PowerState        = 129
                ToolsState        = 152
                IsRunning         = 196
                SupportedFeatures = 197
                IsRecording       = 236
                IsReplaying       = 237
            end
        end
        # on/off/reset options
        module VMPowerOpt
            Normal          = 0x0000
            FromGuest       = 0x0004
            SupressSnapshot = 0x0080
            LaunchGUI       = 0x0200
            StartPaused     = 0x1000
        end
        # Delete VM opt
        module VM
            DeleteDiskFiles = 0x0002
        end
        # R/W Variable types
        module Var
            Guest            = 1
            ConfigRuntime    = 2
            GuestEnvironment = 3
        end
    end

    # Encapsulation for handles (mostly to make properties easy)
    class Handle 
        attr_accessor :handle
        def initialize(handle)
            @handle = handle
        end
        def getproperty(property)
            VixAPI._getproperty(self,property)
        end
    end

end


module VixR

    ## Global Default Options (constant)
    def self.opt 
        {
            # connect opts
            :hosttype => VixAPI::Const::Provider::Workstation,
            :hostname => nil,
            :user => nil,
            :pass => nil,
            :port => 8222,
            
            # vm opts
            :vmxfile => nil,
            :showgui => false,
            :fromguest => true, # reset and poweroff will use the guest OS to shutdown
            :deletefile => false,
        }
    end

    # VixR.connect() => VixR::Host
 	def self.connect(opt={})
		opt = VixR.opt.merge(opt)
        # Figure out Workstation vs. Server
		opt[:hosttype] = VixAPI::Const::Provider::Server2x if opt[:hostname] =~ /^http(s)?:\/\/.*/ 
		case opt[:hosttype]
		when VixAPI::Const::Provider::Workstation
			session = VixAPI._connect(VixAPI::Const::Provider::Workstation,
                                         nil,0,nil,nil)
		when VixAPI::Const::Provider::Server2x
            session = VixAPI._connect(VixAPI::Const::Provider::Server2x,
                                        opt[:hostname],
                                        opt[:port],
                                        opt[:user],
                                        opt[:pass])
		else
			raise "This VMware host provider type is not supported (you probably should use VMware Server 2.x)"
		end

		raise "could not get session" if not session
        
		@host = Host.new(session.handle)
		@host
	end	

    #
    # Host class
    #
	class Host < VixAPI::Handle

        # open_vmx(vmxfilepath) => VixR::VM
        def open_vmx(path=nil,opt={})
            opt = VixR.opt.merge(opt)
            path ||= opt[:vmxfile]
            vmhandle = VixAPI._open_vmx(self,path)
            raise "Could not open vmx file (#{path})" if not vmhandle
            vm = VM.new(vmhandle.handle)
            vm
        end
        
        # disconnect(hostHandle) => true
        #
        # This should automatically be called automatically when the program exits
        def disconnect()
            VixAPI._disconnect(@handle)
        end

        #
        # Properties
        #
        def hosttype
            self.getproperty(VixAPI::Const::Property::Host::HostType)
        end

        def api_version
            self.getproperty(VixAPI::Const::Property::Host::APIVersion)
        end
	end

    #
    # VM class
    #
	class VM < VixAPI::Handle
        
        # power_on(:showgui=>false, :startpaused=>false) => true
        #
        # Power on the VM
        def power_on(opt={})
            opt = VixR.opt.merge(opt)
            opts = 0
            opts |= VixAPI::Const::VMPowerOpt::LaunchGUI if opt[:showgui]
            opts |= VixAPI::Const::VMPowerOpt::StartPaused if opt[:startpaused]
            VixAPI._power_on(self,opts)            
        end

        # power_off(:fromguest=>true) => true
        def power_off(opt={})
            opt = VixR.opt.merge(opt)
            opts = 0
            opts |= VixAPI::Const::VMPowerOpt::FromGuest if opt[:fromguest]
            VixAPI._power_off(self,opts)
        end

        # pause()
        def pause
            VixAPI._pause(self)
        end

        # suspend()
        def suspend
            VixAPI._suspend(self)
        end

        # unpause()
        def unpause
            VixAPI._unpause(self)
        end

        # reset(:fromguest=>true)
        def reset(opt={})
            opt = VixR.opt.merge(opt)
            opts = 0
            opts |= VixAPI::Const::VMPowerOpt::FromGuest if opt[:fromguest]
            VixAPI._reset(self,opts)
        end

        # delete(:deletefiles=>false)
        def delete(opt={})
            opt = VixR.opt.merge(opt)
            opts = 0
            opts |= VixAPI::Const::VM::DeleteDiskFiles if opt[:deletefiles]
            VixAPI._delete(self,opts)
        end

        # upgrade_vhardware()
        def upgrade_vhardware
            VixAPI._upgrade_vhardware(self)
        end

        # read_var()
        def read_var(name,vartype=VixAPI::Config::Var::Guest)
            VixAPI._read_var(self,vartype,name)
        end

        # write_var()
        def write_var(name,val,type=VixAPI::Config::Var::Guest)
            VixAPI._write_var(self,type,name,val)
        end

        #
        # Guest / VM Tools Functions
        #

        # screendump(file->nil).. if file non nil, save to file, else return bytes
        def screendump(file=nil)
            pngdata = VixAPI._capture_screen_image(self)
        end

        # wait_for_tools(timeout=nil)
        def wait_for_tools(timeout=nil)
            timeout ||= VixR.opt[:timeout]
            timeout ||= 300 # 5 minutes
            VixAPI._wait_for_tools(self,timeout)
        end

        def cp_to_host(src,dst)
            VixAPI._copy_file_from_guest_to_host(self,src,dst)
        end

        def cp_to_guest(src,dst)
            VixAPI._copy_file_from_host_to_guest(self,src,dst)
        end

        def mkdir(path)
            VixAPI._create_directory_in_guest(self,path)
        end

        # mktemp() # => tempfile_pathname
        def mktemp()
            VixAPI._create_temp_file_in_guest(self)
        end

        def rmdir(path)
            VixAPI._delete_directory_in_guest(self,path)
        end

        def rm(path)
            VixAPI._delete_file_in_guest(self,path)
        end

        def dir_exists?(path)
            VixAPI._directory_exists_in_guest(self,path)
        end

        # login(user=nil,pass=>nil) # => also reads :username, :password from $opt
        def login(user=nil,pass=nil)
            user ||= VixR.opt[:username]
            pass ||= VixR.opt[:password]
            raise "You must provide a username and password via the options" if not user or not pass
            VixAPI._login_in_guest(self,user,pass)
        end

        def logout
            VixAPI._logout_from_guest(self)
        end

        def enable_shared_folders
            VixAPI._enable_shared_folders(self,true);
        end

        def disable_shared_folders
            VixAPI._enable_shared_folders(self,false);
        end

        def file_exists?(filename)
            VixAPI._file_exists_in_guest(self,filename)
        end

        def file_size(filename)
            VixAPI._file_size(self,filename)
        end
        
        def directory?(path)
            VixAPI._file_is_directory(self,path)
        end

        def symlink?(path)
            VixAPI._file_is_symlink(self,path)
        end
        
        def file_mod_time(path)
            VixAPI._file_mod_time(self,path)
        end
        
        def install_tools
            VixAPI._install_tools(self)
        end

        def kill_pid(pid)
            VixAPI._kill_process_in_guest(self,pid)
        end

        def list_dir(dir)
            VixAPI._list_directory_in_guest(self,dir)
        end

        def list_pids
            VixAPI._list_processes_in_guest(self)
        end

        def open_url(url)
            VixAPI._open_url_in_guest(self,url)
        end

        def rename(src,dst)
            VixAPI._rename_file_in_guest(self,src,dst)
        end

        def run_prog(exe,args)
            VixAPI._run_program_in_guest(self,exe,args)
        end

        def run_script(interpreter,sourcetxt)
            VixAPI._run_script_in_guest(self,interpreter,sourcetext)
        end

        #
        # Properties
        #
        def num_vcpus
            self.getproperty(VixAPI::Const::Property::VM::NumVCPUs)
        end

        def vmx_path
            self.getproperty(VixAPI::Const::Property::VM::VMXPathname)
        end
        
        def team_path
            self.getproperty(VixAPI::Const::Property::VM::TeamPathname)
        end
        
        def mem_size
            self.getproperty(VixAPI::Const::Property::VM::MemorySize)
        end
        
        def read_only?
            self.getproperty(VixAPI::Const::Property::VM::ReadOnly)
        end
        
        def vm_team?
            self.getproperty(VixAPI::Const::Property::VM::InVMTeam)
        end
        
        def power_state
            self.getproperty(VixAPI::Const::Property::VM::PowerState)
        end
        
        def tools_state
            self.getproperty(VixAPI::Const::Property::VM::ToolsState)
        end
        
        def running?
            self.getproperty(VixAPI::Const::Property::VM::IsRunning)
        end

        def features
            self.getproperty(VixAPI::Const::Property::VM::SupportedFeatures)
        end

        def recording?
            self.getproperty(VixAPI::Const::Property::VM::IsRecording)
        end
        
        def replaying?
            self.getproperty(VixAPI::Const::Property::VM::IsReplaying)
        end
	end	


end


## init C runtime (only once)
VixAPI.start

#VixAPI.connect

