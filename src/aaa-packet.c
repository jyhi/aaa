#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "aaa-packet.h"

char *str_split(char *str, const char *delim) {
    static char* left = NULL;
    if(str == NULL) str = left;
    
    const char *p;
    bool flag;
    char *s = str;
    while(*s != 0)
    {
        p = delim;
        flag = false;
        while(*p != 0)
        {
            if(*p++ == *s)
            {
                flag = true;
                *s = 0;
                break;
            }
        }
        ++s;
        if(flag)
        {
            left = s;
            return str;
        }
    }
    left = NULL;
    return str;
}

char *aaa_packet_serialize(const struct AaaPacket packet) {
    size_t packet_length = packet.message_length + packet.nonce_length + packet.signature_length;
    char *res = (char *)malloc(sizeof(char) * (packet_length + 7));
    
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
	struct AaaPacket *res = (struct AaaPacket *)malloc(sizeof(struct AaaPacket));
	char *token;
	
	// Check whether the string starts with "AAA"
	token = str_split(packet_str, "!");
	if (strncmp(token, "AAA", 3) != 0) return NULL;
	
	// Extract message from the packet_str
	token = str_split(NULL, ";");
	res->message = strcat(token, "\0");
	
	// Extract nonce from the packet_str
	token = str_split(NULL, ",");
	res->nonce = strcat(token, "\0");
	
	// Extract signature from the packet_str
	token = str_split(NULL, ".");
	res->signature = strcat(token, "\0");
	
	return res;
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
