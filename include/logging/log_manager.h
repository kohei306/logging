// MIT License

// Copyright (c) 2018 Kohei Otsuka

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef INCLUDE_LOGGING_LOG_MANAGER_H_
#define INCLUDE_LOGGING_LOG_MANAGER_H_

#include <string>
#include <memory>
#include "logging/logger.h"
#include "logging/logger_map.h"
#include "logging/logging_configurator.h"

namespace logging {

class LogManager {
 public:
  using LoggerMapPtr = LoggerMap*;
  static LogManager& GetInstance() {
    static LogManager instance;
    return instance;
  }
  ~LogManager() noexcept;
  LoggerMapPtr GetLoggerMap() { return &m_logger_map; }
  AppenderAddableError AddDefaultAppender(std::unique_ptr<AppenderConfig> new_appender_config);
  void RemoveDefaultAppender(const std::string appender_name);
  void RemoveAllDefaultAppenders();
  LoggerRef GetLogger(const std::string& name) const;
  bool IsLoggerExists(const std::string& name) const;
  std::size_t GetNumLoggers() const;

  void Shutdown();
  void InitFromLogConfigFile(const std::string& file_name);
  bool IsDefaultAppenderExist(const std::string & appender_name) const;
  std::size_t GetNumDefaultAppenders() const;

  LogManager(LogManager const&) = delete;
  void operator = (LogManager const&) = delete;

  void PrintSummaryOfLogConfig() const;

 private:
  friend class Logger;
  LogManager() : m_logger_map() {}
  void WriteToDefaultAppenders(const LogEvent& log_event);
  void ConfigureLogging();

  mutable LoggerMap m_logger_map;
  LoggingConfigurator m_logging_configurator;
};

}  // namespace logging

#endif  // INCLUDE_LOGGING_LOG_MANAGER_H_
