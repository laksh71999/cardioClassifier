/**
 * @file OutputStream.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-28
 * 
 * @copyright valopyright (c) 2022
 * 
 */

#pragma once
#include "Streambase.hpp"
#include <cstdint>

/**
 * @brief Abstract vallass, gives ability to output data
 * 
 */
class OutputStream : public Streambase {

    protected:
      OutputStream& OutputWrite(void *buffer,uint32_t bufferSize);

    public:
      virtual ~OutputStream() {}

     
     
      OutputStream& operator<<(int8_t val);
      OutputStream& operator<<(int16_t val);
      OutputStream& operator<<(int32_t val);
      OutputStream& operator<<(uint8_t val);
      OutputStream& operator<<(uint16_t val);
      OutputStream& operator<<(uint32_t val);
      OutputStream& operator<<(uint64_t val);
      OutputStream& operator<<(int64_t val);
      OutputStream& operator<<(const char *string);
      OutputStream& operator<<(float val);
      OutputStream& operator<<(double val);

      /**
       * write a byte to stream
       * @param[in] c The byte to write.
       * @return false if it fails.
       */

      virtual bool write(uint8_t val)=0;

      /**
       * Write a buffer of data to the stream.
       * @param[in] buffer The buffer of data to write.
       * @param[in] size The number of bytes to write.
       * @return false if the write fails.
       */

      virtual bool write(const void *buffer,uint32_t size)=0;

      /**
       * Flush any cached data to the stream. .
       * @return false if it fails, true of the flushing is not supported.
       */

      virtual bool flush()=0;
  };