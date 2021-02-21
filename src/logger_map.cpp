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

#include <string>
#include <algorithm>
#include <memory>
#include <iostream>
#include "logging/logger_map.h"
#include "logging/appender_factory.h"

namespace logging {

LoggerMap::LoggerMap() : m_loggers(new LoggerPtrMap()) {
  auto appender_config = std::make_unique<AppenderConfig>(AppenderType::CONSOLE, "DefaultConsoleAppender");
  m_defalut_appenders.push_back(AppenderFactory::CreateAppender<ConsoleAppender>(std::move(appender_config)));
}

LoggerMap::~LoggerMap() {
}

bool LoggerMap::IsLoggerExists(const std::string& name) const {
  bool result = true;
  LoggerPtrMap::iterator it = m_loggers->find(name);
  if (it == m_loggers->end()) {
    result = false;
  }
  return result;
}

LoggerRef LoggerMap::GetLogger(const std::string& name) const {
  std::lock_guard<std::recursive_mutex> lock(m_rmtx);
  LoggerPtrMap::iterator it = m_loggers->find(name);
  if (it != m_loggers->end()) {
    return *(it->second.get());
  } else {
    LogLevel logLevel(LogLevel::NOT_SELECTED);
    m_loggers->insert(LoggerPtrMap::value_type(name, std::unique_ptr<Logger>(new Logger(name, logLevel))));
    return GetLogger(name);
  }
}

AppenderAddableError LoggerMap::AddAppender(AppenderUnqPtr newAppender) {
  AppenderAddableError result = AppenderAddableError::NO_ERROR;
  std::lock_guard<std::mutex> lock(m_mtx);
  if (newAppender == nullptr) {
    return AppenderAddableError::ADDING_NULL_APPENDER;
  }

  AppenderList::iterator it = std::find_if(
  m_defalut_appenders.begin(), m_defalut_appenders.end(), [&newAppender](AppenderUnqPtr & appender) {
  bool result = false;
  if (appender->GetAppenderName() == newAppender->GetAppenderName()) {
    result = true;
  }
  return result;});

  if (it == m_defalut_appenders.end()) {
    m_defalut_appenders.push_back(std::move(newAppender));
  } else {
  result = AppenderAddableError::APPENDER_EXIST;
  }
  return result;
}

AppenderAddableError LoggerMap::AddAppender(std::unique_ptr<AppenderConfig> new_appender_config) {
  AppenderAddableError result = AppenderAddableError::NO_ERROR;
  switch (new_appender_config->m_appender_type) {
    case (AppenderType::CONSOLE) : {
  if (AppenderExist(new_appender_config->m_name) == false) {
    AppenderUnqPtr appender(AppenderFactory::CreateAppender<ConsoleAppender>(std::move(new_appender_config)));
    result = AddAppender(std::move(appender));
  } else {
    GetAppender(new_appender_config->m_name)->SetAppenderConfig(std::move(new_appender_config));
    result = AppenderAddableError::APPENDER_EXIST;
  }
    break;
  }
  case (AppenderType::ARALOG) : {
#ifdef ARALOG
    if (AppenderExist(new_appender_config->m_name) == false) {
      AppenderUnqPtr appender(AppenderFactory::CreateAppender<AraLogAppender>(std::move(new_appender_config)));
      result = AddAppender(std::move(appender));
    } else {
      GetAppender(new_appender_config->m_name)->SetAppenderConfig(std::move(new_appender_config));
      result = AppenderAddableError::APPENDER_EXIST;
    }
#endif
    break;
    }
    case (AppenderType::FILE) : {
    if (AppenderExist(new_appender_config->m_name) == false) {
      AppenderUnqPtr appender(AppenderFactory::CreateAppender<FileAppender>(std::move(new_appender_config)));
      result = AddAppender(std::move(appender));
    } else {
      GetAppender(new_appender_config->m_name)->SetAppenderConfig(std::move(new_appender_config));
      result = AppenderAddableError::APPENDER_EXIST;
    }
    break;
    }
    case (AppenderType::NET) : {
    break;
    }
    case (AppenderType::SYSLOG) : {
    break;
    }
    case (AppenderType::NONE) : {
    break;
    }
    default : {
    break;
    }
  }
  return result;
}

AppenderList& LoggerMap::GetAllAppenders() {
  std::lock_guard<std::mutex> lock(m_mtx);
  return m_defalut_appenders;
}


AppenderRawPtr LoggerMap::GetAppender(const std::string& name) const {
  std::lock_guard<std::mutex> lock(m_mtx);
  for (const auto & app : m_defalut_appenders) {
    if (app->GetAppenderConfig().m_name == name) {
      return app.get();
    }
  }
  return nullptr;
}

bool LoggerMap::AddLogger(const std::string& name, LogLevel log_level) {
  bool result = true;
  std::lock_guard<std::mutex> lock(m_mtx);
  LoggerPtrMap::iterator it = m_loggers->find(name);
  if (it != m_loggers->end()) {
    return false;
  } else {
    m_loggers->insert(LoggerPtrMap::value_type(name, std::unique_ptr<Logger>(new Logger(name, log_level))));
    return true;
  }

  return result;
}

bool LoggerMap::AppenderExist(const std::string & appender_name) {
  bool result = false;
  auto it = std::find_if(
  m_defalut_appenders.begin(), m_defalut_appenders.end(), [&appender_name](AppenderUnqPtr & appender_) {
    return (appender_->GetAppenderName() == appender_name);
  });

  if (it == m_defalut_appenders.end()) {
  } else {
    result = true;
  }

  return result;
}

void LoggerMap::RemoveAllAppenders() {
  std::lock_guard<std::mutex> lock(m_mtx);
  for (auto & app : m_defalut_appenders) {
    app->Close();
  }
  m_defalut_appenders.clear();
}

void LoggerMap::RemoveAppender(const std::string& name) {
  std::lock_guard<std::mutex> lock(m_mtx);

  AppenderList::iterator it, itEnd = m_defalut_appenders.end();

  for (it = m_defalut_appenders.begin(); it != itEnd; it++) {
    auto appender_config = it->get()->GetAppenderConfig();
    if (name == appender_config.m_name) {
      m_defalut_appenders.erase(it);
      return;
    }
  }
}

void LoggerMap::WriteToDefaultAppenders(const LogEvent& log_event) {
  std::lock_guard<std::mutex> lock(m_mtx);
  for (auto & appender : m_defalut_appenders) {
    appender->Send(log_event);
  }
}

void LoggerMap::Clear() {
  std::lock_guard<std::mutex> lock(m_mtx);
  CloseDefaultAppenders();
  CloseLoggers();

  m_loggers->clear();
  m_defalut_appenders.clear();
}

void LoggerMap::ClearDefaultAppenders() {
  std::lock_guard<std::mutex> lock(m_mtx);
  CloseDefaultAppenders();
  m_defalut_appenders.clear();
}

void LoggerMap::ClearLoggers() {
  std::lock_guard<std::mutex> lock(m_mtx);
  CloseLoggers();
  m_loggers->clear();
}

LoggerList LoggerMap::GetCurrentLoggers() const {
  std::lock_guard<std::mutex> lock(m_mtx);
  LoggerList logger_list;
  auto loggers = m_loggers.get();
  for (auto & logger : *loggers) {
    logger_list.push_back(logger.second.get());
  }
  return logger_list;
}

void LoggerMap::CloseDefaultAppenders() {
  for (auto & appender : m_defalut_appenders) {
    appender->Close();
  }
}

void LoggerMap::CloseLoggers() {
  auto loggers = m_loggers.get();
  for (auto & logger : *loggers) {
    logger.second->CloseAllAppenders();
    logger.second->RemoveAllAppenders();
  }
}

}  // namespace logging
