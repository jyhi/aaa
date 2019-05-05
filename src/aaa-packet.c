#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "aaa-packet.h"

char *str_split(char *str, const char *delim) {
	static char* left = NULL;
	if(str == NULL) str = left;
	
	bool flag;
	char *s = str;
	while(*s != 0)
	{
		flag = false;
		if(*delim == *s)
		{ // when it meet the delimiter
			flag = true;
			*s = 0; // add '\0' to end the current piece substring
		}
		++s; // move to next char of input string
		if(flag)
		{
			left = s; // record the rest of string
			return str;
		}
	}
	left = NULL; // When reach the end of the string, and did not meet the token.
	return str;
}

void aaa_packet_free(struct AaaPacket *packet)
{
    g_free(packet->message);
    g_free(packet->nonce);
    g_free(packet->signature);

    packet->message_length = 0;
    packet->nonce_length = 0;
    packet->signature_length = 0;
}

char *aaa_packet_serialize(const struct AaaPacket * const packet) {
    if (!packet) return NULL;
    size_t packet_length = packet->message_length + packet->nonce_length + packet->signature_length;
    char *res = (char *)malloc(sizeof(char) * (packet_length + 7));
    
    // [Header]!
    strcpy(res, "AAA!");
    // [Message];
    if (packet->message)
    {
        strcat(res, packet->message);
    }
    strcat(res, ";");
    // [Nonce],
    if (packet->nonce)
    {
        strcat(res, packet->nonce);
    }
    
    strcat(res, ",");
    // [Authentication].
    if (packet->signature)
    {
        strcat(res, packet->signature);
    }
    strcat(res, ".");
    
    return res;
}

struct AaaPacket *aaa_packet_deserialize(const char * const packet_str) {
    if (!packet_str) return NULL;
    struct AaaPacket *res = (struct AaaPacket *)malloc(sizeof(struct AaaPacket));
    char *copy_packet_str = (char *)malloc(sizeof(char) * strlen(packet_str));
    strcpy(copy_packet_str, packet_str);
    char *token;
    
    // Check whether the string starts with "AAA"
    token = str_split(copy_packet_str, "!");
    if (strncmp(token, "AAA", 3) != 0) return NULL;
    
    // Extract message from the packet_str
    token = str_split(NULL, ";");
    res->message_length = strlen(token);
    res->message = (uint8_t *)malloc(sizeof(uint8_t) * strlen(token));
    memcpy(res->message, token, res->message_length);
    
    // Extract nonce from the packet_str
    token = str_split(NULL, ",");
    res->nonce_length = strlen(token);
    res->nonce = (uint8_t *)malloc(sizeof(uint8_t) * strlen(token));
    memcpy(res->nonce, token, res->nonce_length);
    
    // Extract signature from the packet_str
    token = str_split(NULL, ".");
    res->signature_length = strlen(token);
    res->signature = (uint8_t *)malloc(sizeof(uint8_t) * strlen(token));
    memcpy(res->signature, token, res->signature_length);
    
    free(copy_packet_str);
    return res;
}


// for testing
// int main() {
//     	struct AaaPacket test;
//     	test.message = "[message]";
//     	test.message_length = 9;
//     	test.nonce = "[nonce]";
//     	test.nonce_length = 7;
//     	test.signature = "[signature]";
//     	test.signature_length = 11;
//     	char *res = aaa_packet_serialize(test);
// 	printf("%s\n", res);

// 	struct AaaPacket *test_des = aaa_packet_deserialize(res);
// 	if (test_des != NULL)
// 	{
// 		printf("message is: %s %d\n", test_des->message, test_des->message_length);
//        	printf("nonce is: %s %d\n", test_des->nonce, test_des->nonce_length);
//        	printf("signature is: %s %d\n", test_des->signature, test_des->signature_length);
// 	} else {
// 		printf("NULL\n");
// 	}
// 	return 0;
// }
