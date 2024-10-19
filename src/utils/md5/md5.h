/**
 * @file md5.h
 * @brief MD5 hash function encapsulation.
 * @author lqy
 * @date 2024-10-19
 */

#ifndef _SOCKET_MD5_H
#define _SOCKET_MD5_H

/* Header files */
#include <string>

/* Function prototypes */

/**
 * @brief Calculate MD5 hash of input string.
 * @param input The input string.
 * @return The MD5 hash of the input string.
 */
std::string calculateMD5(const std::string& input);

#endif /* _SOCKET_MD5_H */