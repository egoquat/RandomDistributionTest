#pragma once
//#pragma warning ( disable : 4305 4503 4786 4291 4312 4311 4267 4244 4355 )
//#pragma warning ( disable : 4305 4503 4786 4291 4312 4311 4267 4244 4355 4996 4819)
#include <windows.h>

#ifndef NULL
#define NULL 0
#endif

#define SAFEDELETE( p)			{ if ( p ) { delete ( p); ( p) = 0; } }
#define SAFEDELETE_ARRAY( p)	{ if ( p ) { delete[] ( p); ( p) = 0; } }

#ifndef SIZEOF_ARRAY
#define SIZEOF_ARRAY(p)			{ static_cast<int>(sizeof(p) / sizeof(p[0])) }
#endif

#define DEFAULT_FILEPATH		(512)
#define DEFAULT_MAX_FILEEX		(8)

template<typename T>
inline BOOL		ISNULL( T* t )
{
	if( 0 == t || NULL == t )
	{
		return TRUE;
	}
	return FALSE;
};

/// CRT_DBG
#define MEMORY_LEAK_DETECT_TEST 1

#if MEMORY_LEAK_DETECT_TEST
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__ ) 
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif // MEMORY_LEAK_DETECT_TEST

