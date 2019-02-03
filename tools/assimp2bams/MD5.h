#ifndef MD5__H___
#define MD5__H___

#include <string>

class MD5
{
	public:
		static std::string getHexStringHashFromBytes(const char* buffer, int size);
		static void get16bytesHashFromBytes(const char* buffer, int size, unsigned char outBuffer[16]);
		static std::string getHexStringHashFromFile(const char* filename);
		static void get16bytesHashFromFile(const char* filename, unsigned char outBuffer[16]);
		static std::string _16BytesToHexString(unsigned char md5[16]);
};

#endif