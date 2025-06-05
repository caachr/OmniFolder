//
// Created by Christian Caamano on 5/30/25.
//

#ifndef HASHER_H
#define HASHER_H

#include <string>
#include <iostream>
#include <sodium.h>

class Hasher {
public:
    static std::string hash(const std::initializer_list<std::string>& secrets);

    static bool compareHashes(const std::string& hashA, const std::string& hashB);

private:
    static bool constantTimeCompare(const std::string& a, const std::string& b);
};

#endif //HASHER_H
