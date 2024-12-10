/**
 * @file SeggerRttInputOutputStream.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-07-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifdef SEGGER
#include "DebugOutputStream.hpp"
#include "SEGGER_RTT.h"
#endif
#ifdef ITM
#include "DebugOutputStream.hpp"
#include "Itmprint.h"
#endif

constexpr uint32_t bufferIndex = 0;

DebugOutputStream::DebugOutputStream() {}

bool
DebugOutputStream::write(uint8_t val)
{
#ifdef SEGGER
  SEGGER_RTT_Write(bufferIndex, reinterpret_cast<void*>(&val), 1);
#else
  __io_putchar(static_cast<int>(val));
#endif
  return true;
}

bool
DebugOutputStream::write(const void* buffer, uint32_t size)
{
#ifdef SEGGER
  SEGGER_RTT_Write(bufferIndex, buffer, size);
#else
  const uint8_t* ptr = reinterpret_cast<const uint8_t*>(buffer);
  while (size--) {
    __io_putchar(static_cast<int>(*ptr));
    ptr++;
  }
#endif
  return true;
}