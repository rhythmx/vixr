#include <stdio.h>
#include <ruby.h>
#include "vix.h"

#include "functions.h"

/*
 * VM Power Routines
 */

VALUE 
_power_on(VALUE self, VALUE rvm, VALUE ropt)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	int opt = NUM2INT(ropt);

	job = VixVM_PowerOn(vm,opt,VIX_INVALID_HANDLE,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to power on virtual machine: %s\n", Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return Qtrue;
}

VALUE 
_power_off(VALUE self, VALUE rvm, VALUE ropt)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	int opt = NUM2INT(ropt);

	job = VixVM_PowerOff(vm,opt,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to power off virtual machine: %s\n", Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return Qtrue;
}

VALUE 
_pause(VALUE self, VALUE rvm)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	job = VixVM_Pause(vm,0,VIX_INVALID_HANDLE,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to pause virtual machine: %s\n", Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return Qtrue;
}

VALUE 
_suspend(VALUE self, VALUE rvm)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	job = VixVM_Suspend(vm,0,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to suspend virtual machine: %s\n", Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return Qtrue;
}

VALUE 
_unpause(VALUE self, VALUE rvm)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	job = VixVM_Unpause(vm,0,VIX_INVALID_HANDLE,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to unpause virtual machine: %s\n", Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return Qtrue;
}

VALUE 
_reset(VALUE self, VALUE rvm, VALUE ropt)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	int opt = NUM2INT(ropt);

	job = VixVM_Reset(vm,opt,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to reset virtual machine: %s\n", Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return Qtrue;
}

/*
 * Misc VM Routines
 */

//TODO:Implement snapshot obj
//VALUE _clone(VALUE self, VALUE rvm, VALUE snapshot,

/* careful... NOT tested yet */
VALUE 
_delete(VALUE self, VALUE rvm, VALUE opts)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;

	int opt = NUM2INT(opts);

	job = VixVM_Delete(vm,opt,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err))
	{
 		fprintf(stderr,"Failed to delete virtual machine:%s\n",Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return Qtrue;
}

VALUE
_read_var(VALUE self, VALUE rvm, VALUE rvartype, VALUE rname)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;

	VALUE retval;

	int vartype = NUM2INT(rvartype);
	char *name = StringValueCStr(rname);
	int opts = 0;
	char *var = NULL;

	job = VixVM_ReadVariable(vm,vartype,name,opts,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_JOB_RESULT_VM_VARIABLE_STRING, &var, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Failed to read variable virtual machine:%s\n",Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	fprintf(stderr,"Read var \"%s\", got (%s)\n",name,var);

	retval = rb_str_new2(var);
	Vix_FreeBuffer(var);
	return retval;
}

VALUE
_write_var(VALUE self, VALUE rvm, VALUE rtype, VALUE rname, VALUE rval)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;
	
	int type = NUM2INT(rtype);
	char *name = StringValueCStr(rname);
	char *val = StringValueCStr(rval);

	job = VixVM_WriteVariable(vm,type,name,val,0,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Failed to write variable (%s) val=(%s): %s\n",name,val,Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return rval;
}

VALUE
_upgrade_vhardware(VALUE self, VALUE rvm)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;

	job = VixVM_UpgradeVirtualHardware(vm,0,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
		
	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Failed to upgrade the virtual hardware: %s\n",Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return Qtrue;
}
