//! VTITAN CONFIDENTIAL
//! Copyright © 2014 – 2019 vTitan Corporation.
//! All Rights Reserved
//! Confidential Information. The information contained herein is, and remains
//! the property of vTitan Corporation Private Ltd, Chennai, India and vTitan
//! Corporation, Pleasanton, CA USA  and its suppliers, if any, The information
//! contained here shall not be copied, used, disclosed or reproduced except as
//! specifically authorized  by vTitan Corporation.
#ifndef ITMPRINT_TYPE
#define ITMPRINT_TYPE

#include "DebugOutputStream.hpp"
#include "TextOutputStream.hpp"
#include "debugLogger.hpp"

enum class txtcolor
{
  black,
  red,
  green,
  yellow,
  blue,
  magenta,
  cyan,
  white,
  bright_black,
  bright_red,
  bright_green,
  bright_yellow,
  bright_blue,
  bright_magenta,
  bright_cyan,
  bright_white,
  bg_black,
  bg_red,
  bg_green,
  bg_yellow,
  bg_blue,
  bg_magenta,
  bg_cyan,
  bg_white,
  bg_bright_black,
  bg_bright_red,
  bg_bright_green,
  bg_bright_yellow,
  bg_bright_blue,
  bg_bright_magenta,
  bg_bright_cyan,
  bg_bright_white,
  reset,
  clear
};

constexpr char*
getStrval(txtcolor t)
{
  switch (t) {
    case txtcolor::black:
      return (char*)"\x1B[2;30m";
      break;
    case txtcolor::red:
      return (char*)"\x1B[2;31m";
      break;
    case txtcolor::green:
      return (char*)"\x1B[2;32m";
      break;
    case txtcolor::yellow:
      return (char*)"\x1B[2;33m";
      break;
    case txtcolor::blue:
      return (char*)"\x1B[2;34m";
      break;
    case txtcolor::magenta:
      return (char*)"\x1B[2;35m";
      break;
    case txtcolor::cyan:
      return (char*)"\x1B[2;36m";
      break;
    case txtcolor::white:
      return (char*)"\x1B[2;37m";
      break;
    case txtcolor::bright_black:
      return (char*)"\x1B[1;30m";
      break;
    case txtcolor::bright_red:
      return (char*)"\x1B[1;31m";
      break;
    case txtcolor::bright_green:
      return (char*)"\x1B[1;32m";
      break;
    case txtcolor::bright_yellow:
      return (char*)"\x1B[1;33m";
      break;
    case txtcolor::bright_blue:
      return (char*)"\x1B[1;34m";
      break;
    case txtcolor::bright_magenta:
      return (char*)"\x1B[1;35m";
      break;
    case txtcolor::bright_cyan:
      return (char*)"\x1B[1;36m";
      break;
    case txtcolor::bright_white:
      return (char*)"\x1B[1;37m";
      break;
    case txtcolor::bg_black:
      return (char*)"\x1B[24;40m";
      break;
    case txtcolor::bg_red:
      return (char*)"\x1B[24;41m";
      break;
    case txtcolor::bg_green:
      return (char*)"\x1B[24;42m";
      break;
    case txtcolor::bg_yellow:
      return (char*)"\x1B[24;43m";
      break;
    case txtcolor::bg_blue:
      return (char*)"\x1B[24;44m";
      break;
    case txtcolor::bg_magenta:
      return (char*)"\x1B[24;45m";
      break;
    case txtcolor::bg_cyan:
      return (char*)"\x1B[24;46m";
      break;
    case txtcolor::bg_white:
      return (char*)"\x1B[24;47m";
      break;
    case txtcolor::bg_bright_black:
      return (char*)"\x1B[4;40m";
      break;
    case txtcolor::bg_bright_red:
      return (char*)"\x1B[4;41m";
      break;
    case txtcolor::bg_bright_green:
      return (char*)"\x1B[4;42m";
      break;
    case txtcolor::bg_bright_yellow:
      return (char*)"\x1B[4;43m";
      break;
    case txtcolor::bg_bright_blue:
      return (char*)"\x1B[4;44m";
      break;
    case txtcolor::bg_bright_magenta:
      return (char*)"\x1B[4;45m";
      break;
    case txtcolor::bg_bright_cyan:
      return (char*)"\x1B[4;46m";
      break;
    case txtcolor::bg_bright_white:
      return (char*)"\x1B[4;47m";
      break;
    case txtcolor::reset:
      return (char*)"\x1B[0m";
      break;
    case txtcolor::clear:
      return (char*)"\x1B[2J";
      break;
    default:
      return (char*)"";
      break;
  }
}

class ITMPrintf final : public loggerBase<ITMPrintf>
{
public:
  ITMPrintf(log_level_e l = LOG_LEVEL_LIMIT()) noexcept
    : loggerBase(l)
  {}

private:
  friend class loggerBase<ITMPrintf>;
  template<typename first, typename... Args>
  void _print(first f, Args&&... args)
  {
    static DebugOutputStream sg;
    static TextOutputStream sgop(sg);

    if constexpr (std::is_same<first, txtcolor>::value) {
      // sgop << getStrval(f); //NO Support for Text colors in ITM
      sgop << LOG_LEVEL_TO_C_STRING(level());
    } else {
      sgop << LOG_LEVEL_TO_C_STRING(level());
      sgop << f;
    }
    //(sgop <<  ... <<  std::forward<Args>(args));
    ((sgop << " " << std::forward<Args>(args)), ...);
  }
};

#endif /*ITM_TYPE*/