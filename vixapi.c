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
	rb_define_singleton_method(rb_cVixAPI,"_login_in_guest",_login_in_guest,3);
	rb_define_singleton_method(rb_cVixAPI,"_logout_from_guest",_logout_from_guest,1);
	
	/* Misc methods */
	rb_define_singleton_method(rb_cVixAPI,"_getproperty",_getproperty,2);
}
