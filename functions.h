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

/* Misc functions */
VALUE _getproperty(VALUE, VALUE, VALUE);
