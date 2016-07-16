#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "15";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 0;
	static const long BUILD  = 121;
	static const long REVISION  = 104094;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 196;
	#define RC_FILEVERSION 0,0,121,104094
	#define RC_FILEVERSION_STRING "0, 0, 121, 104094\0"
	static const char FULLVERSION_STRING [] = "0.0.121.104094";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 6;
	

}
#endif //VERSION_H
