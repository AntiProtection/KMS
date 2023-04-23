#pragma once
#include "rpc.h"
#include "kms.h"
#include "util.h"

bool _send(SOCKET sock, const char* data, int len);
bool _recv(SOCKET sock, char* data, int len);
int StartServer(const int port, RequestCallback_t callback);
int StopServer();
