#include "stdio.h"
#include "unistd.h"
#include "mraa.h"

int
main(int argc, char** argv)
{
    mraa_init();
    fprintf(stdout, "MRAA Version: %s\nStarting Read on IO6\n", mraa_get_version());

                //! [Interesting]
    mraa_gpio_context gpio;

    gpio = mraa_gpio_init(15);
    mraa_gpio_dir(gpio, MRAA_GPIO_IN);

    for (;;) {
        fprintf(stdout, "Gpio is %d\n", mraa_gpio_read(gpio));
        sleep(1);
    }

    mraa_gpio_close(gpio);
            //! [Interesting]

    return 0;
}
