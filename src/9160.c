#include "stdio.h"
#include "unistd.h"
#include "mraa.h"
#include "lsm6ds3.h"


#define ISD9160_I2C_ADDR            0x50

static mraa_i2c_context i2c;

int
main(int argc, char** argv)
{
    uint8_t buf[16];
    uint8_t res = 0;
    int i = 0;

    mraa_init();
    fprintf(stdout, "MRAA Version: %s\n", mraa_get_version());

    i2c = mraa_i2c_init(0);

    fprintf(stdout, "i2c is %d\n", i2c);

    mraa_i2c_address(i2c, ISD9160_I2C_ADDR);

    buf[0] = 10;
    buf[1] = 0x30;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 60;    // High or Low?
    buf[5] = 0;     // High or Low?
    buf[6] = 0x00;
    buf[7] = 0x00;
    buf[8] = 0x00;
    buf[9] = 0x00;
#if 0
    res = mraa_i2c_write(i2c, buf, 10);
#else
    res = mraa_i2c_write_data(i2c, buf, 10);
#endif
    fprintf(stdout, "Write result: 0x%02x\n", res);
    for(i = 0; i < 10; i++) {
        fprintf(stdout, "Write data[%02x] is 0x%02x\n", i, buf[i]);
    }

    mraa_i2c_stop(i2c);

    return 0;
}
