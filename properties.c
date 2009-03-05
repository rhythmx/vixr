#include <stdio.h>
#include <ruby.h>
#include <vix.h>
#include "functions.h"

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
	char *data;
	int len;

	err = Vix_GetProperties(handle,propid,&len,&data,VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) 
	{
		fprintf(stderr,"Error while attempting to retrieve BLOB property (id %d)\n",propid);
		return Qnil;
	}
	
	retval = rb_str_new(data,len);
	Vix_FreeBuffer(data);
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
