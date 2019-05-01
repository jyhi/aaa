#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <string.h>
#include "aaa-message.h"

#define BUFSIZE 1024
#define HELLO "Hello!"
#define BYE "Bye-bye!"

static char *get_ip() {
	struct ifaddrs *ifaddr, *ifa;
	int family, s;
	char *host = NULL;
 
	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return NULL;
	}
 
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;
 
		family = ifa->ifa_addr->sa_family;
 
		if (!strcmp(ifa->ifa_name, "lo"))
			continue;
		if (family == AF_INET) {
			if ((host = malloc(NI_MAXHOST)) == NULL)
				return NULL;
			s = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ? sizeof(struct sockaddr_in) :
					sizeof(struct sockaddr_in6),
					host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				return NULL;
			}
			freeifaddrs(ifaddr);
			return host;
		}
	}
	return NULL;
}

char *aaa_message_serialize(const struct AaaMessage * const Aaa_msg) {
	if (!Aaa_msg) return NULL;

	char *msg = (char *)malloc(sizeof(char) * BUFSIZE);
	switch (Aaa_msg->type) {
		case AAA_MESSAGE_TYPE_HELLO:
			msg = HELLO;
			break;
		case AAA_MESSAGE_TYPE_MSG:
			msg = Aaa_msg->message;
			break;
		case AAA_MESSAGE_TYPE_BYE:
			msg = BYE;
			break;
		default:
			return NULL;
	}
	return msg;
}

// for testing
// int main() {
// 	struct AaaMessage test;
// 	test.type = AAA_MESSAGE_TYPE_MSG;
// 	test.id = get_ip();
// 	test.message = "May the fourth be with you!";
// 	// test aaa_message_deserialize 
// 	// -> mgs
// 	char *test_msg_ser = aaa_message_serialize(&test);
// 	printf("%s\n", test_msg_ser);
// 	// -> hello
// 	test.type = AAA_MESSAGE_TYPE_HELLO;
// 	test_msg_ser = aaa_message_serialize(&test);
// 	printf("%s\n", test_msg_ser);
// 	// -> bye
// 	test.type = AAA_MESSAGE_TYPE_BYE;
// 	test_msg_ser = aaa_message_serialize(&test);
// 	printf("%s\n", test_msg_ser);
// 	return 0;
// }
