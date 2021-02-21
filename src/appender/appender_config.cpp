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
#include <sstream>
#include <vector>
#include "logging/appender_config.h"

namespace logging {

AppenderConfig::AppenderConfig() : m_appender_type(AppenderType::NONE), m_level(LogLevel::VERBOSE) {}

AppenderConfig::AppenderConfig(AppenderType appender_type, std::string name, LogLevel m_level) : m_appender_type(appender_type), m_level(m_level), m_name(name) {}

bool AppenderConfig::IsValidConfig() {
  return ((m_appender_type != AppenderType::NONE) && (m_name != ""));
}

AraLogAppenderConfig::AraLogAppenderConfig(AppenderType appender_type, std::string name) : AppenderConfig::AppenderConfig(appender_type, name) {}

AraLogAppenderConfig::AraLogAppenderConfig(AppenderType appender_type, std::string name, std::string app_id, std::string app_description, std::string log_mode, std::string directory_path)
  : AppenderConfig::AppenderConfig(appender_type, name),  m_app_id(app_id), m_app_description(app_description), m_log_mode(log_mode), m_directory_path(directory_path) {}

bool AraLogAppenderConfig::IsValidConfig() { return ((m_appender_type != AppenderType::NONE) && (m_name != "") && (m_app_id != "")
                                                && (m_app_description != "") && (m_log_mode !="") && (m_directory_path != ""));}

void AraLogAppenderConfig::InitFromCustomParametersStr(const std::string & custom_parameters) {
  std::stringstream ss(custom_parameters);
  std::vector<std::string> custom_parameter_key_values;
  while ( ss.good() ) {
    std::string parameter;
    std::getline(ss, parameter, ',');
    custom_parameter_key_values.push_back(parameter);
  }

  for (auto key_value : custom_parameter_key_values) {
    auto delimiterPos = key_value.find(":");
    auto name = key_value.substr(0, delimiterPos);
    auto value = key_value.substr(delimiterPos + 1);

    if (name == "AppID") {
      m_app_id = value;
    } else if (name == "AppDescription") {
      m_app_description = value;
    } else if (name == "AraLogMode") {
      m_log_mode = value;
    } else if (name == "OutputFileDirectory") {
      m_directory_path = value;
    } else {
    }
  }
  std::cout << "CustomParameters: " << m_app_id << ", " << m_app_description << ", " << m_log_mode << ", " << m_directory_path << std::endl;
}

FileAppenderConfig::FileAppenderConfig(AppenderType appender_type, std::string name) : AppenderConfig::AppenderConfig(appender_type, name), m_output_file_path("/tmp/"), m_add_timestamp_to_file_name("TRUE"), m_file_name_prefix("default_prefix_"),  m_open_mode("") {}

FileAppenderConfig::FileAppenderConfig(AppenderType appender_type, std::string name, std::string output_file_path, std::string add_timestamp_to_file_name = "TRUE", std::string file_name_prefix = "default_prefix_", std::string open_mode = "APPEND")
  : AppenderConfig::AppenderConfig(appender_type, name), m_output_file_path(output_file_path), m_add_timestamp_to_file_name(add_timestamp_to_file_name), m_file_name_prefix(file_name_prefix), m_open_mode(open_mode) {}

FileAppenderConfig::FileAppenderConfig(AppenderType appender_type, std::string name, LogLevel level, std::string output_file_path, std::string add_timestamp_to_file_name = "TRUE", std::string file_name_prefix = "default_prefix_", std::string open_mode = "APPEND")
  : AppenderConfig::AppenderConfig(appender_type, name, level), m_output_file_path(output_file_path), m_add_timestamp_to_file_name(add_timestamp_to_file_name), m_file_name_prefix(file_name_prefix), m_open_mode(open_mode) {}

bool FileAppenderConfig::IsValidConfig() { return ((m_appender_type != AppenderType::NONE) && (m_name != "") && (m_output_file_path != "")); }

void FileAppenderConfig::InitFromCustomParametersStr(const std::string & custom_parameters) {
  std::stringstream ss(custom_parameters);
  std::vector<std::string> custom_parameter_key_values;
  while ( ss.good() ) {
    std::string parameter;
    std::getline(ss, parameter, ',');
    custom_parameter_key_values.push_back(parameter);
  }

  for (auto key_value : custom_parameter_key_values) {
    auto delimiterPos = key_value.find(":");
    auto name = key_value.substr(0, delimiterPos);
    auto value = key_value.substr(delimiterPos + 1);

    if (name == "OutPutFileDirectory") {
      m_output_file_path  = value;
    } else if (name == "AddTimeStampToFileName") {
      m_add_timestamp_to_file_name = value;
    } else if (name == "OutPutFileNamePrefix") {
      m_file_name_prefix = value;
    } else if (name == "FileOpenMode") {
      m_open_mode = value;
    } else {
    }
  }
  std::cout << "CustomParameters: " << m_output_file_path << ", " << m_add_timestamp_to_file_name << ", " << m_file_name_prefix << ", " << m_open_mode << std::endl;
}

}  // namespace logging
