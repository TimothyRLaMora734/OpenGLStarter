#ifndef int24__t__h
#define int24__t__h

#include <stdint.h>

#pragma pack(push, 1)
class int24_t {
public:
	uint8_t bytes[3];

	int24_t() {}

	inline void operator = (const int32_t& i) {
#ifdef __BIG_ENDIAN__
		//not tested...
		bytes[2] = (i & 0x000000ff);
		bytes[1] = (i & 0x0000ff00) >> 8;
		bytes[0] = (i & 0x007f0000) >> 16 |
			(i & 0x80000000) >> 24;//sign bit
#else 
		// little endian - tested!
		bytes[0] = (i & 0x000000ff);
		bytes[1] = (i & 0x0000ff00) >> 8;
		bytes[2] = (i & 0x007f0000) >> 16 |
			(i & 0x80000000) >> 24;//sign bit
#endif

	}

	inline int24_t(const int8_t& c) { *this = (int32_t)c; }
	inline int24_t(const int16_t& s) { *this = (int32_t)s; }
	inline int24_t(const int32_t& i) { *this = i; }
	inline int24_t(const int64_t& i) { *this = (int32_t)i; }
	inline int24_t(const double& d) { *this = (int32_t)d; }
	inline int24_t(const float& f) { *this = (int32_t)f; }

	//allow this kind of casting:
	//  int24_t value24 = -1;
	//  int32_t value32 = (int32_t)value24;
	inline operator int32_t()const {
#ifdef __BIG_ENDIAN__
		//not tested...
		int32_t bitsign = bytes[0] & 0x80;
		int32_t result = bytes[2] | (bytes[1] << 8) | ((bytes[0] & 0x7f) << 16);
#else
		// little endian - tested!
		int32_t bitsign = bytes[2] & 0x80;
		int32_t result = bytes[0] | (bytes[1] << 8) | ((bytes[2] & 0x7f) << 16);
#endif
		//Two's complement conversion
		if (bitsign)
			result |= 0xff800000;
		return  result;
	}

	inline operator int64_t() const {
		return (int32_t)(*this);
	}

	inline operator float()const {
		return (int32_t)(*this);
	}

	inline operator double()const {
		return (int32_t)(*this);
	}
};
#pragma pack(pop)

#endif