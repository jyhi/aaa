#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aaa-message.h"

#define BUFSIZE 2048

char *aaa_message_serialize(const struct AaaMessage * const Aaa_msg) {
    if (!Aaa_msg) return NULL;
    
    char *json_msg = (char *)malloc(sizeof(char) * BUFSIZE);
    strcpy(json_msg, "{\n");
    switch (Aaa_msg->type) {
        case AAA_MESSAGE_TYPE_HELLO:
            strcat(json_msg, "\t\"type\": \"hello\",\n");
            strcat(json_msg, "\t\"id\": \"");
            strcat(json_msg, Aaa_msg->id);
            strcat(json_msg, "\",\n\t\"cert\": \"");
            strcat(json_msg, Aaa_msg->cert);
            strcat(json_msg, "\"\n");
            break;
        case AAA_MESSAGE_TYPE_MSG:
            strcat(json_msg, "\t\"type\": \"msg\",\n");
            strcat(json_msg, "\t\"msg\": \"");
            strcat(json_msg, Aaa_msg->message);
            strcat(json_msg, "\"\n");
            break;
        case AAA_MESSAGE_TYPE_BYE:
            strcat(json_msg, "\t\"type\": \"bye\"\n");
            break;
        default:
            return NULL;
    }
    strcat(json_msg, "}");
    return json_msg;
}

struct AaaMessage *aaa_message_deserialize(const char * const message_str) {
    struct AaaMessage *messagePacket = (struct AaaMessage *)malloc(sizeof(struct AaaMessage));
    
    if (strncmp(message_str, "/hello", 6) == 0)
    {
        messagePacket->type = AAA_MESSAGE_TYPE_HELLO;
        messagePacket->id = "temp_id"; // Where can get the name of sender?
        messagePacket->cert = "temp_cert"; // Where can get the cert?
    } else if (strncmp(message_str, "/bye", 4) == 0) {
        messagePacket->type = AAA_MESSAGE_TYPE_BYE;
    } else {
        messagePacket->type = AAA_MESSAGE_TYPE_MSG;
        messagePacket->message = message_str;
    }
    return messagePacket;
}

// for testing
// int main() {
// 	struct AaaMessage test;
// 	test.type = AAA_MESSAGE_TYPE_MSG;
// 	test.id = get_ip();
// 	test.message = "May the fourth be with you!";
// 	// test aaa_message_serialize
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

// 	// test aaa_message_deserialize
// 	char *msg;
// 	// -> msg
// 	msg = "How's it going?";
// 	struct AaaMessage *test_msg_des1 = aaa_message_deserialize(msg);
//    	printf("Type is: %u\n",test_msg_des1->type);
//    	printf("%s\n\n",test_msg_des1->message);
	
// 	// ->hello
// 	msg = "/hello";
// 	struct AaaMessage *test_msg_des2 = aaa_message_deserialize(msg);
// 	printf("Type is: %u\n",test_msg_des2->type);
// 	printf("ID is: %s\n",test_msg_des2->id);
// 	printf("Cert is: %s\n\n",test_msg_des2->cert);
	
// 	// -> bye
// 	msg = "/bye";
// 	struct AaaMessage *test_msg_des3 = aaa_message_deserialize(msg);
// 	printf("Type is: %u\n",test_msg_des3->type);

// 	return 0;
// }
