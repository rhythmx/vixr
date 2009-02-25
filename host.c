#include <stdio.h>
#include <string.h>
#include <ruby.h>
#include "vix.h"

#include "host.h"

/* _connect(user,pass,type): async connect */
VALUE
_connect(int hosttype, const char *hostname, int port, const char *user, const char *pass)
{
	VixHandle job = 0;
	VixHandle session = VIX_INVALID_HANDLE;
	VixError err;

	fprintf(stderr,"calling connect(ht:%d,host:%s,port:%d,user:%s,pass.length:%d);\n",hosttype,hostname,port,user,(pass)?strlen(pass):0);

	/* Start the async connect */
	job = VixHost_Connect(VIX_API_VERSION,
		hosttype,hostname,port,user,pass,
		/*VIX_HOSTOPTION_USE_EVENT_PUMP,*/ 0, VIX_INVALID_HANDLE, NULL,NULL
	);

	/* Wait on connect */
	err = VixJob_Wait(job, VIX_PROPERTY_JOB_RESULT_HANDLE, &session, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to connect to vmware host machine\n");
		return Qnil;
	}

	fprintf(stderr,"hostHandle is %d\n",session);
	
	/* @session */
	return INT2NUM(session);
}

VALUE
_connect_server(VALUE self, VALUE rhosttype, VALUE rhostname, VALUE rport, VALUE ruser, VALUE rpass)
{
	char *hostname = StringValueCStr(rhostname);
	int  hosttype  = NUM2INT(rhosttype);
	int  port      = NUM2INT(rport);
	char *user     = StringValueCStr(ruser);
	char *pass     = StringValueCStr(rpass);

	return _connect(hosttype,hostname,port,user,pass);
}

VALUE
_connect_wkstn(VALUE self)
{
	return _connect(VIX_SERVICEPROVIDER_VMWARE_WORKSTATION, NULL, 0, NULL, NULL);
}

VALUE
_vmx_open(VALUE self,VALUE rsession, VALUE rpath)
{
	char *path = StringValueCStr(rpath);
	int session = NUM2INT(rsession);

	VixHandle job;
	VixHandle vm = VIX_INVALID_HANDLE;
	VixError err;

	fprintf(stderr,"calling vmx_open(%d,\"%s\");\n",session,path);

	job = VixVM_Open(session,path,NULL,NULL);

	err = VixJob_Wait(job, VIX_PROPERTY_JOB_RESULT_HANDLE, &vm, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed open vmx file %s: \n",path, Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	fprintf(stderr,"VM handle is %d\n",vm);	

	return INT2NUM(vm);
} 


