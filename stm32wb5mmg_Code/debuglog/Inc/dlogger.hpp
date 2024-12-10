
//! VTITAN CONFIDENTIAL
//! Copyright © 2014 – 2019 vTitan Corporation.
//! All Rights Reserved
//! Confidential Information. The information contained herein is, and remains
//! the property of vTitan Corporation Private Ltd, Chennai, India and vTitan
//! Corporation, Pleasanton, CA USA  and its suppliers, if any, The information
//! contained here shall not be copied, used, disclosed or reproduced except as
//! specifically authorized  by vTitan Corporation.
#ifndef DLOGGER_HPP
#define DLOGGER_HPP

#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_DEBUG_NO_TAG 5
#define LOG_BINARY 6
#define LOG_LEVEL_MAX LOG_BINARY
#define LOG_LEVEL_COUNT (LOG_LEVEL_MAX + 1)

#ifndef LOG_LEVEL
/*Default is Debug*/
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#if LOG_LEVEL != LOG_LEVEL_OFF
#ifdef SEGGER
#include "../Segger/Inc/seggerLogger.hpp"
using debugLog = dbgLogger_t<SeggerPrintf>;
#endif
#ifdef ITM
#include "../ITMLogger/Inc/ItmLogger.hpp"
using debugLog = dbgLogger_t<ITMPrintf>;
#endif
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#ifndef logerror
#define logerror(...) debugLog::error(__VA_ARGS__)
#endif
#else
#define logerror(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#ifndef logwarning
#define logwarning(...) debugLog::warning(__VA_ARGS__)
#endif
#else
#define logwarning(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#ifndef loginfo
#define loginfo(...) debugLog::info(__VA_ARGS__)
#endif
#else
#define loginfo(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#ifndef logdebug
#define logdebug(...) debugLog::debug(__VA_ARGS__)
#endif
#else
#define logdebug(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#ifndef logdebugnotag
#define logdebugnotag(...) debugLog::debugnotags(__VA_ARGS__)
#endif
#else
#define logdebugnotag(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#ifndef logbinary
#define logbinary(...) debugLog::logbinary(__VA_ARGS__)
#endif
#else
#define logbinary(...)
#endif

#if LOG_LEVEL > LOG_LEVEL_OFF
#define loglevel(lvl) debugLog::level(lvl)
#else
#define loglevel(lvl)
#endif
#define TRACEFN() (filename(__FILE__), " ", __FUNCTION__, " ", __LINE__, " ")
#define TRACELINE() (filename(__FILE__), __LINE__, " ")

#endif