/*
 * ILogger.h
 *
 *  Created on: May 21, 2017
 *      Author: jeffrey
 */

#ifndef VMODULE_ILOGGER_H_
#define VMODULE_ILOGGER_H_
#include <stdio.h>

#if !defined(TARGET_WINDOWS)
typedef FILE FILEWRAP;
#else
typedef void FILEWRAP;
#endif

typedef enum vmodule_LogPriority {
	VMODULE_LOG_UNKNOWN = 0,
	VMODULE_LOG_DEFAULT, /* only for SetMinPriority() */
	VMODULE_LOG_VERBOSE,
	VMODULE_LOG_DEBUG,
	VMODULE_LOG_INFO,
	VMODULE_LOG_WARN,
	VMODULE_LOG_ERROR,
	VMODULE_LOG_FATAL,
	VMODULE_LOG_SILENT, /* only for SetMinPriority(); must be last */
} vmodule_LogPriority;

namespace vmodule {

class ILogger {
public:
	ILogger();
	virtual ~ILogger();
	void Log(int loglevel,  const char *format, ...);
	virtual void log(int loglevel, const char* message) = 0;
};

} /* namespace vmodule */
#endif /* ILOGGER_H_ */