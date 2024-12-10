/**
 * @file InputStream.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "InputStream.hpp"


/**
 * @brief 
 * 
 * @param c_ 
 * @return InputStream& 
 */
   InputStream& InputStream::operator>>(int8_t& val ) {
    return InputRead(&val,sizeof(val));
  }

  
  InputStream& InputStream::operator>>(int16_t& val) {
    return InputRead(&val,sizeof(val));
  }

  
  InputStream& InputStream::operator>>(int32_t& val) {
    return InputRead(&val,sizeof(val));
  }

  
  InputStream& InputStream::operator>>(uint8_t& val) {
    return InputRead(&val,sizeof(val));
  }

  
  InputStream& InputStream::operator>>(uint16_t& val) {
    return InputRead(&val,sizeof(val));
  }

  
  InputStream& InputStream::operator>>(uint32_t& val) {
    return InputRead(&val,sizeof(val));
  }


  
  InputStream& InputStream::InputRead(void *buffer_,uint32_t bufferSize_) {
    uint32_t actuallyRead;

    read(buffer_,bufferSize_,actuallyRead);
    return *this;
  }