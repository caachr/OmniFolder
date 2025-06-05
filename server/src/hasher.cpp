//
// Created by Christian Caamano on 5/30/25.
//

#include "../include/hasher.h"

std::string Hasher::hash(const std::initializer_list<std::string>& secrets)
{
    if (secrets.size() == 0) {
        std::cerr << "Error: Hasher's hash() requires at least one secret.\n";
        std::terminate();
    }

    // Concatenate secrets into one
    // Place delimiter between each to prevent concatenation ambiguity & hash collisions
    std::string combinedSecret;
    bool isFirstElement = true;
    for (const auto& s : secrets) {
        if (!isFirstElement) {
            combinedSecret += "\x00";
        }
        combinedSecret += s;
        isFirstElement = false;
    }

    // Create fixed size byte buffer to hold the text representation of the hash
    // Size: large enough to hold every possible output produced by crypto_pwhash_str ('\0' incl)
    char encoded[crypto_pwhash_STRBYTES];

    // Generate a fresh 16-byte random salt, run Argon2id with the cost params below, and encode into destination buffer
    if (crypto_pwhash_str(
            encoded,                           // Destination buffer for hash
            combinedSecret.data(),             // Pointer to combined secret bytes
            combinedSecret.size(),             // Combined secret length
            crypto_pwhash_OPSLIMIT_SENSITIVE,  // CPU cost (about 2 Argon2 passes)
            crypto_pwhash_MEMLIMIT_SENSITIVE)  // RAM cost (19 MiB)
        != 0)
    {
        throw std::runtime_error("Error: hashing failed, likely due to running out of memory.");
    }

    // Return final encoded text: internal stuff + cost params + salt + hash
    return std::string(encoded);
}

//bool Hasher::compareHashes(const std::string& hashA, const std::string& hashB)
//{
//    return constantTimeCompare(hashA, hashB);
//}
//
//bool Hasher::constantTimeCompare(const std::string& a, const std::string& b)
//{
//    if (a.length() != b.length()) {
//        return false;
//    }
//
//    bool result = true;
//    for (size_t i = 0; i < a.length(); ++i) {
//        result &= (a[i] == b[i]);
//        // This checks ALL characters regardless of early mismatches
//    }
//    return result;
//}