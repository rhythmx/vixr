require 'vixapi' # raw C extension mappings

## Global Default Options
$vixopt = {
	# connect opts
	:hosttype => VixAPI::Const::Provider::Workstation,
	:hostname => nil,
	:user => nil,
	:pass => nil,
	:port => 8222,
	
	# vm opts
	:vmxfile => nil,
	:showgui => false,
	:poweroffinguest => true # reset and poweroff will use the guest OS to shutdown
}

class VixAPI
	attr_accessor :opt
	attr_reader :session

	def initialize(opt={})
		@opt=$vixopt.merge(opt)
	end

	def connect(opt={})
		opt = @opt.merge(opt)
		opt[:hosttype] = VixAPI::Const::Provider::Server2x if opt[:hostname] =~ /^http(s)?:\/\/.*/ 
		case opt[:hosttype]
		when Const::Provider::Workstation
			session = _connect_wkstn()
		when Const::Provider::Server2x
			
			session = _connect_server(
				VixAPI::Const::Provider::Server2x,
				opt[:hostname],
				opt[:port],
				opt[:user],
				opt[:pass]
			)
		else
			raise "This VMware host provider type is not supported (you probably should use VMware Server 2.x)"
		end

		raise "could not get session" if not session

		@host = Host.new(self,session)
		@host
	end	

	class VM
		def initialize(vixapi, vmhandle)
			@api = vixapi
			@vm = vmhandle
		end
		def power_on(opt)
			if not opt.class < Integer
				opts = @api.opt.merge(opt)
				opt = 0
				opt += Const::VMPowerOpt::LaunchGUI if opts[:showgui]
				opt += Const::VMPowerOpt::StartPaused if opts[:startpaused]
			end
			@api._power_on(@vm,opt)
		end
		def power_off(fromguest=@api.opt[:poweroffinguest])
			if fromguest
				op = Const::VMPowerOpt::FromGuest
			else
				op = Const::VMPowerOpt::Normal
			end	
			@api._power_off(@vm,op)
		end
		def reset(fromguest=@api.opt[:poweroffinguest])
			if fromguest
				op = Const::VMPowerOpt::FromGuest
			else
				op = Const::VMPowerOpt::Normal
			end	
			@api._reset(@vm,op)
		end
		def pause
			@api._pause(@vm)
		end
		def unpause
			@api._unpause(@vm)
		end
		def suspend
			@api._suspend(@vm)
		end

		def write_var(name,val,type=Const::VarType::Guest)
			@api._write_var(@vm,type,name,val)
		end
		def read_var(name, type=Const::VarType::Guest)
			@api._read_var(@vm,type,name)
		end
		def upgrade_vhardware()
			@api._upgrade_vhardware(@vm)
		end
	end	

	class Host
		def initialize(vixapi,host)
			@api = vixapi
			@session = host
		end	
		def vmx_open(path=opt[:vmxfile])	
			vmhandle = @api._vmx_open(@session,path)
			raise "Could not open vmx file (#{path})" if not vmhandle
			vm = VM.new(@api,vmhandle) 
			vm
		end
	end
end


## init C runtime (only once)
VixAPI.new.start

# Test script
vix = VixAPI.new
host = vix.connect 
vm = host.vmx_open("/data/tmp/BPSLABS-W2K3-01/Windows Server 2003 Enterprise Edition.vmx")
vm.read_var("poo")
#vm.power_on(:showgui=>true)

#puts "Powered on"

