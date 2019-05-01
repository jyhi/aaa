#include <string.h>
#include "aaa-packet.h"

#define BUFSIZE 2048

char *aaa_packet_serialize(const struct AaaPacket packet) {
    char *res = (char *)malloc(sizeof(char) * BUFSIZE);

    // [Header]!
    strcpy(res, "AAA!");
    // [Message];
    strcat(res, packet.message);
    strcat(res, ";");
    // [Nonce],
    strcat(res, packet.nonce);
    strcat(res, ",");
    // [Authentication].
    strcat(res, packet.signature);
	strcat(res, ".");

    return res;
}

struct AaaPacket *aaa_packet_deserialize(const char * const packet_str) {
	struct AaaPacket res;
	char *token;
	
	// Check whether the string starts with "AAA"
	token = strtok(packet_str, "!");
	if (strncmp(token, "AAA", 3) != 0) return NULL;
	
	// Extract message from the packet_str
	token = strtok(NULL, ";");
	res.message = strcat(token, "\0");
	
	// Extract nonce from the packet_str
	token = strtok(NULL, ",");
	res.nonce = strcat(token, "\0");
	
	// Extract signature from the packet_str
	token = strtok(NULL, ".");
	res.signature = strcat(token, "\0");
	
	return &res;
}


// for testing
// int main() {
// 	struct AaaPacket test;
// 	test.message = "[message]";
// 	test.nonce = "[nonce]";
// 	test.signature = "[signature]";
// 	char *res = aaa_packet_serialize(test);
// 	printf("%s\n", res);

// 	struct AaaPacket *test_des = aaa_packet_deserialize(res);
// 	if (test_des != NULL)
// 	{
// 		printf("message is: %s\n", test_des->message);
// 		printf("nonce is: %s\n", test_des->nonce);
// 		printf("signature is: %s\n", test_des->signature);
// 	} else {
// 		printf("NULL\n");
// 	}
// 	return 0;
// }
