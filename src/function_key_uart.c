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
    mraa_uart_set_baudrate(uart1,115200);
    mraa_uart_set_baudrate(uart2,115200);

    mraa_uart_set_mode(uart1,8,MRAA_UART_PARITY_NONE,1);
    mraa_uart_set_mode(uart2,8,MRAA_UART_PARITY_NONE,1);

    char buffer[1000]={0};
    char buff[1000];
    char good[]="This is uart!";
    while(gets(buffer)!=NULL)
	{
		mraa_uart_write(uart1,buffer,10);
		printf("uart1 send:");
		puts(buffer);
		mraa_uart_read(uart2,buff,1000);
		printf("uart2 receive:");
		puts(buff);
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
