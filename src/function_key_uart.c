#include "stdio.h"
#include "unistd.h"
#include "mraa.h"
#include <mraa/uart.h>

void isr(void *parm)
{
    printf("isr enter\n");
}

int
main(int argc, char** argv)
{
    mraa_uart_context uart1;
    mraa_uart_context uart2;

    uart1 = mraa_uart_init(1);
    uart2 = mraa_uart_init(2);

    if (uart1 == NULL) {
        fprintf(stderr, "UART failed to setup\n");
        return EXIT_FAILURE;
    }
    if (uart2 == NULL) {
        fprintf(stderr, "UART failed to setup\n");
        return EXIT_FAILURE;
    }
    mraa_uart_set_baudrate(uart1, 115200);
    mraa_uart_set_baudrate(uart2, 115200);

    mraa_uart_set_mode(uart1, 8, MRAA_UART_PARITY_NONE, 1);
    mraa_uart_set_mode(uart2, 8, MRAA_UART_PARITY_NONE, 1);

    uint8_t buffer[64] = {0};
    uint8_t buff[100] = {0};
    char good[] = "This is uart!";

    uint8_t res0 = 0;
    uint8_t i = 0;
    uint8_t wakeupcmdbuf[24] = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
                                0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};
    uint8_t readvercmdbuf[9] = {0x00, 0x00, 0xff, 0x02, 0xfe, 0xd4, 0x02, 0x2a, 0x00};

    while(1)
	{
#if 1
		res0 = mraa_uart_write(uart1, wakeupcmdbuf, 24);
		fprintf(stdout, "uart1 send: %d\n", res0);
		sleep(1);
		res0 = mraa_uart_read(uart1, buffer, 15);
		fprintf(stdout, "uart1receive: %d\n", res0);
        for(i = 0; i < 15; i++)
        {
                fprintf(stdout, "buffer[0x%2x]: 0x%02x\n", i, buffer[i]);
        }
#endif
        res0 = mraa_uart_write(uart1, readvercmdbuf, 9);
        fprintf(stdout, "uart1 send: %d\n", res0);
        sleep(1);
        res0 = mraa_uart_read(uart1, buffer, 19);
        fprintf(stdout, "uart1receive: %d\n", res0);
        for(i = 0; i < 19; i++)
        {
                fprintf(stdout, "buffer[0x%2x]: 0x%02x\n", i, buffer[i]);
        }
        fprintf(stdout, "Firmware Version: 0x");
        for(i = 0; i < 3; i++)
        {
                fprintf(stdout, "%02x", buffer[i+13]);
        }
        fprintf(stdout, "\n");

        sleep(3);
	}
    mraa_uart_stop(uart2);
    mraa_uart_stop(uart1);

    mraa_deinit();
    return EXIT_SUCCESS;
/*
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
*/
}
