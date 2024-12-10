/**
 * @brief Provides functioanlity to read from input. The base class implementes the >> operator
 * and this class provies the worker functions.
 * 
 */

#pragma once

#include <cstdint>
#include "Streambase.hpp"

  class InputStream : public Streambase {

    protected:
      InputStream& InputRead(void *buffer,uint32_t bufferSize);

    public:
      virtual ~InputStream() {}


      InputStream& operator>>(int8_t& val);
      InputStream& operator>>(int16_t& val);
      InputStream& operator>>(int32_t& val);
      InputStream& operator>>(uint8_t& val);
      InputStream& operator>>(uint16_t& val);
      InputStream& operator>>(uint32_t& val);

      
      /**
       * @brief Blocks until data is available, and returns a byte of data. if the read fails then returns stream error
       * or the end of stream is reached then returns end of stream.
       * 
       * @return int16_t 
       */
      
      virtual int16_t read()=0;

      /**
       * @brief Retuns a number of bytes read, takes two size one is requested size and another one is actually read.
       * 
       * @param buffer 
       * @param size 
       * @param actuallyRead 
       * @return true 
       * @return false 
       */
      

      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead)=0;

      /**
       * @brief How much data to seek
       * 
       * @param bytesToSkip 
       * @return true 
       * @return false 
       */
      virtual bool skip(uint32_t bytesToSkip)=0;

      /**
       * @brief check if data is available in stream. to read.
       * 
       * @return true 
       * @return false 
       */
      virtual bool available()=0;


     /**
      * @brief reset to start of stream 
      * 
      * @return true 
      * @return false 
      */
      virtual bool reset()=0;
  };

