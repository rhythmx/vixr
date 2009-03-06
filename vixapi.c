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

	/* VM Guest Methods */
	rb_define_singleton_method(rb_cVixAPI,"_capture_screen_image",_capture_screen_image,1);
	rb_define_singleton_method(rb_cVixAPI,"_wait_for_tools",_wait_for_tools,2);
	rb_define_singleton_method(rb_cVixAPI,"_copy_file_from_guest_to_host",_copy_file_from_guest_to_host, 3);
	rb_define_singleton_method(rb_cVixAPI,"_copy_file_from_host_to_guest",_copy_file_from_host_to_guest, 3);
	rb_define_singleton_method(rb_cVixAPI,"_create_directory_in_guest", _create_directory_in_guest, 2);
	rb_define_singleton_method(rb_cVixAPI,"_create_temp_file_in_guest", _create_temp_file_in_guest, 1);
	rb_define_singleton_method(rb_cVixAPI,"_delete_directory_in_guest", _delete_directory_in_guest, 2);
	rb_define_singleton_method(rb_cVixAPI,"_delete_file_in_guest", _delete_file_in_guest, 2);
	rb_define_singleton_method(rb_cVixAPI,"_directory_exists_in_guest", _directory_exists_in_guest, 2);
	rb_define_singleton_method(rb_cVixAPI,"_login_in_guest",_login_in_guest,3);
	rb_define_singleton_method(rb_cVixAPI,"_logout_from_guest",_logout_from_guest,1);
	rb_define_singleton_method(rb_cVixAPI,"_enable_shared_folders",_enable_shared_folders,2);
	rb_define_singleton_method(rb_cVixAPI,"_file_exists_in_guest",_file_exists_in_guest,2);
	rb_define_singleton_method(rb_cVixAPI,"_file_size",_file_size,2);
	rb_define_singleton_method(rb_cVixAPI,"_file_is_directory",_file_is_directory,2);
	rb_define_singleton_method(rb_cVixAPI,"_file_is_symlink",_file_is_symlink,2);
	rb_define_singleton_method(rb_cVixAPI,"_file_mod_time",_file_mod_time,2);
	rb_define_singleton_method(rb_cVixAPI,"_install_tools",_install_tools,1);
	rb_define_singleton_method(rb_cVixAPI,"_kill_process_in_guest",_kill_process_in_guest,2);
	rb_define_singleton_method(rb_cVixAPI,"_list_directory_in_guest",_list_directory_in_guest,2);
	rb_define_singleton_method(rb_cVixAPI,"_list_processes_in_guest",_list_processes_in_guest,1);
	rb_define_singleton_method(rb_cVixAPI,"_open_url_in_guest",_open_url_in_guest,2);
	rb_define_singleton_method(rb_cVixAPI,"_rename_file_in_guest",_rename_file_in_guest,3);
	rb_define_singleton_method(rb_cVixAPI,"_run_program_in_guest",_run_program_in_guest,3);
	rb_define_singleton_method(rb_cVixAPI,"_run_script_in_guest",_run_script_in_guest,3);

	/* Misc methods */
	rb_define_singleton_method(rb_cVixAPI,"_getproperty",_getproperty,2);
}
