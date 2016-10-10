#ifndef __LOG_H__
#define __LOG_H__

typedef enum {
	LOG_LEVEL_ALL,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_FATAL,
	LOG_LEVEL_OFF,
} LOG_LEVEL;

#define LOG_LEVEL_SYMBOL_DEBUG          "D"
#define LOG_LEVEL_SYMBOL_INFO           "I"
#define LOG_LEVEL_SYMBOL_WARN           "W"
#define LOG_LEVEL_SYMBOL_ERROR          "E"
#define LOG_LEVEL_SYMBOL_FATAL          "F"

#define LOG_DEBUG(fmt , arg...)         log_out(LOG_LEVEL_DEBUG, LOG_LEVEL_SYMBOL_DEBUG " " fmt , ## arg)
#define LOG_INFO(fmt , arg...)          log_out(LOG_LEVEL_INFO , LOG_LEVEL_SYMBOL_INFO  " " fmt , ## arg)
#define LOG_WARN(fmt , arg...)          log_out(LOG_LEVEL_WARN , LOG_LEVEL_SYMBOL_WARN  " " fmt , ## arg)
#define LOG_ERROR(fmt , arg...)         log_out(LOG_LEVEL_ERROR, LOG_LEVEL_SYMBOL_ERROR " " fmt , ## arg)
#define LOG_FATAL(fmt , arg...)         log_out(LOG_LEVEL_FATAL, LOG_LEVEL_SYMBOL_FATAL " " fmt , ## arg)


/**
 * Another API for quick debug
 * It will output line and function name
 */
#define D()                             printf("* L%d, %s\r\n", __LINE__, __FUNCTION__)
#define DD(fmt, arg...)                 printf("* L%d, %s: " fmt, __LINE__, __FUNCTION__, ##arg)
#define DDD(fmt, arg...)                printf("* L%d: " fmt, __LINE__, ##arg)

/* Public API for log */
void log_out(LOG_LEVEL level, const char * fmt, ...);

void log_set_level(LOG_LEVEL level);
void log_init(void);

#endif /* __LOG_H__ */

/* vim: set ts=4 sw=4 tw=0 noexpandtab nolist : */
