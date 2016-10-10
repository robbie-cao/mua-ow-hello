#include <stdarg.h>
#include <stdio.h>

#include <libubox/ulog.h>

int main(void)
{
	int cnt = 0;

	ulog_open(ULOG_STDIO, -1, "mua");
	while (cnt++ < 5) {
		ulog(LOG_INFO, "%d - %d\n", __LINE__, cnt);
		ulog(LOG_WARNING, "%d - %d\n", __LINE__, cnt);
		ulog(LOG_ERR, "%d - %d\n", __LINE__, cnt);
		sleep(1);
	}
	ulog_close();

	cnt = 0;
	ulog_open(ULOG_SYSLOG, -1, "mua");
	while (cnt++ < 5) {
		ulog(LOG_INFO, "%d - %d\n", __LINE__, cnt);
		ulog(LOG_WARNING, "%d - %d\n", __LINE__, cnt);
		ulog(LOG_ERR, "%d - %d\n", __LINE__, cnt);
		sleep(1);
	}
	ulog_close();

	return 0;
}


/* vim: set ts=4 sw=4 tw=0 noexpandtab nolist : */
