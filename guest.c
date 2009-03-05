#include <stdio.h>
#include <ruby.h>
#include "vix.h"
#include "functions.h"

VALUE 
_capture_screen_image(VALUE self, VALUE rvm)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 
	VALUE pngdata;

	job = VixVM_CaptureScreenImage(vm, VIX_CAPTURESCREENFORMAT_PNG, 
								   VIX_INVALID_HANDLE, NULL, NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
		
	if(VIX_FAILED(err)) 
	{
		Vix_ReleaseHandle(job);
		fprintf(stderr,"Failed to capture screen on virtual machine: %s\n", 
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	/* Read the image from teh Job properties */
	pngdata = _getproperty_blob(job, VIX_PROPERTY_JOB_RESULT_SCREEN_IMAGE_DATA);
	Vix_ReleaseHandle(job);

	return pngdata;
}

VALUE 
_wait_for_tools(VALUE self, VALUE rvm, VALUE rtimeout)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;

	int timeout = NUM2INT(rtimeout);

	job = VixVM_WaitForToolsInGuest(vm,timeout,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Error waiting for VMtools in guest: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return Qtrue;
}

VALUE 
_copy_file_from_guest_to_host(VALUE self, VALUE rvm, VALUE rsrc, VALUE rdst)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	char *src = StringValueCStr(rsrc);
	char *dst = StringValueCStr(rdst);

	job = VixVM_CopyFileFromGuestToHost(vm, src, dst, 0, 
										VIX_INVALID_HANDLE, 
										NULL, NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to copy file to host: %s\n", 
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return Qtrue;
}

VALUE 
_copy_file_from_host_to_guest(VALUE self, VALUE rvm, VALUE rsrc, VALUE rdst)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	char *src = StringValueCStr(rsrc);
	char *dst = StringValueCStr(rdst);

	
	job = VixVM_CopyFileFromHostToGuest(vm, src, dst, 0, 
										VIX_INVALID_HANDLE, 
										NULL, NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to copy file to guest: %s\n", 
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return Qtrue;
}

VALUE 
_create_directory_in_guest(VALUE self, VALUE rvm, VALUE rpath)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	char *path = StringValueCStr(rpath);

	job = VixVM_CreateDirectoryInGuest(vm, path, VIX_INVALID_HANDLE, NULL, NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to create directory on guest: %s\n", 
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return Qtrue;
}

VALUE 
_create_temp_file_in_guest(VALUE self, VALUE rvm)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 
	char *tempfilepath;
	
	job = VixVM_CreateTempFileInGuest(vm, 0, VIX_INVALID_HANDLE, NULL, NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_JOB_RESULT_ITEM_NAME, 
					  &tempfilepath, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to create temp file on guest: %s\n", 
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}


	return rb_str_new2(tempfilepath);
}

VALUE 
_delete_directory_in_guest(VALUE self, VALUE rvm, VALUE rpath)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	char *path = StringValueCStr(rpath);

	job = VixVM_DeleteDirectoryInGuest(vm, path, 0, NULL, NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to directory on guest: %s\n", 
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return Qtrue;
}

VALUE 
_delete_file_in_guest(VALUE self, VALUE rvm, VALUE rpath)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	char *path = StringValueCStr(rpath);

	job = VixVM_DeleteFileInGuest(vm, path, NULL, NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to delete file on guest: %s\n", 
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return Qtrue;
}

VALUE 
_directory_exists_in_guest(VALUE self, VALUE rvm, VALUE rpath)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err; 

	char *path = StringValueCStr(rpath);

	job = VixVM_DirectoryExistsInGuest(vm, path, NULL, NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to check directory on guest: %s\n", 
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return Qtrue;
}


VALUE 
_login_in_guest(VALUE self, VALUE rvm, VALUE ruser, VALUE rpass)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;
	char *user=StringValueCStr(ruser);
	char *pass=StringValueCStr(rpass);
	
	job = VixVM_LoginInGuest(vm, user, pass, 0, NULL, NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to login on guest: %s\n", 
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return Qtrue;
}

VALUE 
_logout_from_guest(VALUE self, VALUE rvm)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;
	
	job = VixVM_LogoutFromGuest(vm, NULL, NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err)) 
	{
		fprintf(stderr,"Failed to logout of guest: %s\n", 
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return Qtrue;
}


/*

VALUE 
_enable_shared_folders(VALUE self, VALUE rvm, VALUE enabled)
{

}

VALUE 
_file_exists_in_guest(VALUE self, VALUE rvm)
{
}

VALUE 
_get_file_info_in_guest(VALUE self, VALUE rvm)
{
}

VALUE 
_install_tools(VALUE self, VALUE rvm)
{
}

VALUE 
_kill_process_in_guest(VALUE self, VALUE rvm)
{
}

VALUE 
_list_directory_in_guest(VALUE self, VALUE rvm)
{
}

VALUE 
_list_processes_in_guest(VALUE self, VALUE rvm)
{
}

VALUE 
_open_url_in_guest(VALUE self, VALUE rvm)
{
}

VALUE 
_rename_file_in_guest(VALUE self, VALUE rvm)
{
}

VALUE 
_run_program_in_guest(VALUE self, VALUE rvm)
{
}

VALUE 
_run_script_in_guest(VALUE self, VALUE rvm)
{
}

*/
