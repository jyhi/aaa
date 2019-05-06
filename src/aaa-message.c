#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "aaa-message.h"

#define BUFSIZE 2048
#define TYPE 0
#define ID 1
#define CERT 2
#define MSG 3

static const char *type_prefix                 = "type: ";
static const char *id_prefix                 = "id: ";
static const char *cert_prefix                 = "cert: ";
static const char *msg_prefix                 = "msg: ";

static char *JSON_extract(const char * const message_str, int infro) {
    char *buffer = g_malloc0(BUFSIZE);
    const char *line = NULL;

    switch (infro) {
        case TYPE:
            line = g_strstr_len(message_str, -1, type_prefix);
            if (line) {
                sscanf(line, "%*s %s", buffer);
            }
            break;
        case ID:
            line = g_strstr_len(message_str, -1, id_prefix);
            if (line) {
                sscanf(line, "%*s %s", buffer);
            }
            break;
        case CERT:
            line = g_strstr_len(message_str, -1, cert_prefix);
            if (line) {
                sscanf(line, "%*s %s", buffer);
            }
            break;
        case MSG:
            line = g_strstr_len(message_str, -1, msg_prefix);
            if (line) {
                sscanf(line, "%*s %s", buffer);
            }
            break;
        default:
            break;
    }
    char *ret = g_strdup(buffer);

    g_free(buffer);
    return ret;
}

char *aaa_message_serialize(const struct AaaMessage * const Aaa_msg) {
    if (!Aaa_msg) return NULL;

    char *json_msg = (char *)malloc(sizeof(char) * BUFSIZE);
    strcpy(json_msg, "{\n");
    switch (Aaa_msg->type) {
        case AAA_MESSAGE_TYPE_HELLO:
            strcat(json_msg, "type: hello ,\n");
            strcat(json_msg, "id: ");
            if (Aaa_msg->id)
            {
                strcat(json_msg, Aaa_msg->id);
            } else {
                return NULL;
            }
            strcat(json_msg, " ,\ncert: ");
            if (Aaa_msg->cert)
            {
                strcat(json_msg, Aaa_msg->cert);
            } else {
                return NULL;
            }
            strcat(json_msg, "\n");
            break;
        case AAA_MESSAGE_TYPE_MSG:
            strcat(json_msg, "type: msg ,\n");
            strcat(json_msg, "msg: ");
            if (Aaa_msg->message)
            {
                strcat(json_msg, Aaa_msg->message);
            } else {
                return NULL;
            }
            strcat(json_msg, "\n");
            break;
        case AAA_MESSAGE_TYPE_BYE:
            strcat(json_msg, "type: bye\n");
            break;
        default:
            return NULL;
    }
    strcat(json_msg, "}");
    return json_msg;
}

struct AaaMessage *aaa_message_deserialize(const char * const message_str) {
    if (!message_str) return NULL;

    struct AaaMessage *messagePacket = (struct AaaMessage *)malloc(sizeof(struct AaaMessage));
    char *type = JSON_extract(message_str, TYPE);

    if (strncmp(type, "hello", 5) == 0)
    {
        messagePacket->type = AAA_MESSAGE_TYPE_HELLO;
        messagePacket->id = JSON_extract(message_str, ID);
        messagePacket->cert = JSON_extract(message_str, CERT);
    } else if (strncmp(type, "msg", 3) == 0) {
        messagePacket->type = AAA_MESSAGE_TYPE_MSG;
        messagePacket->message = JSON_extract(message_str, MSG);
    } else if (strncmp(type, "bye", 3) == 0) {
        messagePacket->type = AAA_MESSAGE_TYPE_BYE;
    } else {
        return NULL;
    }

    free(type);
    return messagePacket;
}
