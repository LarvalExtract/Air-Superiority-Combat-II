#ifndef _UTILS_H_
#define _UTILS_H_

#include <cstdlib>

#define SAFE_DELETE_PTR(ptr){ if(ptr){ delete ptr; ptr = nullptr; } }
#define SAFE_DELETE_ARY(ptr){ if(ptr){ delete[] ptr; ptr = nullptr; } }

inline unsigned int Random(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

inline int Min(int a, int b) { return a < b ? a : b; }
inline int Max(int a, int b) { return a > b ? a : b; }

#endif
