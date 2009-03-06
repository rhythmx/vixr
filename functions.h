/* Auth/Session routines */
VALUE _connect(VALUE, VALUE, VALUE, VALUE, VALUE, VALUE);
VALUE _disconnect(VALUE, VALUE);

VALUE _open_vmx(VALUE, VALUE, VALUE);

/* VM functions */
VALUE _power_on(VALUE, VALUE, VALUE);
VALUE _power_off(VALUE, VALUE, VALUE);
VALUE _pause(VALUE, VALUE);
VALUE _suspend(VALUE, VALUE);
VALUE _unpause(VALUE, VALUE);
VALUE _reset(VALUE, VALUE, VALUE);
VALUE _delete(VALUE, VALUE, VALUE);
VALUE _upgrade_vhardware(VALUE, VALUE);
VALUE _read_var(VALUE, VALUE, VALUE, VALUE);
VALUE _write_var(VALUE, VALUE, VALUE, VALUE, VALUE);

/* VM Guest functions */
VALUE _capture_screen_image(VALUE, VALUE);
VALUE _wait_for_tools(VALUE,VALUE,VALUE);
VALUE _copy_file_from_guest_to_host(VALUE,VALUE,VALUE,VALUE);
VALUE _copy_file_from_host_to_guest(VALUE,VALUE,VALUE,VALUE);
VALUE _create_directory_in_guest(VALUE, VALUE, VALUE);
VALUE _create_temp_file_in_guest(VALUE, VALUE);
VALUE _delete_directory_in_guest(VALUE, VALUE, VALUE);
VALUE _delete_file_in_guest(VALUE, VALUE, VALUE);
VALUE _directory_exists_in_guest(VALUE, VALUE, VALUE);
VALUE _login_in_guest(VALUE, VALUE, VALUE, VALUE);
VALUE _logout_from_guest(VALUE, VALUE);
VALUE _enable_shared_folders(VALUE, VALUE, VALUE);

/* Misc functions */
VALUE _getproperty(VALUE, VALUE, VALUE);
VALUE _getproperty_int(VixHandle, int);
VALUE _getproperty_str(VixHandle, int);
VALUE _getproperty_bool(VixHandle, int);
VALUE _getproperty_handle(VixHandle, int);
VALUE _getproperty_int64(VixHandle, int);
VALUE _getproperty_blob(VixHandle, int);
