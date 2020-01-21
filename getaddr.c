// getaddr.c - Lists all (both IPv4 & IPv6) addresses for given host
// Taken from Beej's guide for network programming
// (C) Srdjan Rajcevic srdjan[AT]rajcevic.net§

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char * argv[]) {

	struct addrinfo hints, *res, *p; // two pointers for linked list travestal
	int error;
	char ipstr[INET6_ADDRSTRLEN];

	memset(&hints,0,sizeof(hints)); // bzeroing hints struct
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if(argc != 2) {
		fprintf(stderr,"Error. USAGE: %s FQDN\n",argv[0]);
		return 1;
	}

	error = getaddrinfo(argv[1],NULL,&hints,&res);
	
	if (error != 0) {
		fprintf(stderr,"Error. Dying.\n");
		return 2;
	}
	
	printf("IP address for %s:\n",argv[1]);

	for(p = res; p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;

		if(p->ai_family == AF_INET) {
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}
		else {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr); // converting from net to presentation (to display IP)
		printf("%s: %s\n",ipver,ipstr);
	}

	freeaddrinfo(res);

	return 0;
}


