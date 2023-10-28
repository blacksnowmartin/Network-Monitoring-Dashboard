#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#else
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

int main() {
    printf("Network Monitoring and Evaluation\n");

#ifdef _WIN32
    // Windows-specific network information retrieval
    DWORD dwRetVal;
    ULONG ulOutBufLen = sizeof(MIB_IFTABLE);
    PMIB_IFTABLE pIfTable;
    
    pIfTable = (MIB_IFTABLE *)malloc(ulOutBufLen);
    if (pIfTable == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    if (GetIfTable(pIfTable, &ulOutBufLen, 0) == NO_ERROR) {
        for (int i = 0; i < pIfTable->dwNumEntries; i++) {
            printf("Interface Index: %lu\n", pIfTable->table[i].dwIndex);
            printf("Interface Description: %s\n", pIfTable->table[i].bDescr);
            // Add more network evaluation logic here
        }
    } else {
        printf("GetIfTable failed with error %ld\n", dwRetVal);
    }

    free(pIfTable);
#else
    // Unix-like system network information retrieval
    struct ifaddrs *ifap, *ifa;
    char host[NI_MAXHOST];
    
    if (getifaddrs(&ifap) == -1) {
        perror("getifaddrs");
        exit(1);
    }
    
    for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;
        
        int family = ifa->ifa_addr->sa_family;
        if (family == AF_INET) {
            if (getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) == 0) {
                printf("Interface: %s, Address: %s\n", ifa->ifa_name, host);
                // Add more network evaluation logic here
            }
        }
    }
    
    freeifaddrs(ifap);
#endif

    return 0;
}
