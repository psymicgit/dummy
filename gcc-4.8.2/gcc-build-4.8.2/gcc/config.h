#ifndef GCC_CONFIG_H
	#define GCC_CONFIG_H
	#ifdef GENERATOR_FILE
		#error config.h is for the host, not build, machine.
	#endif

	#define GATHER_STATISTICS 0

#include "auto-host.h"
#include "cppconfig.h"

#include "system.h"
#include "coretypes.h"
#include "config/i386/i386.h"

	#include <limits.h>

//#define GENERATOR_FILE 0

	#ifdef IN_GCC
		#include "ansidecl.h"
	#endif

#define snprintf sprintf_s
#define HAVE_COMDAT_GROUP 1

#endif /* GCC_CONFIG_H */
