/**
 * @file TextOutputStream.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include "OutputStream.hpp"
#include <cstring>
#include "itoa.hpp"


/**
 * @brief Wraps exisitng op stream and provies text conversion of numbers.
 * 
 */

  
  class TextOutputStream : public OutputStream {

    protected:
      OutputStream& _stream;

    public:

      TextOutputStream(OutputStream& stream);

      
      TextOutputStream& operator<<(const char *str);
      TextOutputStream& operator<<(char c);
      TextOutputStream& operator<<(int32_t val);
      TextOutputStream& operator<<(uint32_t val);
      TextOutputStream& operator<<(int16_t val);
      TextOutputStream& operator<<(uint16_t val);
      TextOutputStream& operator<<(int8_t val);
      TextOutputStream& operator<<(uint8_t val);
      TextOutputStream& operator<<(uint64_t val);
      TextOutputStream& operator<<(int64_t val);
      TextOutputStream& operator<<(const float & val);
      TextOutputStream& operator<<(double val);

      
      virtual bool write(uint8_t c) override;
      virtual bool write(const void *buffer,uint32_t size) override;
      virtual bool close() override;
      virtual bool flush() override;
  };


  
  inline TextOutputStream::TextOutputStream(OutputStream& stream)
    : _stream(stream) {

  }

  
  inline TextOutputStream& TextOutputStream::operator<<(const char *str) {
    write(str,strlen(str));
    return *this;
  }



  inline TextOutputStream& TextOutputStream::operator<<(char c) {
    write(c);
    return *this;
  }



  inline TextOutputStream& TextOutputStream::operator<<(int32_t val) {

    char buf[15];
    itoa10(val,buf);
    write(buf,strlen(buf));

    return *this;
  }

  inline TextOutputStream& TextOutputStream::operator<<(int64_t val) {

    char buf[30];
    litoa10(val,buf);
    write(buf,strlen(buf));

    return *this;
  }

  inline TextOutputStream& TextOutputStream::operator<<(uint64_t val) {

    char buf[30];
    ulitoa10(val,buf);
    write(buf,strlen(buf));

    return *this;
  }


  /**
   * Unsigned int writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(uint32_t val) {

    char buf[15];
    uitoa10(val,buf);
    write(buf,strlen(buf));

    return *this;
  }


  /**
   * Signed 16-bit writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(int16_t val) {
    return operator<<((int32_t)val);
  }


  /**
   * Unsigned 16-bit writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(uint16_t val) {
    return operator<<((uint32_t)val);
  }

/**
   * Unsigned 8-bit writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(uint8_t val) {
    return operator<<((uint32_t)val);
  }


  /**
   * Unsigned 8-bit writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(int8_t val) {
    return operator<<((int32_t)val);
  }

  /**
   * Double writer
   * @param val The double to write
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(double val) {

    char str[25];

    cdtoa2(val,str,6);
    write(str,strlen(str));
    return *this;
  }


  /**
   * Double writer
   *  @param val The double to write
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(const float& val ) {
    return operator<<(double(val));
  }


  /**
   * Write a byte - call through to the underlying stream
   * @param buffer The buffer
   * @param size The size
   * @return the underlying stream success result
   */

  inline bool TextOutputStream::write(uint8_t c) {
    return _stream.write(c);
  }


  /**
   * Write a buffer - call through to the underlying stream
   * @param buffer The buffer
   * @param size The size
   * @return the underlying stream success result
   */

  inline bool TextOutputStream::write(const void *buffer,uint32_t size) {
    return _stream.write(buffer,size);
  }


  /**
   * Close the stream (do nothing)
   * @return true
   */

  inline bool TextOutputStream::close() {
    return true;
  }


  /**
   * Flush the stream
   * @return the underlying stream flush result
   */

  inline bool TextOutputStream::flush() {
    return _stream.flush();
  }


