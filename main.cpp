#include <cstdlib>
#include <unistd.h>
#include "rpc.h"
#include "kms.h"
#include "libkms.h"

static WCHAR ePID[] = u"00000-00000-000-000000-00-0000-0000.0000-0000000";
static QWORD HWID = 0x0000000000000000;

void KmsCallback(const REQUEST* const request, RESPONSE* const response, QWORD* const hwId, const char* ipstr)
{
    response->Version = request->Version;
    response->PIDSize = sizeof(ePID);
    memcpy(response->KmsPID, ePID, response->PIDSize);
    response->CMID = request->CMID;
    response->ClientTime = request->ClientTime;
    response->Count = request->N_Policy;
    response->VLActivationInterval = 120;
    response->VLRenewalInterval = 10800;
    *hwId = HWID;
}

int main(int argc, char* argv[])
{
    if (daemon(0, 0) == 0) {
        if (argc > 1)
            StartServer(atoi(argv[1]), KmsCallback);
        else
            StartServer(1688, KmsCallback);
    }
    return 0;
}
