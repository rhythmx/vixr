/* VM Power Functions */
VALUE _power_on(VALUE, VALUE, VALUE);
VALUE _power_off(VALUE, VALUE, VALUE);
VALUE _reset(VALUE, VALUE, VALUE);
VALUE _pause(VALUE, VALUE);
VALUE _suspend(VALUE, VALUE);
VALUE _unpause(VALUE, VALUE);

/* Misc VM Functions */
VALUE _delete(VALUE, VALUE, VALUE);
VALUE _read_var(VALUE, VALUE, VALUE, VALUE);
VALUE _write_var(VALUE, VALUE, VALUE, VALUE, VALUE);
VALUE _upgrade_vhardware(VALUE, VALUE);

