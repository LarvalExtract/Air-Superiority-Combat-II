#ifndef _UTILS_H_
#define _UTILS_H_

#include <cstdlib>

#define SAFE_DELETE_PTR(ptr){ if(ptr){ delete ptr; ptr = NULL; } }
#define SAFE_DELETE_ARY(ptr){ if(ptr){ delete[] ptr; ptr = NULL; } }

inline unsigned int Random(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

#endif
