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

#ifndef INCLUDE_LOGGING_LOGGER_MAP_H_
#define INCLUDE_LOGGING_LOGGER_MAP_H_

#include <thread>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include "logging/logger.h"

namespace logging {

using LoggerPtrMap = std::map<std::string, std::unique_ptr<Logger>>;
using LoggerList = std::vector<LoggerRawPtr>;

class LoggerMap : public IAppenderAddable {
 public:
  LoggerMap();
  ~LoggerMap();

  bool IsLoggerExists(const std::string& name) const;

  std::size_t GetNumLoggers() const { return m_loggers->size(); }

  LoggerRef GetLogger(const std::string& name) const;

  AppenderAddableError AddAppender(std::unique_ptr<AppenderConfig> new_appender_config) final;

  AppenderList& GetAllAppenders() override;

  AppenderRawPtr GetAppender(const std::string& name) const final;

  bool AddLogger(const std::string& name, LogLevel log_level = LogLevel::NOT_SELECTED);

  bool AppenderExist(const std::string & appender_name) final;
  std::size_t GetNumDefaultAppenders() const { return m_defalut_appenders.size(); }

  void RemoveAllAppenders() final;

  void RemoveAppender(const std::string& name) final;

  void WriteToDefaultAppenders(const LogEvent& log_event);

  void Clear();
  void ClearDefaultAppenders();
  void ClearLoggers();
  LoggerList GetCurrentLoggers() const;

 private:
  void CloseDefaultAppenders();
  void CloseLoggers();
  void AddAppenderToLogger(const AppenderConfig& appender_config);
  AppenderAddableError AddAppender(AppenderUnqPtr new_appender) final;

  std::unique_ptr<LoggerPtrMap> m_loggers;
  AppenderList m_defalut_appenders;

  mutable std::mutex m_mtx;
  mutable std::recursive_mutex m_rmtx;
};

}  // namespace logging

#endif  // INCLUDE_LOGGING_LOGGER_MAP_H_
