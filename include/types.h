#ifndef _TYPES_H_
#define _TYPES_H_

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#if __x86_64__
typedef u64 size_t;
#else
typedef u32 size_t;
#endif

typedef int bool;
#define true 1
#define false 0

#ifndef NULL
#define NULL 0
#endif

#endif
