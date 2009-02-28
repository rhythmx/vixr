#include <stdio.h>
#include <ruby.h>
#include <vix.h>

/* local methods */
#include "functions.h"

/*
 * Globals
 */
static VALUE started = Qnil;
static VALUE rb_cVixAPI; /* module VixAPI    */

VALUE _getproperty_int(VixHandle handle, int propid)
{
	VixError err;
	int result;

	err = Vix_GetProperties(handle,propid,&result,VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) 
	{
		fprintf(stderr,"Error while attempting to retrieve INT property (id %d)\n",propid);
		return Qnil;
	}
	
	return INT2NUM(result);
}

VALUE _getproperty_str(VixHandle handle, int propid)
{
	VixError err;
	char *result;
	VALUE retval;

	err = Vix_GetProperties(handle,propid,&result,VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) 
	{
		fprintf(stderr,"Error while attempting to retrieve string property (id %d)\n",propid);
		return Qnil;
	}
	
	retval = rb_str_new2(result);
	Vix_FreeBuffer(result);
	return retval;
}

VALUE _getproperty_bool(VixHandle handle, int propid)
{
	VixError err;
	char result;

	err = Vix_GetProperties(handle,propid,&result,VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) 
	{
		fprintf(stderr,"Error while attempting to retrieve BOOL property (id %d)\n",propid);
		return Qnil;
	}

	//fprintf(stderr,"attempting to retrieve BOOL property (id %d), res=%d\n",propid,result);
	
	return (result)?Qtrue:Qfalse;
}

VALUE _getproperty_handle(VixHandle handle, int propid)
{
	VixError err;
	VixHandle result;
	char str[200];

	err = Vix_GetProperties(handle,propid,&result,VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) 
	{
		fprintf(stderr,"Error while attempting to retrieve HANDLE property (id %d)\n",propid);
		return Qnil;
	}
	
	sprintf(str,"VixAPI::Handle.new(%d)",result);
	return  rb_eval_string(str);
}

VALUE _getproperty_int64(VixHandle handle, int propid)
{
	VixError err;
	int64 result;

	err = Vix_GetProperties(handle,propid,&result,VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) 
	{
		fprintf(stderr,"Error while attempting to retrieve INT64 property (id %d)\n",propid);
		return Qnil;
	}
	
	return INT2NUM(result);
}

VALUE _getproperty_blob(VixHandle handle, int propid)
{
	VixError err;
	VALUE retval;
	struct {int len;char *data;} result;

	err = Vix_GetProperties(handle,propid,&result,VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) 
	{
		fprintf(stderr,"Error while attempting to retrieve BLOB property (id %d)\n",propid);
		return Qnil;
	}
	
	retval = rb_str_new(result.data,result.len);
	Vix_FreeBuffer(result.data);
	return retval;
}


VALUE _getproperty(VALUE self, VALUE rhandleobj, VALUE rpropid)
{
	VixHandle handle;
	VixError err;
	VixPropertyType type;
	int propid;

	handle = NUM2INT(rb_iv_get(rhandleobj,"@handle"));
	propid = NUM2INT(rpropid);

	/* Get the type of data to return */
	if(VIX_FAILED(err = Vix_GetPropertyType(handle,propid,&type)))
	{
		fprintf(stderr,"Could not determine property type: %s\n",Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	/* do diff things based on prop type */
	switch(type)
	{
	case VIX_PROPERTYTYPE_INTEGER:
		return _getproperty_int(handle,propid);
	case VIX_PROPERTYTYPE_STRING:
		return _getproperty_str(handle,propid);
	case VIX_PROPERTYTYPE_BOOL:
		return _getproperty_bool(handle,propid);
	case VIX_PROPERTYTYPE_HANDLE:
		return _getproperty_handle(handle,propid);
	case VIX_PROPERTYTYPE_INT64:
		return _getproperty_int64(handle,propid);
	case VIX_PROPERTYTYPE_BLOB:
		return _getproperty_blob(handle,propid);
	case VIX_PROPERTYTYPE_ANY: default:
		break;
	}

	fprintf(stderr,"Unknown Property Type (%d)\n",type);
	return Qnil;
}

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

void 
Init_vixapi() 
{
	rb_cVixAPI = rb_define_module("VixAPI");

	/* global init function */
	rb_define_singleton_method(rb_cVixAPI, "start", vix_start, 0);

	/* VixAPI wrapper methods */
	rb_define_singleton_method(rb_cVixAPI,"_connect",_connect,5);
	rb_define_singleton_method(rb_cVixAPI,"_disconnect",_disconnect,1);
	rb_define_singleton_method(rb_cVixAPI,"_open_vmx",_open_vmx,2);

	/* VM methods */
	rb_define_singleton_method(rb_cVixAPI,"_power_on",_power_on,2);
	rb_define_singleton_method(rb_cVixAPI,"_power_off",_power_off,2);
	rb_define_singleton_method(rb_cVixAPI,"_pause",_pause,1);
	rb_define_singleton_method(rb_cVixAPI,"_suspend",_suspend,1);
	rb_define_singleton_method(rb_cVixAPI,"_unpause",_unpause,1);
	rb_define_singleton_method(rb_cVixAPI,"_reset",_reset,2);
	rb_define_singleton_method(rb_cVixAPI,"_delete",_delete,2);
	rb_define_singleton_method(rb_cVixAPI,"_upgrade_vhardware",_upgrade_vhardware,1);
	rb_define_singleton_method(rb_cVixAPI,"_read_var",_read_var,3);
	rb_define_singleton_method(rb_cVixAPI,"_write_var",_write_var,4);
	
	/* Misc methods */
	rb_define_singleton_method(rb_cVixAPI,"_getproperty",_getproperty,2);
}
