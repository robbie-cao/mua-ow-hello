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
    res0 = mraa_i2c_write_word_data(i2c, &buf0, LSM6DS3_CTRL1_XL);
    fprintf(stdout, "Set_addr_0 is %d - 0x%02x - 0x%02x\n", res0, buf0[0], buf0[1]);
    res0 = mraa_i2c_write_word_data(i2c, &buf1, LSM6DS3_CTRL3_C);
    fprintf(stdout, "Set_addr_0 is %d - 0x%02x - 0x%02x\n", res0, buf1[0], buf1[1]);
    res0 = mraa_i2c_write_word_data(i2c, &buf2, LSM6DS3_CTRL5_C);
    fprintf(stdout, "Set_addr_0 is %d - 0x%02x - 0x%02x\n", res0, buf2[0], buf2[1]);
    res0 = mraa_i2c_write_word_data(i2c, &buf3, LSM6DS3_CTRL7_G);
    fprintf(stdout, "Set_addr_0 is %d - 0x%02x - 0x%02x\n", res0, buf3[0], buf3[1]);
    res0 = mraa_i2c_write_word_data(i2c, &buf4, LSM6DS3_CTRL9_XL);
    fprintf(stdout, "Set_addr_0 is %d - 0x%02x - 0x%02x\n", res0, buf4[0], buf4[1]);
#endif
    sleep(1);
    res = mraa_i2c_write(i2c, buf, 10);
    fprintf(stdout, "Write result: 0x%02x\n", res);
    for(i = 0; i < 10; i++) {
        fprintf(stdout, "Write data[%02x] is 0x%02x\n", i, buf[i]);
    }

    mraa_i2c_stop(i2c);

    return 0;
}
