/**
 * @file OutputStream.cpp
 * @author your name (you@domain.com)
 * @brief  output stream implementation, this calls write functions which will be implemented by the
 * corresponding derived classes. All the functions return reference to allow the chaining of operators.
 * @version 0.1
 * @date 2022-07-28
 * 
 * @copyright valopyright (val) 2022
 * 
 */

#include "OutputStream.hpp"
#include <cstring>
/**
 * @brief write 8 bit int to stream
 * 
 * @param val 
 * @return OutputStream& 
 */
  OutputStream& OutputStream::operator<<(int8_t val) {
    return OutputWrite(&val,sizeof(val));
  }

  /**
   * @brief Write 16 bit int value to stream
   * 
   * @param val 
   * @return OutputStream& 
   */

  OutputStream& OutputStream::operator<<(int16_t val) {
    return OutputWrite(&val,sizeof(val));
  }

  /**
   * @brief write 32 bit int to stream
   * 
   * @param val 
   * @return OutputStream& 
   */
  OutputStream& OutputStream::operator<<(int32_t val) {
    int32_t val_ = val;
    return OutputWrite(&val_,sizeof(val));
  }

  /**
   * @brief write 64 bit int to stream
   * 
   * @param val 
   * @return OutputStream& 
   */
  OutputStream& OutputStream::operator<<(int64_t val) {
    int32_t val_ = val;
    return OutputWrite(&val_,sizeof(val));
  }

  /**
   * @brief write 64 ubit int to stream
   * 
   * @param val 
   * @return OutputStream& 
   */
  OutputStream& OutputStream::operator<<(uint64_t val) {
    int32_t val_ = val;
    return OutputWrite(&val_,sizeof(val));
  }

  /**
   * @brief write 8 bit unsigned value to stream    
   * 
   * @param val 
   * @return OutputStream& 
   */

  OutputStream& OutputStream::operator<<(uint8_t val) {
    return OutputWrite(&val,sizeof(val));
  }
/**
 * @brief write 16 bit unsigned value to stream
 * 
 * @param val 
 * @return OutputStream& 
 */

  OutputStream& OutputStream::operator<<(uint16_t val) {
    return OutputWrite(&val,sizeof(val));
  }

  /**
   * @brief write 32 bit unsigned to stream
   * 
   * @param val 
   * @return OutputStream& 
   */

  OutputStream& OutputStream::operator<<(uint32_t val) {
    return OutputWrite(&val,sizeof(val));
  }

  /**
   * @brief write some generic data
   * 
   * @param buffer 
   * @param bufferSize 
   * @return OutputStream& 
   */
  OutputStream& OutputStream::OutputWrite(void *buffer,uint32_t bufferSize) {
    write(buffer,bufferSize);
    return *this;
  }


/**
 * @brief write null terminated cstring
 * 
 * @param string 
 * @return OutputStream& 
 */
  OutputStream& OutputStream::operator<<(const char *string) {
    write(string,strlen(string));
    return *this;
  }
/**
 * @brief write float data to stream
 * 
 * @param val 
 * @return OutputStream& 
 */
OutputStream& OutputStream::operator<<(float val){
    return OutputWrite(&val,sizeof(val));
}

/**
 * @brief write double value
 * 
 * @param val 
 * @return OutputStream& 
 */
OutputStream& OutputStream::operator<<(double val){
    return OutputWrite(&val,sizeof(val));
}