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

#include <algorithm>

#include "logger.h"
#include "log_manager.h"
#include "appender_factory.h"
#include "log_event.h"
#include "log_level.h"
#include "appender_interface.h"

namespace logging {

Logger::Logger(const std::string &name, const LogLevel level, const bool use_default_appender)
  : IAppenderAddable{}, m_name(name), m_level(level), m_use_default_appender(use_default_appender), m_mtx() {
}

void Logger::LogFatal(const std::string &message) const {
  if (IsFatalEnabled() && m_level != LogLevel::OFF) {
    Write(LogLevel::FATAL, message);
  }
}

void Logger::LogError(const std::string &message) const {
  if (IsErrorEnabled() && m_level != LogLevel::OFF) {
    Write(LogLevel::ERROR, message);
  }
}

void Logger::LogWarn(const std::string &message) const {
  if (IsWarnEnabled() && m_level != LogLevel::OFF) {
  Write(LogLevel::WARN, message);
  }
}

void Logger::LogInfo(const std::string &message) const {
  if (IsInfoEnabled() && m_level != LogLevel::OFF) {
    Write(LogLevel::INFO, message);
  }
}

void Logger::LogInfoRawBuffer(const std::string &message) const {
  if (IsInfoEnabled() && m_level != LogLevel::OFF) {
      LogEvent log_event;
      log_event.m_log_level = LogLevel::INFO;
      log_event.m_message = message;
      log_event.m_logger_name = GetName();

      std::lock_guard<std::mutex> lock(m_mtx);

      if (m_use_default_appender) {
        LogManager::GetInstance().WriteToDefaultAppenders(log_event);
      }
      if (m_appenders.size() > 0) {
        WriteToAllAppenders(log_event);
      }
  }
}

void Logger::LogDebug(const std::string &message) const {
  if (IsDebugEnabled() && m_level != LogLevel::OFF) {
    Write(LogLevel::DEBUG, message);
  }
}

void Logger::LogVerbose(const std::string &message) const {
  if (IsVerboseEnabled() && m_level != LogLevel::OFF) {
    Write(LogLevel::VERBOSE, message);
  }
}

bool Logger::IsFatalEnabled() const {
  return m_level >= LogLevel::FATAL;
}

bool Logger::IsErrorEnabled() const {
  return m_level >= LogLevel::ERROR;
}

bool Logger::IsWarnEnabled() const {
  return m_level >= LogLevel::WARN;
}

bool Logger::IsInfoEnabled() const {
  return m_level >= LogLevel::INFO;
}

bool Logger::IsDebugEnabled() const {
  return m_level >= LogLevel::DEBUG;
}

bool Logger::IsVerboseEnabled() const {
  return m_level >= LogLevel::VERBOSE;
}

void Logger::Write(const LogLevel log_level, const std::string &message) const {
  LogEvent log_event;
  log_event.m_log_level = log_level;
  log_event.m_message = message;
  log_event.m_logger_name = GetName();

  std::lock_guard<std::mutex> lock(m_mtx);

  if (m_use_default_appender) {
    LogManager::GetInstance().WriteToDefaultAppenders(log_event);
  }

  if (m_appenders.size() > 0) {
    WriteToAllAppenders(log_event);
  }
}

void Logger::CloseAllAppenders() {
  for (AppenderList::iterator it = GetAllAppenders().begin(); it != GetAllAppenders().end(); ++it) {
    (*it)->Close();
  }
}

AppenderAddableError Logger::AddAppender(AppenderUnqPtr newAppender) {
  AppenderAddableError result = AppenderAddableError::NO_ERROR;
  std::lock_guard<std::mutex> lock(m_mtx);
  if (newAppender == nullptr) {
    return AppenderAddableError::ADDING_NULL_APPENDER;
  }

  AppenderList::iterator it = std::find_if(
         m_appenders.begin(), m_appenders.end(), [&newAppender](AppenderUnqPtr & appender) {
	  bool result = false;
	  if (appender->GetAppenderName() == newAppender->GetAppenderName()) {
	    result = true;
	  }
      return result;});

  if (it == m_appenders.end()) {
    m_appenders.push_back(std::move(newAppender));
  } else {
	result = AppenderAddableError::APPENDER_EXIST;
  }
  return result;
}

AppenderAddableError Logger::AddAppender(std::unique_ptr<AppenderConfig> new_appender_config) {
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

AppenderList& Logger::GetAllAppenders()  {
  std::lock_guard<std::mutex> lock(m_mtx);
  return m_appenders;
}


AppenderRawPtr Logger::GetAppender(const std::string &name) const {
  std::lock_guard<std::mutex> lock(m_mtx);
  if (name.empty()) {
    return nullptr;
  }

  for (const auto & app : m_appenders) {
    if (app->GetAppenderConfig().m_name == name) {
      return app.get();
    }
  }
  return nullptr;
}

bool Logger::AppenderExist(const std::string &appender_name) {
  bool result = false;
  auto it = std::find_if(
		  m_appenders.begin(), m_appenders.end(), [&appender_name](AppenderUnqPtr & appender_) {
	  return (appender_->GetAppenderName() == appender_name);
  });

  if (it == m_appenders.end()) {
  } else {
    result = true;
  }
  return result;
}

void Logger::RemoveAllAppenders() {
  std::lock_guard<std::mutex> lock(m_mtx);
  for (auto & app : m_appenders) {
    app->Close();
  }
  m_appenders.clear();
}

void Logger::RemoveAppender(const std::string &name) {
  std::lock_guard<std::mutex> lock(m_mtx);

  AppenderList::iterator it, itEnd = m_appenders.end();

  for (it = m_appenders.begin(); it != itEnd; it++) {
    if (name == it->get()->GetAppenderConfig().m_name) {
      m_appenders.erase(it);
      return;
    }
  }
}

void Logger::WriteToAllAppenders(const LogEvent &log_event) const {
  for (auto & appender : m_appenders) {
    appender->Send(log_event);
  }
}

}  // namespace logging
