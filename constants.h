#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
// Three different debug values. \
    NONE will not output any debug info. \
    ERROR will only output error messages. \
	ALL will output all debug messages.
	
namespace Constants{

enum DEBUG_T {
	NONE,
	ERROR,
	ALL
};

const static int DEBUG = ALL;
}
#endif