#include <stdarg.h>
#include <stdio.h>

#include "log.h"

int main(void)
{
	log_init();

	log_set_level(LOG_LEVEL_ALL);
	LOG_DEBUG("%d\n" , __LINE__);
	LOG_INFO("%d\n"  , __LINE__);
	LOG_WARN("%d\n"  , __LINE__);
	LOG_ERROR("%d\n" , __LINE__);
	LOG_FATAL("%d\n" , __LINE__);

	log_set_level(LOG_LEVEL_WARN);
	LOG_DEBUG("%s\n" , "Hello");
	LOG_INFO("%s\n"  , "Hello");
	LOG_WARN("%s\n"  , "Hello");
	LOG_ERROR("%s\n" , "Hello");
	LOG_FATAL("%s\n" , "Hello");

	log_out(LOG_LEVEL_DEBUG, "%d, %s\n", __LINE__, __FUNCTION__);
	D();

	LOG_FATAL("%s\n" , __FILE__);

	log_set_level(LOG_LEVEL_OFF);
	LOG_DEBUG("%s\n" , __FILE__);
	LOG_INFO("%s\n"  , __FILE__);
	LOG_WARN("%s\n"  , __FILE__);
	LOG_ERROR("%s\n" , __FILE__);
	LOG_FATAL("%s\n" , __FILE__);

	return 0;
}


/* vim: set ts=4 sw=4 tw=0 noexpandtab nolist : */
