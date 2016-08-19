#include "stdio.h"
#include "unistd.h"
#include "mraa.h"


void isr(void *parm)
{
    printf("isr enter\n");
}

int
main(int argc, char** argv)
{
    mraa_init();
    fprintf(stdout, "MRAA Version: %s\nStarting Read on IO16\n", mraa_get_version());

                //! [Interesting]
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

    return 0;
}
