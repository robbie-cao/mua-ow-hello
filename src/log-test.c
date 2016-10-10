#include <stdarg.h>
#include <stdio.h>

#include "log.h"

int main(void)
{
	log_open("mua0", LOG_CHANNEL_CONSOLE);

	log_set_level(LOG_DEBUG);
	log_debug("%d\n" , __LINE__);
	log_info("%d\n"  , __LINE__);
	log_warn("%d\n"  , __LINE__);
	log_err("%d\n" , __LINE__);

	log_set_level(LOG_ERR);
	log_debug("%s\n" , "hello");
	log_info("%s\n"  , "hello");
	log_warn("%s\n"  , "hello");
	log_err("%s\n" , "hello");
	log_alert("%s\n" , "hello");

	log_output(LOG_ALERT, "%d, %s\n", __LINE__, __FUNCTION__);
	D();

	log_alert("%s\n" , __FILE__);

	log_set_level(LOG_OFF);
	log_debug("%s\n" , __FILE__);
	log_info("%s\n"  , __FILE__);
	log_warn("%s\n"  , __FILE__);
	log_err("%s\n" , __FILE__);

	log_close();

	log_open("mua1", LOG_CHANNEL_CONSOLE | LOG_CHANNEL_SYSLOG);

	log_set_level(LOG_DEBUG);
	log_debug("%d\n" , __LINE__);
	log_info("%d\n"  , __LINE__);
	log_warn("%d\n"  , __LINE__);
	log_err("%d\n" , __LINE__);

	log_set_level(LOG_ERR);
	log_debug("%s\n" , "hello");
	log_info("%s\n"  , "hello");
	log_warn("%s\n"  , "hello");
	log_err("%s\n" , "hello");
	log_alert("%s\n" , "hello");

	log_output(LOG_ALERT, "%d, %s\n", __LINE__, __FUNCTION__);
	D();

	log_alert("%s\n" , __FILE__);

	log_set_level(LOG_OFF);
	log_debug("%s\n" , __FILE__);
	log_info("%s\n"  , __FILE__);
	log_warn("%s\n"  , __FILE__);
	log_err("%s\n" , __FILE__);

	log_close();

	return 0;
}


/* vim: set ts=4 sw=4 tw=0 noexpandtab nolist : */
