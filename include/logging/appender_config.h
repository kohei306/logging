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

#ifndef INCLUDE_LOGGING_APPENDER_CONFIG_H_
#define INCLUDE_LOGGING_APPENDER_CONFIG_H_

#include <string>
#include <iostream>
#include "logging/log_level.h"

namespace logging {

enum class AppenderType {
  CONSOLE = 1,
  FILE = 2,
  NET = 3,
  SYSLOG = 4,
  ARALOG = 5,
  NONE = 6
};

const inline std::string AppenderTypelToString(AppenderType appender_type) {
  std::string result = "Default";
  if (appender_type == AppenderType::CONSOLE) result = "CONSOLE";
  else if (appender_type == AppenderType::FILE) result = "FILE";
  else if (appender_type == AppenderType::SYSLOG) result = "SYSLOG";
  else if (appender_type == AppenderType::ARALOG) result = "ARALOG";
  else if (appender_type == AppenderType::NET) result = "NET";
  else throw std::invalid_argument("Invalid appender type.");
  return result;
}

class AppenderConfig {
 public:
  AppenderConfig();
  virtual ~AppenderConfig() = default;
  explicit AppenderConfig(AppenderType appender_type, std::string name, LogLevel m_level = LogLevel::VERBOSE);
  virtual bool IsValidConfig();
  virtual void InitFromCustomParametersStr(const std::string & custom_parameters) {}
  AppenderType m_appender_type;
  LogLevel m_level;
  std::string m_name;
};

class AraLogAppenderConfig : public AppenderConfig {
 public:
  AraLogAppenderConfig(AppenderType appender_type, std::string name);
  explicit AraLogAppenderConfig(AppenderType appender_type, std::string name, std::string app_id, std::string app_description, std::string log_mode, std::string directory_path);
  bool IsValidConfig() override;
  void InitFromCustomParametersStr(const std::string & custom_parameters) override;
  std::string m_app_id;
  std::string m_app_description;
  std::string m_log_mode;
  std::string m_directory_path;
};

class FileAppenderConfig : public AppenderConfig {
 public:
  FileAppenderConfig(AppenderType appender_type, std::string name);
  explicit FileAppenderConfig(AppenderType appender_type, std::string name, std::string output_file_path, std::string add_timestamp_to_file_name, std::string file_name_prefix, std::string open_mode);
  explicit FileAppenderConfig(AppenderType appender_type, std::string name, LogLevel m_level, std::string output_file_path, std::string add_timestamp_to_file_name, std::string file_name_prefix, std::string open_mode);
  bool IsValidConfig() override;
  void InitFromCustomParametersStr(const std::string & custom_parameters) override;
  std::string m_output_file_path;
  std::string m_add_timestamp_to_file_name;
  std::string m_file_name_prefix;
  std::string m_open_mode;
};

}  // namespace logging

#endif  // INCLUDE_LOGGING_APPENDER_CONFIG_H_
