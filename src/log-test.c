#include <stdarg.h>
#include <stdio.h>

#include "log.h"

int main(void)
{
	log_init();

	LOG_DEBUG("%d\n" , __LINE__);
	LOG_INFO("%d\n"  , __LINE__);
	LOG_WARN("%d\n"  , __LINE__);
	LOG_ERROR("%d\n" , __LINE__);
	LOG_FATAL("%d\n" , __LINE__);

	LOG_DEBUG("%s\n" , "Hello");
	LOG_INFO("%s\n"  , "Hello");
	LOG_WARN("%s\n"  , "Hello");
	LOG_ERROR("%s\n" , "Hello");
	LOG_FATAL("%s\n" , "Hello");

	log_out(LOG_LEVEL_DEBUG, "%d, %s\n", __LINE__, __FUNCTION__);
	D();

	return 0;
}


/* vim: set ts=4 sw=4 tw=0 noexpandtab nolist : */
