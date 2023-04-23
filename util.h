#pragma once
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define INVALID_SOCKET		-1
typedef int					SOCKET;

typedef uint64_t	QWORD;
typedef uint32_t	DWORD;
typedef bool		BOOL;
typedef uint8_t		BYTE;
typedef uint16_t	WORD;
typedef char16_t	WCHAR;

struct GUID {
	DWORD  Data1;
	WORD Data2;
	WORD Data3;
	BYTE Data4[8];
};

struct FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
};

inline WORD BE16(WORD value)
{
	WORD ret;
	ret = value << 8;
	ret |= value >> 8;
	return ret;
}
inline DWORD BE32(DWORD value)
{
	DWORD ret;
	ret = value << 24;
	ret |= (value & 0x0000FF00) << 8;
	ret |= (value & 0x00FF0000) >> 8;
	ret |= value >> 24;
	return ret;
}
inline QWORD BE64(QWORD value)
{
	QWORD ret;
	ret = value << 56;
	ret |= (value & 0x000000000000FF00) << 40;
	ret |= (value & 0x0000000000FF0000) << 24;
	ret |= (value & 0x00000000FF000000) << 8;
	ret |= (value & 0x000000FF00000000) >> 8;
	ret |= (value & 0x0000FF0000000000) >> 24;
	ret |= (value & 0x00FF000000000000) >> 40;
	ret |= value >> 56;
	return ret;
}
