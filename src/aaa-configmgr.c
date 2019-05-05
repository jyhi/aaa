#include <glib.h>
#include <glib-object.h>
#include <stdint.h>
#include "aaa-crypto.h"

#define AAA_CONFIGMGR_DEFAULT_CONFIG_FILE "aaa.conf"

// Internal states
static char    *_id          = NULL;
static uint8_t *_cert        = NULL;
static size_t   _cert_length = 0;
static uint8_t *_key         = NULL;
static size_t   _key_length  = 0;

char *aaa_config_get_id(void)
{
  return g_strdup(_id);
}

void aaa_config_set_id(const char * const id)
{
  if (_id) {
    g_debug("configmgr: id changed from %s to %s", _id, id);
    g_free(_id);
  } else {
    g_debug("configmgr: new id %s", id);
  }

  _id = g_strdup(id);
}

uint8_t *aaa_config_get_cert(size_t *size)
{
  *size = _cert_length;
  return g_memdup(_cert, _cert_length);
}

void aaa_config_set_cert(const uint8_t * const cert, const size_t cert_length)
{
  if (_cert || _cert_length > 0) {
    g_debug("configmgr: cert replaced");
    g_free(_cert);
    _cert_length = 0;
  } else {
    g_debug("configmgr: cert added");
  }

  _cert = g_memdup(cert, cert_length);
  _cert_length = cert_length;
}

uint8_t *aaa_config_get_key(size_t *size)
{
  *size = _key_length;
  return g_memdup(_key, _key_length);
}

void aaa_config_set_key(const uint8_t * const key, const size_t key_length)
{
  if (_key || _key_length > 0) {
    g_debug("configmgr: key replaced");
    g_free(_key);
    _key_length = 0;
  } else {
    g_debug("configmgr: key added");
  }

  _key = g_memdup(key, key_length);
  _key_length = key_length;
}

int aaa_config_load(void)
{
  GError  *err = NULL;
  gboolean r   = FALSE;

  g_debug("loading configuration file from %s", AAA_CONFIGMGR_DEFAULT_CONFIG_FILE);

  GKeyFile *conf = g_key_file_new();
  r = g_key_file_load_from_file(conf, AAA_CONFIGMGR_DEFAULT_CONFIG_FILE, G_KEY_FILE_KEEP_COMMENTS, &err);
  if (!r) {
    g_warning("configmgr: failed to load config file: %s", err->message);
    g_clear_error(&err);
    g_clear_object(&conf);
    return 0;
  }

  // Load ID
  gchar *id = g_key_file_get_string(conf, "AAA", "id", &err);
  if (!id) {
    g_warning("configmgr: id not found in config file: %s", err->message);
    g_clear_error(&err);
  }
  _id = id;

  // Load certificate
  gchar *cert = g_key_file_get_string(conf, "AAA", "cert", &err);
  if (!cert) {
    g_warning("configmgr: cert not found in config file: %s", err->message);
    g_clear_error(&err);
  }

  // Deserialize certificate from Base64
  _cert = aaa_base642bin(&_cert_length, cert);

  // Load key
  gchar *key = g_key_file_get_string(conf, "AAA", "key", &err);
  if (!key) {
    g_warning("configmgr: key not found in config file: %s", err->message);
    g_clear_error(&err);
  }

  // Deserialize key from Base64
  _key = aaa_base642bin(&_key_length, key);

  g_clear_object(&conf);

  g_debug("done reading config file %s", AAA_CONFIGMGR_DEFAULT_CONFIG_FILE);

  return 1;
}

int aaa_config_save(void)
{
  GError  *err = NULL;
  gboolean r   = FALSE;

  g_debug("creating config file for rewriting");

  GKeyFile *conf = g_key_file_new();

  // Serialize cert and key first
  gchar *cert_base64 = aaa_bin2base64(_cert, _cert_length);
  gchar *key_base64  = aaa_bin2base64(_key, _key_length);

  // Build the config file
  g_key_file_set_string(conf, "AAA", "id", _id);
  g_key_file_set_string(conf, "AAA", "cert", cert_base64);
  g_key_file_set_string(conf, "AAA", "key", key_base64);

  // Rewrite the file
  r = g_key_file_save_to_file(conf, AAA_CONFIGMGR_DEFAULT_CONFIG_FILE, &err);
  if (!r) {
    g_warning("configmgr: failed to write config file: %s", err->message);
    g_clear_error(&err);
    return 0;
  }

  g_debug("rewritten config file at %s", AAA_CONFIGMGR_DEFAULT_CONFIG_FILE);

  g_clear_object(&conf);

  return 1;
}
