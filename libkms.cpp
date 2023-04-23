#include "libkms.h"
#include <thread>
static SOCKET s_server;

void ServerThread(SOCKET s_client, DWORD RpcAssocGroup, const char* ipstr, const char* localPort) {
	WORD NdrCtx = RPC_INVALID_CTX, Ndr64Ctx = RPC_INVALID_CTX;
	RPC_HEADER rpcRequestHeader;
	unsigned int request_len;
	unsigned int response_len;
	BYTE requestBuffer[MAX_REQUEST_SIZE + sizeof(RPC_RESPONSE64)];
	BYTE responseBuffer[MAX_RESPONSE_SIZE + sizeof(RPC_HEADER) + sizeof(RPC_RESPONSE64)];

	while (true) {
		if (!rpcGetRequestHeader(s_client, rpcRequestHeader)) break;
		if (!rpcGetRequestLength(rpcRequestHeader, request_len)) break;
		if (!rpcGetRequest(s_client, requestBuffer, request_len)) break;
		if (!rpcCreateResponse(NdrCtx, Ndr64Ctx, rpcRequestHeader, requestBuffer, request_len, responseBuffer, response_len, RpcAssocGroup, ipstr, localPort)) break;
		if (!rpcSendResponse(s_client, responseBuffer, response_len)) break;
	}

	close(s_client);
}

int runServer(int port)
{
	DWORD RpcAssocGroup = rand();
	const char* ipstr;
	char localPort[6];
	snprintf(localPort, 6, "%d", port);

	while (true)
	{
		SOCKET s_client;

		struct sockaddr source_addr = {}; // Large enough for both IPv4 or IPv6
		socklen_t addr_len = sizeof(source_addr);

		if ((s_client = accept(s_server, &source_addr, &addr_len)) == INVALID_SOCKET)
		{
			if (errno == EINTR || errno == ECONNABORTED) continue;
			return errno;
		}

		RpcAssocGroup++;
		ipstr = inet_ntoa(((struct sockaddr_in*)&source_addr)->sin_addr);
		std::thread th(ServerThread, s_client, RpcAssocGroup, ipstr, localPort);
		th.detach();
	}
	return 0;
}

int listenOnAllAddresses(int port)
{
	int socketOption = 1;

	struct sockaddr_in addr4 = {
		addr4.sin_family = AF_INET,
		addr4.sin_port = htons(port),
		addr4.sin_addr.s_addr = htonl(INADDR_ANY)
	};

	s_server = socket(AF_INET, SOCK_STREAM, 0);

	if (s_server == INVALID_SOCKET
		|| setsockopt(s_server, SOL_SOCKET, SO_REUSEADDR, (char*)&socketOption, sizeof(socketOption))
		|| bind(s_server, (struct sockaddr*)&addr4, sizeof(addr4))
		|| listen(s_server, SOMAXCONN))
	{
		return errno;
	}

	return 0;
}

int StartServer(const int port, RequestCallback_t callback)
{
	SetCreateResponseBase(callback);
	srand((unsigned int)time(NULL));
	listenOnAllAddresses(port);
	runServer(port);
	return 1;
}

int StopServer()
{
	shutdown(s_server, SHUT_RDWR);
	close(s_server);
	return 1;
}
