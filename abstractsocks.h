#ifndef ABSTRACTSOCKS_H
#define ABSTRACTSOCKS_H

#include <netinet/in.h>

int createTCPIPv4Socket();
struct sockaddr_in createIPv4Address(const char* ip, const int port);

#endif