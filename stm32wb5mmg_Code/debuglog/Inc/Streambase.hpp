/**
 * @file Streambase.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once
/**
 * @brief Base class to provide << and >> operators, it should used only for debug log printing,
 * It doesn't implement lot of error cases, use it with care.
 * 
 */

class Streambase {

    protected:
      virtual ~Streambase() {}

    public:

      enum {
        end_of_Stream=-1,

        Stream_Error=-2,

        Operation_Not_Supported=-3
      };

    public:

      /**
       * Close the stream. 
       * @return 
       */

      virtual bool close()=0;
  };

