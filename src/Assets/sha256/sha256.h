#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define SHA256_DIGEST_SIZE 32  // SHA-256 produces 32 raw bytes (256 bits)

/**
 * Computes the SHA-256 hash of the given input message.
 *
 * @param msg        Pointer to the input message (raw bytes).
 * @param msgLen     Length of the input message in bytes.
 * @param digest     Pointer to a buffer of at least SHA256_DIGEST_SIZE bytes.
 *                   The resulting 32-byte hash will be written here.
 *
 * @return           0 on success, non-zero on failure.
 */
int SHA256(const uint8_t* msg, size_t msgLen, uint8_t* digest);

#ifdef __cplusplus
}
#endif