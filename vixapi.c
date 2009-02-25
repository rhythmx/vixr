#include <stdio.h>
#include <ruby.h>

#include "vix.h"

/* local methods */
#include "vm.h"
#include "host.h"

/*
 * Globals
 */
static VALUE started = Qnil;

VALUE 
vix_start(VALUE self)
{
	if(started == Qnil)
	{
		fprintf(stderr,"--     loading vix api for ruby    --\n");
		fprintf(stderr,"--         Sean Bradly, 2009       --\n");
		fprintf(stderr,"-- This extension is BSD-licensed  --\n");
		fprintf(stderr,"-- The VMware VIX API (R) is not   --\n");
		started = Qtrue;
		return Qtrue;
	}
	return Qnil;
}

/*
VALUE
_vix_event_loop(VALUE self)
{
	int session = NUM2INT(rb_iv_get(self,"@session"));
	Vix_PumpEvents(session,0);
	return Qnil;
}
*/ 

void 
Init_vixapi() {
	VALUE vixapi = rb_define_class("VixAPI", rb_cObject);

	/* global init function */
	rb_define_method(vixapi, "start", vix_start, 0);

	/* instance methods */
	rb_define_method(vixapi,"_connect_server",_connect_server,5);
	rb_define_method(vixapi,"_connect_wkstn",_connect_wkstn,0);
	rb_define_method(vixapi,"_vmx_open",_vmx_open,2);
	rb_define_method(vixapi,"_delete",_delete,2);
	rb_define_method(vixapi,"_read_var",_read_var,3);
	rb_define_method(vixapi,"_write_var",_write_var,4);
	rb_define_method(vixapi,"_upgrade_vhardware",_upgrade_vhardware,1);
	/* VM Power Methods */
	rb_define_method(vixapi,"_power_on",_power_on,2);
	rb_define_method(vixapi,"_power_off",_power_on,2);
	rb_define_method(vixapi,"_reset",_power_on,2);
	rb_define_method(vixapi,"_pause",_power_on,1);
	rb_define_method(vixapi,"_suspend",_power_on,1);
	rb_define_method(vixapi,"_unpause",_power_on,1);
	

	/* rb_define_method(vixapi,"_vix_event_loop",_vix_event_loop); */
	
	/* define constants (ruby form of vix.h) */
	{
		VALUE constants  = rb_define_module_under(vixapi,"Const");
		VALUE provider   = rb_define_module_under(constants,"Provider");
		VALUE poweropt   = rb_define_module_under(constants,"VMPowerOpt");
		VALUE vartype    = rb_define_module_under(constants,"VarType");
		VALUE eventtype  = rb_define_module_under(constants,"EventType");
		VALUE handletype = rb_define_module_under(constants,"HandleType");
		VALUE property   = rb_define_module_under(constants,"PropertyType"); 
		VALUE props      = rb_define_module_under(constants,"Property");
		VALUE propshost  = rb_define_module_under(props, "Host");
		VALUE propsvm    = rb_define_module_under(props, "VM");
		VALUE propssnap  = rb_define_module_under(props,"Snapshot");

		/* NULL Handle */
		rb_define_const(constants, "InvalidHandle", INT2NUM(VIX_INVALID_HANDLE));

		/* Var Types */
		rb_define_const(vartype, "Guest",            INT2NUM(VIX_VM_GUEST_VARIABLE));
		rb_define_const(vartype, "ConfigRuntime",    INT2NUM(VIX_VM_CONFIG_RUNTIME_ONLY));
		rb_define_const(vartype, "GuestEnvironment", INT2NUM(VIX_GUEST_ENVIRONMENT_VARIABLE));

		/* Event Types */
		rb_define_const(eventtype, "JobCompleted", INT2NUM(VIX_EVENTTYPE_JOB_COMPLETED));
		rb_define_const(eventtype, "JobProgress",  INT2NUM(VIX_EVENTTYPE_JOB_PROGRESS));
		rb_define_const(eventtype, "FindItem",     INT2NUM(VIX_EVENTTYPE_FIND_ITEM));

		/* VixProperty Types */
		rb_define_const(property, "Any",     INT2NUM(VIX_PROPERTYTYPE_ANY));
		rb_define_const(property, "Integer", INT2NUM(VIX_PROPERTYTYPE_INTEGER));
		rb_define_const(property, "String",  INT2NUM(VIX_PROPERTYTYPE_STRING));
		rb_define_const(property, "Bool",    INT2NUM(VIX_PROPERTYTYPE_BOOL));
		rb_define_const(property, "Handle",  INT2NUM(VIX_PROPERTYTYPE_HANDLE));
		rb_define_const(property, "Int64",   INT2NUM(VIX_PROPERTYTYPE_INT64));
		rb_define_const(property, "Blob",    INT2NUM(VIX_PROPERTYTYPE_BLOB));
		
		/* VixProperties */		
		rb_define_const(props,     "None",              INT2NUM(VIX_PROPERTY_NONE));
		rb_define_const(props,     "MetadataContainer", INT2NUM(VIX_PROPERTY_META_DATA_CONTAINER));
		rb_define_const(props,     "FoundItemLocation", INT2NUM(VIX_PROPERTY_FOUND_ITEM_LOCATION));
		rb_define_const(propshost, "HostType",          INT2NUM(VIX_PROPERTY_HOST_HOSTTYPE));
		rb_define_const(propshost, "APIVersion",        INT2NUM(VIX_PROPERTY_HOST_API_VERSION));
		rb_define_const(propsvm,   "NumCPUs",           INT2NUM(VIX_PROPERTY_VM_NUM_VCPUS));
		rb_define_const(propsvm,   "VMXPath",           INT2NUM(VIX_PROPERTY_VM_VMX_PATHNAME));
		rb_define_const(propsvm,   "VMTeamPath",        INT2NUM(VIX_PROPERTY_VM_VMTEAM_PATHNAME));
		rb_define_const(propsvm,   "MemSize",           INT2NUM(VIX_PROPERTY_VM_MEMORY_SIZE));
		rb_define_const(propsvm,   "ReadOnly",          INT2NUM(VIX_PROPERTY_VM_READ_ONLY));
		rb_define_const(propsvm,   "InVMTeam",          INT2NUM(VIX_PROPERTY_VM_IN_VMTEAM));
		rb_define_const(propsvm,   "PowerState",        INT2NUM(VIX_PROPERTY_VM_POWER_STATE));
		rb_define_const(propsvm,   "ToolsState",        INT2NUM(VIX_PROPERTY_VM_TOOLS_STATE));
		rb_define_const(propsvm,   "IsRunning",         INT2NUM(VIX_PROPERTY_VM_IS_RUNNING));
		rb_define_const(propsvm,   "SupportedFeatures", INT2NUM(VIX_PROPERTY_VM_SUPPORTED_FEATURES));
		rb_define_const(propsvm,   "IsRecording",       INT2NUM(VIX_PROPERTY_VM_IS_RECORDING));
		rb_define_const(propsvm,   "IsPlaying",         INT2NUM(VIX_PROPERTY_VM_IS_REPLAYING));
		rb_define_const(propssnap, "DisplayName",       INT2NUM(VIX_PROPERTY_SNAPSHOT_DISPLAYNAME));
		rb_define_const(propssnap, "Description",       INT2NUM(VIX_PROPERTY_SNAPSHOT_DESCRIPTION));
		rb_define_const(propssnap, "PowerState",        INT2NUM(VIX_PROPERTY_SNAPSHOT_POWERSTATE));
		rb_define_const(propssnap, "IsReplayable",      INT2NUM(VIX_PROPERTY_SNAPSHOT_IS_REPLAYABLE));
		
		/* VixHandle types */
		rb_define_const(handletype, "None",              INT2NUM(VIX_HANDLETYPE_NONE));
		rb_define_const(handletype, "Host",              INT2NUM(VIX_HANDLETYPE_HOST));
		rb_define_const(handletype, "VM",                INT2NUM(VIX_HANDLETYPE_VM));
		rb_define_const(handletype, "Network",           INT2NUM(VIX_HANDLETYPE_NETWORK));
		rb_define_const(handletype, "Job",               INT2NUM(VIX_HANDLETYPE_JOB));
		rb_define_const(handletype, "Snapshot",          INT2NUM(VIX_HANDLETYPE_SNAPSHOT));
		rb_define_const(handletype, "PropertyList",      INT2NUM(VIX_HANDLETYPE_PROPERTY_LIST));
		rb_define_const(handletype, "MetadataContainer", INT2NUM(VIX_HANDLETYPE_METADATA_CONTAINER));

		/* VMware host types */
		rb_define_const(provider, "Default",     INT2NUM(VIX_SERVICEPROVIDER_DEFAULT));
		rb_define_const(provider, "Server1x",    INT2NUM(VIX_SERVICEPROVIDER_VMWARE_SERVER));
		rb_define_const(provider, "Server2x",    INT2NUM(VIX_SERVICEPROVIDER_VMWARE_VI_SERVER));
		rb_define_const(provider, "Workstation", INT2NUM(VIX_SERVICEPROVIDER_VMWARE_WORKSTATION));

		/* VMPower function options */
		rb_define_const(poweropt,"Normal",           INT2NUM(VIX_VMPOWEROP_NORMAL));
		rb_define_const(poweropt,"FromGuest",        INT2NUM(VIX_VMPOWEROP_FROM_GUEST));
		rb_define_const(poweropt,"SuppressSnapshot", INT2NUM(VIX_VMPOWEROP_SUPPRESS_SNAPSHOT_POWERON));
		rb_define_const(poweropt,"LaunchGUI",        INT2NUM(VIX_VMPOWEROP_LAUNCH_GUI));
		rb_define_const(poweropt,"StartPaused",      INT2NUM(VIX_VMPOWEROP_START_VM_PAUSED));
		
	}
}
