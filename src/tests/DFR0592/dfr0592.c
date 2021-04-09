#include <dfr0592.h>

/* This function will not be exported and is not
 * directly callable by users of this library.
 */
int internal_function() {
    int file = 0;
    i2c_smbus_read_byte_data(file, 0x01);
    return 0;
}

int dfr_func() {
    return internal_function();
}
