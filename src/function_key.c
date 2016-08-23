#include "stdio.h"
#include "unistd.h"
#include "mraa.h"

#define GSENSOR_I2C_ADDR                                0x6A
#define LSM6DS3_XG_WHO_AM_I_ADDR                        0x0F

void isr(void *parm)
{
    printf("isr enter\n");
}

int
main(int argc, char** argv)
{
    int res0 = 0;
    int res1 = 0;

    mraa_init();
    fprintf(stdout, "MRAA Version: %s\nStarting Read on IO16\n", mraa_get_version());

                //! [Interesting]
#if 0
    mraa_gpio_context gpio;

    gpio = mraa_gpio_init(16);
    mraa_gpio_dir(gpio, MRAA_GPIO_IN);

    //for (;;) {
    //    fprintf(stdout, "Gpio is %d\n", mraa_gpio_read(gpio));
    //    sleep(1);
    //}i

    printf("ISR TETS\r\n");
    mraa_gpio_isr(gpio,MRAA_GPIO_EDGE_BOTH,&isr,NULL);
    printf("..");

    while(1);

    mraa_gpio_close(gpio);
            //! [Interesting]
#endif

    mraa_i2c_context i2c;
    i2c = mraa_i2c_init(0);

    fprintf(stdout, "i2c is %d\n", i2c);

    res0 = mraa_i2c_address(i2c, GSENSOR_I2C_ADDR);
    fprintf(stdout, "Set_addr is %d\n", res0);

    res1 = mraa_i2c_read_byte_data(i2c, LSM6DS3_XG_WHO_AM_I_ADDR);
    fprintf(stdout, "Read data is 0x%02x\n", res1);

    return 0;
}
