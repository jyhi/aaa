#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>

#include "aaa-packet.h"
#include "aaa-crypto.h"

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
			strcat(str, "\0");
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
    char *rub1 = NULL, *rub2 = NULL, *rub3 = NULL;
    size_t rub1_size = 0, rub2_size = 0, rub3_size = 0;

    if (packet->message)
    {
        rub1 = aaa_bin2base64(packet->message, packet->message_length);
        rub1_size = strlen(rub1);
    }
    if (packet->nonce)
    {
        rub2 = aaa_bin2base64(packet->nonce, packet->nonce_length);
        rub2_size = strlen(rub2);
    }
    if (packet->signature)
    {
        rub3 = aaa_bin2base64(packet->signature, packet->signature_length);
        rub3_size = strlen(rub3);
    }

    char *res = (char *)malloc(sizeof(char) * (rub1_size + rub2_size + rub3_size + 7));

    // [Header]!
    strcpy(res, "AAA!");
    // [Message];
    if (packet->message)
    {
        strcat(res, rub1);
        free(rub1);
    }
    strcat(res, ";");
    // [Nonce],
    if (packet->nonce)
    {
        strcat(res, rub2);
        free(rub2);
    }
    strcat(res, ",");
    // [Authentication].
    if (packet->signature)
    {
        strcat(res, rub3);
        free(rub3);
    }
    strcat(res, ".");
    
    return res;
}

struct AaaPacket *aaa_packet_deserialize(const char * const packet_str) {
    struct AaaPacket *res = (struct AaaPacket *)malloc(sizeof(struct AaaPacket));
    char *copy_packet_str = (char *)malloc(sizeof(char) * strlen(packet_str));
    strcpy(copy_packet_str, packet_str);
    char *token;

    // Check whether the string starts with "AAA"
    token = str_split(copy_packet_str, "!");
    if (strncmp(token, "AAA", 3) != 0) return NULL;

    // Extract message from the packet_str
    token = str_split(NULL, ";");
    // Transform a Base64 representation into its corresponding binary sequence
    res->message = aaa_base642bin(&res->message_length, token);

    // Extract nonce from the packet_str
    token = str_split(NULL, ",");
    res->nonce = aaa_base642bin(&res->nonce_length, token);

    // Extract signature from the packet_str
    token = str_split(NULL, ".");
    res->signature = aaa_base642bin(&res->signature_length, token);

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
