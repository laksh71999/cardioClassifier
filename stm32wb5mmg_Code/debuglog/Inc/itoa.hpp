/**
 * @file itoa.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once 
#include <cstddef>
#include <cstdint>

size_t itoa10(int32_t value, char* buf);
size_t uitoa10(uint32_t value, char* buf);
size_t litoa10(int64_t value, char* buf);
size_t ulitoa10(uint64_t value, char* buf);
size_t cdtoa2(double value, char* buf, uint32_t precision);
char* uitoa16(uint32_t value, char* buf, uint32_t final);