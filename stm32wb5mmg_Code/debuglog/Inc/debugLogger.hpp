//! VTITAN CONFIDENTIAL
//! Copyright © 2014 – 2019 vTitan Corporation.
//! All Rights Reserved
//! Confidential Information. The information contained herein is, and remains
//! the property of vTitan Corporation Private Ltd, Chennai, India and vTitan
//! Corporation, Pleasanton, CA USA  and its suppliers, if any, The information
//! contained here shall not be copied, used, disclosed or reproduced except as
//! specifically authorized  by vTitan Corporation.
#ifndef DEBUGLOGGER_HPP
#define DEBUGLOGGER_HPP

#include "dlogger.hpp"
#include <array>
#include <string_view>
#include <utility>

enum log_level_e
{
  off = LOG_LEVEL_OFF,
  error = LOG_LEVEL_ERROR,
  warning = LOG_LEVEL_WARNING,
  info = LOG_LEVEL_INFO,
  debug = LOG_LEVEL_DEBUG,
  debugnotags = LOG_LEVEL_DEBUG_NO_TAG,
  logbinary = LOG_BINARY
};

constexpr log_level_e
LOG_LEVEL_LIMIT() noexcept
{
  return static_cast<log_level_e>(LOG_LEVEL_MAX);
}

struct logNames
{
  constexpr static const char* level_names[LOG_LEVEL_COUNT] = {
    "", "ERR: ", "WAR: ", "INF: ", "DBG: ", "", ""
  };
};

constexpr const char*
LOG_LEVEL_TO_C_STRING(log_level_e level)
{
  return logNames::level_names[level];
}

constexpr auto
filename(std::string_view path)
{
  return path.substr(path.find_last_of('/') + 1).data();
}
using cstr = const char* const;
#define STRINGPASTE(x) #x
#define TOSTRING(x) STRINGPASTE(x)
#define TRACE()                                                                \
  ({                                                                           \
    constexpr cstr sf__{ filename(__FILE__ ":" TOSTRING(__LINE__) "=>") };     \
    sf__;                                                                      \
  })
template<typename D>
class loggerBase
{
public:
  log_level_e level() const noexcept { return level_; }

  log_level_e level(log_level_e l) noexcept
  {
    if (l <= LOG_LEVEL_LIMIT()) {
      level_ = l;
    }

    return level_;
  }

  template<typename first, typename... Args>
  void error(first f, const Args&... args)
  {
    log(log_level_e::error, f, std::forward<const Args>(args)...);
  }

  template<typename first, typename... Args>
  void warning(first f, const Args&... args)
  {
    log(log_level_e::warning, f, std::forward<const Args>(args)...);
  }

  template<typename first, typename... Args>
  void debug(first f, const Args&... args)
  {
    log(log_level_e::debug, f, std::forward<const Args>(args)...);
  }

  template<typename first, typename... Args>
  void debugnotags(first f, const Args&... args)
  {
    log(log_level_e::debugnotags, f, std::forward<const Args>(args)...);
  }

  template<typename first, typename... Args>
  void logbinary(first f, const Args&... args)
  {
    log(log_level_e::logbinary, f, std::forward<const Args>(args)...);
  }

  template<typename first, typename... Args>
  void info(first f, const Args&... args)
  {
    log(log_level_e::info, f, std::forward<const Args>(args)...);
  }

  template<typename first, typename... Args>
  void log(log_level_e l, first f, const Args&... args) noexcept
  {
    if (l <= level_) {
      // print(LOG_LEVEL_TO_C_STRING(l),',');
      print(f, args...);
    }
  }
  template<typename first, typename... Args>
  void print(first f, Args&&... args)
  {
    static_cast<D&>(*this).template _print(f, args...);
  }

private:
  log_level_e level_ = LOG_LEVEL_LIMIT();

protected:
  virtual ~loggerBase() = default;
  explicit loggerBase(log_level_e l = LOG_LEVEL_LIMIT()) noexcept
    : level_(l)
  {}
};

template<class TLogger>
class dbgLogger_t
{
public:
  dbgLogger_t() = default;
  ~dbgLogger_t() = default;

  static TLogger& inst()
  {
    static TLogger logger_;
    return logger_;
  }

  template<typename... Args>
  inline static void error(const Args&... args)
  {
    inst().error(std::forward<const Args>(args)...);
  }

  template<typename... Args>
  inline static void warning(const Args&... args)
  {
    inst().warning(std::forward<const Args>(args)...);
  }

  template<typename... Args>
  inline static void info(const Args&... args)
  {
    inst().info(std::forward<const Args>(args)...);
  }
  template<typename... Args>
  inline static void debug(const Args&... args)
  {
    inst().debug(std::forward<const Args>(args)...);
  }

  template<typename... Args>
  inline static void debugnotags(const Args&... args)
  {
    inst().debugnotags(std::forward<const Args>(args)...);
  }

  template<typename... Args>
  inline static void logbinary(const Args&... args)
  {
    inst().logbinary(std::forward<const Args>(args)...);
  }

  template<typename... Args>
  inline static void print(const Args&... args)
  {
    inst().print(std::forward<const Args>(args)...);
  }

  inline static log_level_e level(log_level_e l) { return inst().level(l); }

  inline static log_level_e level() { return inst().level(); }
};

template<std::string_view const&... Strs>
struct join
{
  /*Join all strings into a single std::array of chars*/
  static constexpr auto impl() noexcept
  {
    constexpr std::size_t len = (Strs.size() + ... + 0);
    std::array<char, len + 1> arr{};
    auto append = [i = 0, &arr](auto const& s) mutable {
      for (auto c : s)
        arr[i++] = c;
    };
    (append(Strs), ...);
    arr[len] = 0;
    return arr;
  }
  /* Give the joined string static storage*/
  static constexpr auto arr = impl();
  /*View as a std::string_view*/
  static constexpr std::string_view value{ arr.data(), arr.size() - 1 };
};

template<std::string_view const&... Strs>
static constexpr auto join_v = join<Strs...>::value;

#endif
