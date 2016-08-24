#include "stdio.h"
#include "unistd.h"
#include "mraa.h"
#include "lsm6ds3.h"

void isr(void *parm)
{
    printf("isr enter\n");
}

int
main(int argc, char** argv)
{
    int res0 = 0;
    int res1 = 0;
    uint16_t buf0[2] = {0x00, 0x00};
    uint16_t buf1[2] = {0x00, 0x00};
    uint16_t buf2[2] = {0x00, 0x00};
    uint16_t buf3[2] = {0x00, 0x00};
    uint16_t buf4[2] = {0x00, 0x00};
    uint8_t buf5 = 0;

    mraa_init();
    fprintf(stdout, "MRAA Version: %s\nStarting Read on IO16\n", mraa_get_version());

    mraa_i2c_context i2c;
    i2c = mraa_i2c_init(0);

    fprintf(stdout, "i2c is %d\n", i2c);

    mraa_i2c_address(i2c, GSENSOR_I2C_ADDR);

    buf0[0] = 0x30;
    buf0[1] = 0x00;
    buf1[0] = 0x44;
    buf1[1] = 0x00;
    buf2[0] = 0x00;
    buf2[1] = 0x00;
    buf3[0] = 0x00;
    buf3[1] = 0x00;
    buf4[0] = 0x38;
    buf4[1] = 0x00;
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

    sleep(1);
    res1 = mraa_i2c_read_byte_data(i2c, LSM6DS3_STATUS_REG);
    fprintf(stdout, "Read data_1 is 0x%02x - 0x%02x\n", res1, LSM6DS3_STATUS_REG);
    if (res1) {
        res0 = mraa_i2c_read_byte_data(i2c, LSM6DS3_OUTX_L_XL);
        fprintf(stdout, "Read data_1 is 0x%02x - 0x%02x\n", res0, LSM6DS3_OUTX_L_XL);
    }
    buf5 = 0x12;
    res0 = mraa_i2c_write_byte_data(i2c, &buf5, LSM6DS3_FIFO_CTRL3);
    fprintf(stdout, "Set_addr_0 is %d - 0x%02x\n", res0, buf5);

    sleep(1);
    res1 = mraa_i2c_read_byte_data(i2c, LSM6DS3_FIFO_CTRL3);
    fprintf(stdout, "Read data_1 is 0x%02x\n", res1);

    return 0;
}
