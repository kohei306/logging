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

#ifndef INCLUDE_LOGGING_LOGGER_H_
#define INCLUDE_LOGGING_LOGGER_H_

#include <string>
#include <memory>
#include <mutex>
#include <atomic>
#include <iostream>

#include "logging/appender_addable_interface.h"

namespace logging {

class Logger;
using LoggerRawPtr = Logger*;
using LoggerRef = Logger &;
using LoggerUnqPtr = std::unique_ptr<Logger>;

class Logger : public IAppenderAddable {
 public:
  ~Logger() = default;
  // These could throw exception.
  void LogFatal(const std::string &message) const;
  void LogError(const std::string &message) const;
  void LogWarn(const std::string &message) const;
  void LogInfo(const std::string &message) const;
  void LogInfoRawBuffer(const std::string &message) const;
  void LogDebug(const std::string &message) const;
  void LogVerbose(const std::string &message) const;
  bool IsFatalEnabled() const;
  bool IsErrorEnabled() const;
  bool IsWarnEnabled() const;
  bool IsInfoEnabled() const;
  bool IsDebugEnabled() const;
  bool IsVerboseEnabled() const;

  void Write(LogLevel log_level, const std::string & message) const;

  void CloseAllAppenders();

  const std::string & GetName() const { return m_name;}
  LogLevel GetLogLevel() const { return m_level;}
  bool GetUseDefaultAppender() const { return m_use_default_appender; }

  // In case it is configured from logging config file, this call doesn't overwrite.
  // Normally, it is better to use this function to set LogLevel in code so that it can be changed
  // By config file if it is needed.
  void SetLogLevel(const LogLevel log_level) {
    if (m_level == LogLevel::NOT_SELECTED) {
      m_level = log_level;
    }
  }

  void SetUseDefaultAppender(bool use_default_appender) { m_use_default_appender = use_default_appender; }

  AppenderAddableError AddAppender(std::unique_ptr<AppenderConfig> new_appender_config) final;
  AppenderAddableError AddAppender(AppenderUnqPtr new_appender) final;

  AppenderList& GetAllAppenders() final;

  AppenderList GetDefaultAppenders() const;

  AppenderRawPtr GetAppender(const std::string& name) const final;

  bool AppenderExist(const std::string & appender_name) final;
  std::size_t GetNumAppenders() const { return m_appenders.size(); }

  void RemoveAllAppenders() final;

  void RemoveAppender(const std::string& name) final;

 protected:
  void WriteToAllAppenders(const LogEvent & log_event) const;
  AppenderList m_appenders;

 private:
  friend class LoggerMap;
  friend class LogManager;
  explicit Logger(const std::string &name, const LogLevel level, const bool use_default_appender = true);
  // This function will overwrite the log_level in any case.
  void SetLogLevelForce(const LogLevel log_level) {m_level = log_level;}

  std::string m_name;
  LogLevel m_level;
  std::atomic<bool> m_use_default_appender;
  mutable std::mutex m_mtx;
};

}  // namespace logging

// These could throw exception.
#define LOG_FATAL(logger, message) {\
        if (logger.IsFatalEnabled()) {\
           logger.LogFatal(message); }} \

#define LOG_ERROR(logger, message) {\
        if (logger.IsErrorEnabled()) {\
           logger.LogError(message); }} \

#define LOG_WARN(logger, message) {\
        if (logger.IsWarnEnabled()) {\
           logger.LogWarn(message); }} \

#define LOG_INFO(logger, message) { \
        if (logger.IsInfoEnabled()) {\
           logger.LogInfo(message); }} \

#define LOG_DEBUG(logger, message) {\
        if (logger.IsDebugEnabled()) {\
           logger.LogDebug(message); }} \

#define LOG_VERBOSE(logger, message) {\
        if (logger.IsVerboseEnabled()) {\
           logger.LogVerbose(message); }} \

#endif  // INCLUDE_LOGGING_LOGGER_H_
