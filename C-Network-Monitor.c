#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

bool isHostReachable(const char *host) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;

    if (getaddrinfo(host, NULL, &hints, &res) == 0) {
        freeaddrinfo(res);
        return true;
    }
    return false;
}

bool isPortOpen(const char *host, int port) {
    struct sockaddr_in addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        return false;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &(addr.sin_addr)) <= 0) {
        close(sockfd);
        return false;
    }

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
        close(sockfd);
        return true;
    }

    close(sockfd);
    return false;
}

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

    const char *remoteHost = "example.com";
    int remotePort = 80;

    if (isHostReachable(remoteHost)) {
        printf("Host %s is reachable.\n", remoteHost);
        if (isPortOpen(remoteHost, remotePort)) {
            printf("Port %d is open on %s.\n", remotePort, remoteHost);
        } else {
            printf("Port %d is closed on %s.\n", remotePort, remoteHost);
        }
    } else {
        printf("Host %s is not reachable.\n", remoteHost);
    }

    return 0;
}
