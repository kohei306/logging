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

#include <iostream>
#include <fstream>
#include <algorithm>

#include "logging_configurator.h"

namespace logging {

bool LoggingConfigurator::ReadConfigFromFile(const std::string & file_name) {
  bool result = true;
  std::ifstream cFile (file_name);
  enum class Section {
    DEFAULT_APPENDER,
    LOGGER,
    NONE
  };

  enum class SubSection {
    LOGGER_APPENDER,
    NONE
  };

  if (cFile.is_open()) {
    std::string line;
    Section section_state = Section::NONE;
    SubSection sub_section_state = SubSection::NONE;
    std::unique_ptr<AppenderConfig> appender_config(nullptr);
    std::unique_ptr<LoggerConfig> logger_config(nullptr);
    while(getline(cFile, line)){
      line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
      if(line[0] == '#' || line.empty()) {
        continue;
      }
      if (line[0] == '[') {
        auto delimiterPos = line.find("]");
        auto section = line.substr(1, delimiterPos - 1);
        if (section == "DefaultAppender") {
          section_state = Section::DEFAULT_APPENDER;
          sub_section_state = SubSection::NONE;
        } else if (section == "Logger") {
          section_state = Section::LOGGER;
          sub_section_state = SubSection::NONE;
        } else if (section == "LoggerAppender") {
          sub_section_state = SubSection::LOGGER_APPENDER;
        } else {
        }
      }
      auto delimiterPos = line.find("=");
      auto name = line.substr(0, delimiterPos);
      auto value = line.substr(delimiterPos + 1);

      switch (section_state) {
        case(Section::DEFAULT_APPENDER): {
          if (name == "AppenderType") {
            if (value == AppenderTypelToString(AppenderType::CONSOLE)) {
              appender_config = std::make_unique<AppenderConfig>(AppenderType::CONSOLE, "");
            } else if (value == AppenderTypelToString(AppenderType::FILE)) {
              appender_config = std::make_unique<FileAppenderConfig>(AppenderType::FILE, "");
            } else if (value == AppenderTypelToString(AppenderType::ARALOG)) {
              appender_config = std::make_unique<AraLogAppenderConfig>(AppenderType::ARALOG, "AraLogAppenderDefaultName", "DFT", "Default App description", "ARA_CONSOLE", "/tmp/");
            } else if (value == AppenderTypelToString(AppenderType::SYSLOG)) {
              appender_config = std::make_unique<AppenderConfig>(AppenderType::SYSLOG, "");
            } else if (value == AppenderTypelToString(AppenderType::NET)) {
              appender_config = std::make_unique<AppenderConfig>(AppenderType::NET, "");
            } else {
            }
          } else if (name == "AppenderName") {
            appender_config->m_name = value;
          } else if (name == "LogLevel") {
            appender_config->m_level = LogLevellFromString(value);
          } else if (name == "CustomParameters"){
            appender_config->InitFromCustomParametersStr(value);
            if (appender_config->IsValidConfig()) {
              m_default_appender_configs.push_back(std::move(appender_config));
            } else {
              std::cout << "AppenderConfig invalid." << std::endl;
            }
          } else {
          }
        break;
        }
        case(Section::LOGGER): {
           if (name == "LoggerName") {
        	 logger_config = std::make_unique<LoggerConfig>();
             logger_config->m_name = value;
           } else if (name == "LogLevel" && sub_section_state == SubSection::NONE) {
             if (LogLevellFromString(value) == LogLevel::FATAL) {
               logger_config->m_log_level = LogLevel::FATAL;
             } else if (LogLevellFromString(value) == LogLevel::ERROR) {
               logger_config->m_log_level = LogLevel::ERROR;
             } else if (LogLevellFromString(value) == LogLevel::WARN) {
               logger_config->m_log_level = LogLevel::WARN;
             } else if (LogLevellFromString(value) == LogLevel::INFO) {
               logger_config->m_log_level = LogLevel::INFO;
             } else if (LogLevellFromString(value) == LogLevel::DEBUG) {
               logger_config->m_log_level = LogLevel::DEBUG;
             } else if (LogLevellFromString(value) == LogLevel::VERBOSE) {
               logger_config->m_log_level = LogLevel::VERBOSE;
             } else if (LogLevellFromString(value) == LogLevel::OFF) {
               logger_config->m_log_level = LogLevel::OFF;
             } else if (LogLevellFromString(value) == LogLevel::NOT_SELECTED) {
               logger_config->m_log_level = LogLevel::NOT_SELECTED;
             } else {
             }
           } else if (name == "AppenderType") {
             if (value == AppenderTypelToString(AppenderType::CONSOLE)) {
               appender_config = std::make_unique<AppenderConfig>(AppenderType::CONSOLE, "");
             } else if (value == AppenderTypelToString(AppenderType::FILE)) {
               appender_config = std::make_unique<FileAppenderConfig>(AppenderType::FILE, "");
             } else if (value == AppenderTypelToString(AppenderType::ARALOG)) {
               appender_config = std::make_unique<AraLogAppenderConfig>(AppenderType::ARALOG, "AraLogAppenderDefaultName", "DFT", "Default App description", "ARA_CONSOLE", "/tmp/");
             } else if (value == AppenderTypelToString(AppenderType::SYSLOG)) {
               appender_config = std::make_unique<AppenderConfig>(AppenderType::SYSLOG, "");
             } else if (value == AppenderTypelToString(AppenderType::NET)) {
               appender_config = std::make_unique<AppenderConfig>(AppenderType::NET, "");
             } else {
             }
           } else if (name == "AppenderName") {
             appender_config->m_name = value;
           } else if (name == "LogLevel" && sub_section_state == SubSection::LOGGER_APPENDER) {
             appender_config->m_level = LogLevellFromString(value);
           } else if (name == "CustomParameters"){
             appender_config->InitFromCustomParametersStr(value);
             if (appender_config->IsValidConfig()) {
               logger_config->m_appender_configs.push_back(std::move(appender_config));
               m_logger_configs.push_back(std::move(logger_config));
             } else {
               std::cout << "AppenderConfig invalid." << std::endl;
             }
           } else {
           }
        break;
        }
        case(Section::NONE): {
        break;
        }
        default: {
        break;
        }
      }
    }
  }
  else {
    std::cerr << "Couldn't open config file for reading." << std::endl;
    result = false;
  }

  cFile.close();
  return result;
}

void LoggingConfigurator::Clear() {
  m_default_appender_configs.clear();
  m_logger_configs.clear();
}

}  // namespace logging
