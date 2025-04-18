/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef EXTDLL_H
#define EXTDLL_H

//
// Global header file for extension DLLs
//

// Allow "DEBUG" in addition to default "_DEBUG"
#ifdef _DEBUG
#define DEBUG 1
#endif

// Silence certain warnings
#pragma warning(disable : 4244) // int or float down-conversion
#pragma warning(disable : 4305) // int or float data truncation
#pragma warning(disable : 4201) // nameless struct/union
#pragma warning(disable : 4514) // unreferenced inline function removed
#pragma warning(disable : 4100) // unreferenced formal parameter

// Prevent tons of unused windows definitions
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOWINRES
#define NOSERVICE
#define NOMCX
#define NOIME
#include "windows.h"
#else // _WIN32
#define FALSE 0
#define TRUE 1

typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned long ULONG;
typedef unsigned char BYTE;
typedef unsigned char byte;
typedef unsigned int uint;
typedef int BOOL;

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <cstddef>
#include <limits.h>
#include <stdarg.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

#include "strhelper.h"
#endif //_WIN32

// Misc C-runtime library headers
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

// Header file containing definition of globalvars_t and entvars_t
typedef int func_t;   //
typedef int string_t; // from engine's pr_comp.h;
typedef float vec_t;  // needed before including progdefs.h

// Vector class
#include "vector.h"

// Defining it as a (bogus) struct helps enforce type-checking
#define vec3_t Vector

// Shared engine/DLL constants
#include "const.h"
#include "progdefs.h"
#include "edict.h"

// Shared header describing protocol between engine and DLLs
#include "eiface.h"

// Shared header between the client DLL and the game DLLs
#include "cdll_dll.h"

#endif //EXTDLL_H
