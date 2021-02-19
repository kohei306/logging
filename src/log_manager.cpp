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
#include <iostream>
#include <assert.h>
#include "log_manager.h"
#include "logger_map.h"

namespace logging {

LogManager::~LogManager() {
  try {
    Shutdown();
  }
  catch (const std::exception &e) { std::cerr << "Exception at LogManager destructor: " << e.what() << std::endl;}
}

AppenderAddableError LogManager::AddDefaultAppender(std::unique_ptr<AppenderConfig> new_appender_config) {
  return m_logger_map.AddAppender(std::move(new_appender_config));
}

void LogManager::RemoveDefaultAppender(const std::string appender_name) {
  m_logger_map.RemoveAppender(appender_name);
}

void LogManager::RemoveAllDefaultAppenders() {
  m_logger_map.RemoveAllAppenders();
}

LoggerRef LogManager::GetLogger(const std::string& name) const {
  return m_logger_map.GetLogger(name);
}

bool LogManager::IsLoggerExists(const std::string& name) const {
  return m_logger_map.IsLoggerExists(name);
}

std::size_t LogManager::GetNumLoggers() const {
  return m_logger_map.GetNumLoggers();
}

void LogManager::WriteToDefaultAppenders(const LogEvent& log_event) {
  m_logger_map.WriteToDefaultAppenders(log_event);
}

void LogManager::Shutdown() {
  m_logger_map.Clear();
}

void LogManager::PrintSummaryOfLogConfig() const {
  std::cout << "############ PrintSummaryOfLogConfig ##############" << std::endl;
  std::cout << "### Default Appenders ####" << std::endl;
  AppenderList & default_appenders = m_logger_map.GetAllAppenders();
  if (default_appenders.size() == 0) {
    std::cout << "NONE" << std::endl;
  }
  for (auto & default_appender : default_appenders) {
    std::cout << default_appender->GetAppenderName() << " : " << AppenderTypelToString(default_appender->GetAppenderType()) << " : " << LogLevellToString(default_appender->GetAppenderLogLevel())<< std::endl;
  }
  std::cout << "### Loggers ####" << std::endl;
  LoggerList loggers = m_logger_map.GetCurrentLoggers();
  for (auto logger : loggers) {
    std::cout << "Logger: " << logger->GetName() << " : " << LogLevellToString(logger->GetLogLevel()) << std::endl;
    AppenderList & appenders = logger->GetAllAppenders();
    if (appenders.size() == 0) {
      std::cout << "  has NO Appender" << std::endl;
    } else {
      std::cout << "  has Appenders" << std::endl;
    }

    for (auto & appender : appenders) {
      std::cout << "  " <<appender->GetAppenderName() << " : " << AppenderTypelToString(appender->GetAppenderType()) << " : " << LogLevellToString(appender->GetAppenderLogLevel()) << std::endl;
    }
  }
  std::cout << "############ PrintSummaryOfLogConfig ##############" << std::endl;
}

void LogManager::InitFromLogConfigFile(const std::string& file_name) {
  if (m_logging_configurator.ReadConfigFromFile(file_name)) {
    m_logger_map.ClearDefaultAppenders();

    for (auto & default_appender_config : m_logging_configurator.m_default_appender_configs) {
      std::cout << "Adding default appender.." <<  default_appender_config->m_name << " type: " << AppenderTypelToString(default_appender_config->m_appender_type) << std::endl;
      m_logger_map.AddAppender(std::move(default_appender_config));
    }

    for (auto & logger_config : m_logging_configurator.m_logger_configs) {
      std::cout << "Adding logger.." <<  logger_config->m_name << " type: " << LogLevellToString(logger_config->m_log_level) << std::endl;
      m_logger_map.AddLogger(logger_config->m_name);
      LoggerRef logger = GetLogger(logger_config->m_name);
      logger.SetLogLevelForce(logger_config->m_log_level);
      for (auto & appender_config : logger_config->m_appender_configs) {
        std::cout << "Adding Appender " << appender_config->m_name << " to "<<  logger_config->m_name << " type: " << LogLevellToString(appender_config->m_level) << std::endl;
        logger.AddAppender(std::move(appender_config));
      }
    }
  } else {
     LOG_ERROR(GetLogger("LogManager"), "Failed to read config file ")
  }

  m_logging_configurator.Clear();
  // For Debug
 // PrintSummaryOfLogConfig();
}

bool LogManager::IsDefaultAppenderExist(const std::string & appender_name) const {
  return m_logger_map.AppenderExist(appender_name);
}
std::size_t LogManager::GetNumDefaultAppenders() const {
  return m_logger_map.GetNumDefaultAppenders();
}

void LogManager::ConfigureLogging() {
}

} // namespace logging
