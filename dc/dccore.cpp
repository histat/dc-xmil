#include "dccore.h"
#include <ronin/dc_time.h>

unsigned int dccore_gettick(void)
{
	static unsigned int count = 0;
	static unsigned int old = 0;
	unsigned int now = Timer();
  
	if (now != old) {
		unsigned int diff = now - old;
		unsigned int steps = (diff<<6) / (100000>>5);
		diff -= (steps*(100000>>5))>>6;
		old = now - diff;
		return (count += steps);
	}
  
	return count;
}
