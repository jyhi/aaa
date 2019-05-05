#ifndef AAA_CONFIGMGR_H
#define AAA_CONFIGMGR_H

#include <stdint.h>

/**
 * Get ID of the current user.
 *
 * @return ID string of the user. The caller is responsible to free the string.
 */
char *aaa_config_get_id(void);

/**
 * Set user ID.
 *
 * @param id [in] ID to set.
 */
void aaa_config_set_id(const char * const id);

/**
 * Get certificate of the current user.
 *
 * @param  size [out] The size of certificate. Must be a valid pointer.
 * @return Certificate of the user. The caller is responsible to free the
 *         memory.
 */
uint8_t *aaa_config_get_cert(size_t *size);

/**
 * Set user certificate.
 *
 * @param cert        [in] Certificate to set.
 * @param cert_length [in] Length of the certificate.
 */
void aaa_config_set_cert(const uint8_t * const cert, const size_t cert_length);

/**
 * Get private key of the current user.
 *
 * @param  size [out] The size of private key. Must be a valid pointer.
 * @return key of the user. The caller is responsible to free the
 *         memory.
 */
uint8_t *aaa_config_get_key(size_t *size)

/**
 * Set user private key.
 *
 * @param key        [in] Private key to set.
 * @param key_length [in] Length of the private key.
 */
void aaa_config_set_key(const uint8_t * const key, const size_t key_length)

/**
 * Load configuration from disk to memory.
 *
 * @return 0 for failure, 1 for success. On failure, the configuration file may
 *         be missing, so it depends on the caller what to do.
 */
int aaa_config_load(void);

/**
 * Save configuration from disk to memory.
 *
 * @return 0 for failure, 1 for success. On failure, the I/O operation may be
 *         failed, so it depends on the caller what to do.
 */
int aaa_config_save(void);

#endif
