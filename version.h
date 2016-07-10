#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "10";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2016";
	static const char UBUNTU_VERSION_STYLE[] =  "16.07";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 0;
	static const long BUILD  = 108;
	static const long REVISION  = 44230;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 130;
	#define RC_FILEVERSION 0,0,108,44230
	#define RC_FILEVERSION_STRING "0, 0, 108, 44230\0"
	static const char FULLVERSION_STRING [] = "0.0.108.44230";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 8;
	

}
#endif //VERSION_H
