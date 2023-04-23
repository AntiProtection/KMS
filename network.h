#pragma once
#include "util.h"

// Send or receive a fixed number of bytes regardless if received in one or more chunks
bool _send(SOCKET sock, const char* data, int len)
{
	int n = 0;
	do n = send(sock, data, len, 0);
	while ((n < 0 && errno == EINTR) || (n > 0 && (data += n, (len -= n) > 0)));
	return !len;
}
bool _recv(SOCKET sock, char* data, int len)
{
	int n = 0;
	do n = recv(sock, data, len, 0);
	while ((n < 0 && errno == EINTR) || (n > 0 && (data += n, (len -= n) > 0)));
	return !len;
}
