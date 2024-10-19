/**
 * @file md5.cpp
 * @headerfile md5.h
 * @brief MD5 hash function implementation.
 * @author lqy
 * @date 2024-10-19
 */

/* Header files */
#include "md5.h"
#include <iostream>
#include <string>
#include <openssl/md5.h>

/* Function definitions */

std::string calculateMD5(const std::string& input)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), digest);

    char md5String[33]; // MD5 is 32 characters long + null terminator
    for (int i = 0; i < 16; ++i) {
        snprintf(&md5String[i * 2], 3, "%02x", (unsigned int)digest[i]);
    }
    return std::string(md5String);
}