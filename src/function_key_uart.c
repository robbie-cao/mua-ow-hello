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

    mraa_init();
    fprintf(stdout, "MRAA Version: %s\nStarting Read Card\n", mraa_get_version());

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
    uint8_t foundCard = 0;
    uint8_t res0 = 0;
    uint8_t i = 0;
    uint8_t wakeupcmdbuf[24] = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
                                0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};
    uint8_t readvercmdbuf[9] = {0x00, 0x00, 0xff, 0x02, 0xfe, 0xd4, 0x02, 0x2a, 0x00};
    uint8_t readuidcmdbuf[11] = {0x00, 0x00, 0xff, 0x04, 0xfc, 0xd4, 0x4a, 0x01, 0x00, 0xe1, 0x00};

    // Cmd:0x14; mode:01-normal,02-virtual; timeout:0x14; irq:00-disable,01-enable
    uint8_t samconfcmdbuf[12] = {0x00, 0x00, 0xff, 0x05, 0xfb, 0xd4, 0x14, 0x01, 0x14, 0x01, 0x02, 0x00};

    mraa_uart_write(uart1, wakeupcmdbuf, 24);
    sleep(1);
    res0 = mraa_uart_read(uart1, buffer, 15);
    fprintf(stdout, "wakeup res: %d\n", res0);
    for (i = 0; i < 15; i++)
    {
        fprintf(stdout, " %02x", buffer[i]);
    }
    fprintf(stdout, "\n");
    memset(buffer, 0x0, sizeof(buffer));

    mraa_uart_write(uart1, readvercmdbuf, 9);
    sleep(1);
    mraa_uart_read(uart1, buffer, 19);
    for (i = 0; i < 19; i++)
    {
        fprintf(stdout, " %02x", buffer[i]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "Firmware Version: 0x");
    for (i = 0; i < 3; i++)
    {
        fprintf(stdout, "%02x", buffer[i+13]);
    }
    fprintf(stdout, "\n");
    memset(buffer, 0x0, sizeof(buffer));

    res0 =  mraa_uart_write(uart1, samconfcmdbuf, 12);
    fprintf(stdout, "samconfig write res: %d\n", res0);
    sleep(1);
    mraa_uart_read(uart1, buffer, 15);
    fprintf(stdout, "samconf:\n");
    for (i = 0; i < 20; i++)
    {
        fprintf(stdout, " %02x", buffer[i]);
    }
    fprintf(stdout, "\n");
    memset(buffer, 0x0, sizeof(buffer));

    mraa_uart_write(uart1, readuidcmdbuf, 11);
    sleep(1);
    mraa_uart_read(uart1, buffer, 25);

    while(!foundCard)
	{
        if (buffer[9] != 0)
        {
            fprintf(stdout, "Read card data:\n");
            for (i = 0; i < 25; i++)
            {
                fprintf(stdout, " %02x", buffer[i]);
            }
            fprintf(stdout, "\n");
            fprintf(stdout, "Card uid: 0x");
            for (i = 0; i < (buffer[9] - 1); i++)
            {
                fprintf(stdout, "%02x", buffer[i+13]); // Active card uid data lengt is buffer[9], it behind of 0x4b, outside of the lastest data
            }
            fprintf(stdout, "\n");
            memset(buffer, 0x0, sizeof(buffer));
            sleep(1);
	    }
        else
        {
            fprintf(stdout, "Waiting for a Card\n");
            sleep(2);
            mraa_uart_write(uart1, wakeupcmdbuf, 24);
            sleep(1);
            mraa_uart_read(uart1, buffer, 15);
            memset(buffer, 0x0, sizeof(buffer));
            mraa_uart_write(uart1, readuidcmdbuf, 11);
            sleep(1);
            mraa_uart_read(uart1, buffer, 25);
        }
    }
    mraa_uart_stop(uart2);
    mraa_uart_stop(uart1);

    mraa_deinit();
    return EXIT_SUCCESS;
}
