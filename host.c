#include <stdio.h>
#include <string.h>
#include <ruby.h>
#include <vix.h>

#include "functions.h"

/* _connect(user,pass,type): async connect */
VALUE
_connect(VALUE self, VALUE rhosttype, VALUE rhostname, VALUE rport, VALUE ruser, VALUE rpass)
{
	VixHandle job = 0;
	VixHandle session = VIX_INVALID_HANDLE;
	VixError err;

	char *hostname,*pass,*user,str[200];
	int  hosttype,port;

	hosttype = NUM2INT(rhosttype);
	hostname = (rhostname!=Qnil)?StringValueCStr(rhostname):NULL;
	port     = (rport!=Qnil)?NUM2INT(rport):0;
	user     = (ruser!=Qnil)?StringValueCStr(ruser):NULL;
	pass     = (rpass!=Qnil)?StringValueCStr(rpass):NULL;

	//fprintf(stderr,"calling connect(ht:%d,host:%s,port:%d,user:%s,pass.length:%d);\n",hosttype,hostname,port,user,(pass)?strlen(pass):0);

	/* Start the async connect */
	job = VixHost_Connect(VIX_API_VERSION,
						  hosttype,hostname,port,user,pass,
						  /*VIX_HOSTOPTION_USE_EVENT_PUMP,*/ 
						  0, VIX_INVALID_HANDLE, NULL,NULL);

	/* Wait on connect */
	err = VixJob_Wait(job, VIX_PROPERTY_JOB_RESULT_HANDLE, &session, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err) || session == VIX_INVALID_HANDLE) 
	{
		fprintf(stderr,"Failed to connect to vmware host machine: %s\n", 
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	sprintf(str,"VixAPI::Handle.new(%d)",session);
	return  rb_eval_string(str);
}

VALUE
_disconnect(VALUE self, VALUE rhandle)
{
	VixHandle host = NUM2INT(rhandle); 
	VixHost_Disconnect(host);
	return Qtrue;
}



VALUE
_open_vmx(VALUE self,VALUE rhosthandle, VALUE rpath)
{
	VixHandle host = NUM2INT(rb_iv_get(rhosthandle,"@handle"));
	VixHandle job;
	VixHandle vm = VIX_INVALID_HANDLE;
	VixError err;
	char *path = StringValueCStr(rpath);
	char str[200];

	// fprintf(stderr,"calling vmx_open(%d,\"%s\");\n",session,path);

	job = VixVM_Open(host,path,NULL,NULL);

	err = VixJob_Wait(job, VIX_PROPERTY_JOB_RESULT_HANDLE, &vm, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed open vmx file (@%s): %s\n",path, Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	// fprintf(stderr,"VM handle is %d\n",vm);	

	sprintf(str,"VixAPI::Handle.new(%d)",(int)vm);
	return  rb_eval_string(str);
} 


static void
VixDiscoveryProc(VixHandle jobHandle, VixEventType eventType, VixHandle moreEventInfo, void *clientData)
{
	VALUE arr = (VALUE)clientData;
	VixError err = VIX_OK;
	char *url = NULL;
	VALUE str;

	// Check callback event; ignore progress reports.
	if (VIX_EVENTTYPE_FIND_ITEM != eventType) {
	  return;
	}

	// Found a virtual machine.
	err = Vix_GetProperties(moreEventInfo,
									VIX_PROPERTY_FOUND_ITEM_LOCATION,
									&url,
									VIX_PROPERTY_NONE);
	if (VIX_OK != err) {
	  // Handle the error...
	  Vix_FreeBuffer(url);
	  return;
	}

	// puts(url);
	str = rb_str_new2(url);
	rb_ary_push(arr, str);
	Vix_FreeBuffer(url);
}


VALUE
_find_items(VALUE self, VALUE rhosthandle, VALUE rstype)
{
	VixHandle job;
	VixHandle host;
	int stype;
	VixError err;
	VALUE arr;

	host = NUM2INT(rb_iv_get(rhosthandle,"@handle"));
	stype = (rstype!=Qnil)?NUM2INT(rstype):0;
	arr = rb_ary_new();
	job = VixHost_FindItems(host, stype,
									VIX_INVALID_HANDLE, // searchCriteria
									-1, // timeout
									VixDiscoveryProc,
									(void *)arr);

	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	if (VIX_FAILED(err)) {
	  // Handle the error...
	  printf("ERROR in _find_items: %s\n", Vix_GetErrorText(err, NULL));
	}

   Vix_ReleaseHandle(job);
   return arr;
}
