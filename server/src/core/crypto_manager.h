//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_CRYPTO_MANAGER_H
#define OMNISERVER_CRYPTO_MANAGER_H

#include <vector>
#include <span>

#include <sodium.h>

class CryptoManager {
public:
    /**
     * @brief hashCreds - One-way hash the network username + password, return the hash.
     * @param username - Network username.
     * @param password - Network password.
     * @return The resulting hash.
     */
    std::string hashCreds(const std::string &username, const std::string &password);

    /**
     * @brief deriveConfigKey - Derive the config key from a username, password, and salt.
     * @param username - Network username to incorporate into derivation
     * @param password - Network password to incorporate into derivation
     * @param salt - Salt to incorporate into derivation
     * @param output - The output buffer that will hold the resulting key.
     */
    void deriveConfigKey(const std::string username, const std::string password, const unsigned char *salt, unsigned char output[32]);

    /**
     * @brief decrypt - Decrypts the specified AEAD ciphertext/tag buffer using the specified key and nonce.
     * Places the plaintext result into the ouptut param. Returns true if decryption succeeded, false if decryption failed.
     * @param key - The decryption key.
     * @param nonce - The nonce originally used during encryption.
     * @param cipherAndTag - The buffer containing the ciphertext and MAC (message authentication code) tag.
     * @param output - The output buffer to store the plaintext result.
     * @return True if decryption succeeded, false if failed.
     */
    bool decrypt(const unsigned char *key,
                 const unsigned char *nonce,
                 std::span<const unsigned char> cipherAndTag,
                 std::vector<unsigned char> &output);
};


#endif //OMNISERVER_CRYPTO_MANAGER_H
