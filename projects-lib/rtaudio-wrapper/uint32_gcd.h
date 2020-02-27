#ifndef uintgcd__H
#define uintgcd__H

#include <stdint.h>

uint32_t uint32_gcd(uint32_t a, uint32_t b)
{
	const uint32_t zero = 0;
	while (true)
	{
		if (a == zero)
			return b;
		b %= a;

		if (b == zero)
			return a;
		a %= b;
	}
}


#endif