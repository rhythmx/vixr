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

VALUE 
_enable_shared_folders(VALUE self, VALUE rvm, VALUE enabled)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;
	int enable = (enabled==Qfalse||enabled==Qnil)?0:1; 

	job = VixVM_EnableSharedFolders(vm,enable,0,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Failed to enable shared folders: %s\n", Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return enabled;
}

VALUE 
_file_exists_in_guest(VALUE self, VALUE rvm, VALUE rpath)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;
	char exists;
	char *path = StringValueCStr(rpath);

	job = VixVM_FileExistsInGuest(vm,path,NULL,NULL);
	err = VixJob_Wait(job, VIX_PROPERTY_JOB_RESULT_GUEST_OBJECT_EXISTS, &exists, VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Failed to check if file exists in guest: %s",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}

	return (exists)?Qtrue:Qfalse;
}

VALUE 
_file_size(VALUE self, VALUE rvm, VALUE rpath)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;
	char *path = StringValueCStr(rpath);
	int64 size;

	job = VixVM_GetFileInfoInGuest(vm,path,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_JOB_RESULT_FILE_SIZE,&size,VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Error while obtaining file info: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return INT2NUM(size);
}

VALUE 
_file_is_directory(VALUE self, VALUE rvm, VALUE rpath)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;
	char *path = StringValueCStr(rpath);
	int flags;

	job = VixVM_GetFileInfoInGuest(vm,path,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_JOB_RESULT_FILE_FLAGS,&flags,
					  VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Error while obtaining file info: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return (flags&VIX_FILE_ATTRIBUTES_DIRECTORY)?Qtrue:Qfalse;
}

VALUE 
_file_is_symlink(VALUE self, VALUE rvm, VALUE rpath)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;
	char *path = StringValueCStr(rpath);
	int flags;

	job = VixVM_GetFileInfoInGuest(vm,path,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_JOB_RESULT_FILE_FLAGS,&flags,
					  VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Error while obtaining file info: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return (flags&VIX_FILE_ATTRIBUTES_SYMLINK)?Qtrue:Qnil;
}

VALUE 
_file_mod_time(VALUE self, VALUE rvm, VALUE rpath)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;
	char *path = StringValueCStr(rpath);
	int64 modtime;

	job = VixVM_GetFileInfoInGuest(vm,path,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_JOB_RESULT_FILE_MOD_TIME,&modtime,
					  VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);
	
	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Error while obtaining file info: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}
	
	return INT2NUM(modtime);
}

VALUE 
_install_tools(VALUE self, VALUE rvm)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;	

	job = VixVM_InstallTools(vm,0,NULL,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Error attemping to install vmtools: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}	

	return Qtrue;
}

VALUE 
_kill_process_in_guest(VALUE self, VALUE rvm, VALUE rpid)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;	
	
	uint64 pid = NUM2INT(rpid);

	job = VixVM_KillProcessInGuest(vm,pid,0,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Error killing process id (%#016llx): %s\n",pid,
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}	

	return Qtrue;
}

