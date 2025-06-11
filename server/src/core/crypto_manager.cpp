//
// Created by Christian Caamano on 6/6/25.
//

#include "crypto_manager.h"

std::string CryptoManager::hashCreds(const std::string &username, const std::string &password)
{
    // Concatenate username & password into one secret
    // Place delimiter between each to prevent concatenation ambiguity (e.g. "usrnme pword" vs. "usrnmepw ord") & hash collisions
    const std::string secret = username + ":" + password;

    // Create fixed size byte buffer to hold the text representation of the hash
    // Size: large enough to hold every possible output produced by crypto_pwhash_str ('\0' incl)
    char encoded[crypto_pwhash_STRBYTES];

    // Generate a fresh 16-byte random salt, run Argon2id with the cost params below, and encode into destination buffer
    if (crypto_pwhash_str(
            encoded,                           // Destination buffer for hash
            secret.data(),                     // Pointer to combined secret bytes
            secret.size(),                     // Combined secret length
            crypto_pwhash_OPSLIMIT_SENSITIVE,  // CPU cost (about 2 Argon2 passes)
            crypto_pwhash_MEMLIMIT_SENSITIVE)  // RAM cost (19 MiB)
        != 0)
    {
        throw std::runtime_error("Error: hashing failed, likely due to running out of memory.");
    }

    // Return final encoded text: internal stuff + cost params + salt + hash
    return std::string(encoded);
}

void CryptoManager::deriveConfigKey(std::string username, std::string password, const unsigned char *salt, unsigned char output[32])
{
    // Concatenate username & pass with delimiter
    const std::string secret = username + ':' + password;

    // Get config key, put into buffer
    constexpr std::size_t KEY_LEN = 32;
    constexpr std::size_t OPS       = crypto_pwhash_OPSLIMIT_MODERATE;
    constexpr std::size_t MEM       = crypto_pwhash_MEMLIMIT_MODERATE;

    if (crypto_pwhash(output, KEY_LEN,
                      secret.data(), secret.size(),
                      salt,
                      OPS,
                      MEM,
                      crypto_pwhash_ALG_ARGON2ID13) != 0)
    {
        throw std::runtime_error("Argon2id failed (out of memory?)");
    }
}

bool CryptoManager::decrypt(const unsigned char *key,
             const unsigned char *nonce,
             std::span<const unsigned char> cipherAndTag,
             std::vector<unsigned char> &output)
{
    // ChaCha20-Poly1305 IETF tag: 16 bytes
    constexpr std::size_t TAG_LEN = crypto_aead_chacha20poly1305_IETF_ABYTES;

    if (cipherAndTag.size() < TAG_LEN) {
        // Wrong tag size; malformed
        return false;
    }

    // Allocate space for plaintext (cipherAndTag length - tag length)
    output.reserve(cipherAndTag.size() - TAG_LEN);
    unsigned long long plaintextLength = 0;

    // Check MAC (message authentication code)
    int rc = crypto_aead_chacha20poly1305_ietf_decrypt(
        output.data(), &plaintextLength,
        /*nsec =*/ nullptr,
        cipherAndTag.data(), cipherAndTag.size(),
        /*aad =*/ nullptr, 0,
        nonce,
        key);

    if (rc != 0) {
        output.clear();
        return false;
    }

    // Trim
    output.resize(static_cast<std::size_t>(plaintextLength));

    // Success
    return true;
}