VALUE 
_list_directory_in_guest(VALUE self, VALUE rvm, VALUE rdir)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;	

	int idx,num;
	char *name = StringValueCStr(rdir);
	VALUE array = rb_eval_string("Array.new()");

	job = VixVM_ListDirectoryInGuest(vm,name,0,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_NONE);

	if(VIX_FAILED(err))
	{
		Vix_ReleaseHandle(job);
		fprintf(stderr,"Error listing directory: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}	

	/* Get each of the results */
	num=VixJob_GetNumProperties(job,VIX_PROPERTY_JOB_RESULT_ITEM_NAME);
	for(idx=0;idx<num;idx++)
	{
		char *name;
		err = VixJob_GetNthProperties(job,idx,
									  VIX_PROPERTY_JOB_RESULT_ITEM_NAME,
									  &name,
									  VIX_PROPERTY_NONE);
		if(VIX_FAILED(err))
		{
			Vix_ReleaseHandle(job);
			fprintf(stderr,"Failed while reading directory: %s\n",
					Vix_GetErrorText(err,NULL));
			return Qnil;
		}
		rb_funcall(array,rb_intern("<<"),1,rb_str_new2(name));
		Vix_FreeBuffer(name);
	}
	
	Vix_ReleaseHandle(job);

	return array;
}

VALUE 
_list_processes_in_guest(VALUE self, VALUE rvm)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;	

	int idx,num;
	VALUE array = rb_eval_string("Array.new()");
	VALUE hash;
	char *name;
	char *owner;
	char *cmdline;
	uint64 pid;


	job = VixVM_ListProcessesInGuest(vm,0,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_NONE);

	if(VIX_FAILED(err))
	{
		Vix_ReleaseHandle(job);
		fprintf(stderr,"Error listing processes: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}	

	/* Get each of the results */
	num=VixJob_GetNumProperties(job,VIX_PROPERTY_JOB_RESULT_ITEM_NAME);
	for(idx=0;idx<num;idx++)
	{
		err = VixJob_GetNthProperties(job,idx,
									  VIX_PROPERTY_JOB_RESULT_ITEM_NAME, 
									  &name,
									  VIX_PROPERTY_JOB_RESULT_PROCESS_ID, 
									  &pid,
									  VIX_PROPERTY_JOB_RESULT_PROCESS_OWNER, 
									  &owner,
									  VIX_PROPERTY_JOB_RESULT_PROCESS_COMMAND, 
									  &cmdline,
									  VIX_PROPERTY_NONE);
		if(VIX_FAILED(err))
		{
			Vix_ReleaseHandle(job);
			fprintf(stderr,"Failed while listing processes: %s\n",
					Vix_GetErrorText(err,NULL));
			return Qnil;
		}
		hash = rb_eval_string("Hash.new()");
		rb_funcall(hash,rb_intern("[]="),2,rb_str_new2("name"),rb_str_new2(name));
		rb_funcall(hash,rb_intern("[]="),2,rb_str_new2("pid"),INT2NUM(pid));
		rb_funcall(hash,rb_intern("[]="),2,rb_str_new2("owner"),rb_str_new2(owner));
		rb_funcall(hash,rb_intern("[]="),2,rb_str_new2("cmdline"),rb_str_new2(cmdline));
		rb_funcall(array,rb_intern("<<"),1,hash);
		Vix_FreeBuffer(name);
		Vix_FreeBuffer(owner);
		Vix_FreeBuffer(cmdline);
	}
	
	Vix_ReleaseHandle(job);

	return array;
}

VALUE 
_open_url_in_guest(VALUE self, VALUE rvm, VALUE rurl)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;	
	
	char *url = StringValueCStr(rurl);

	job = VixVM_OpenUrlInGuest(vm,url,0,VIX_INVALID_HANDLE,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Error opening URL in guest: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}	

	return Qtrue;
}

VALUE 
_rename_file_in_guest(VALUE self, VALUE rvm, VALUE rsrc, VALUE rdst)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;	
	
	char *src = StringValueCStr(rsrc);
	char *dst = StringValueCStr(rdst);

	job = VixVM_RenameFileInGuest(vm,src,dst,0,VIX_INVALID_HANDLE,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Error renaming file in guest: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}	

	return Qtrue;	
}

VALUE 
_run_program_in_guest(VALUE self, VALUE rvm, VALUE rcmd, VALUE rargs)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;	
	
	char *cmd = StringValueCStr(rcmd);
	char *args = StringValueCStr(rargs);
	job = VixVM_RunProgramInGuest(vm,cmd,args,0,VIX_INVALID_HANDLE,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Error running command in guest: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}	

	return Qtrue;	
}

VALUE 
_run_script_in_guest(VALUE self, VALUE rvm, VALUE rintp, VALUE rtext)
{
	VixHandle vm = NUM2INT(rb_iv_get(rvm,"@handle"));
	VixHandle job;
	VixError err;	
	
	char *intp = StringValueCStr(rintp);
	char *text = StringValueCStr(rtext);
	job = VixVM_RunScriptInGuest(vm,intp,text,0,VIX_INVALID_HANDLE,NULL,NULL);
	err = VixJob_Wait(job,VIX_PROPERTY_NONE);
	Vix_ReleaseHandle(job);

	if(VIX_FAILED(err))
	{
		fprintf(stderr,"Error running script in guest: %s\n",
				Vix_GetErrorText(err,NULL));
		return Qnil;
	}	

	return Qtrue;	
}

